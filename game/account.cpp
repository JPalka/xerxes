#include "account.h"
#include <misc/utils.h>
#include <game/world.h>
#include <game/game.h>
#include <string>

#include <exceptions/wrongcredentials.h>
#include <exceptions/worldinactive.h>

bool Account::login ( std::string baseLink, std::string worldId ) {
	try
	{
		Client client;
		client.getUrl ( baseLink, this->cookiesFile );
		std::string postLogin = "username=" + this->username + "&password=" + this->password + "&remember=1";

		std::fstream loginFile;
		loginFile.open ( "login.html", std::ios::out );

		std::string beforeLogin = client.postUrl ( baseLink + "/page/auth", postLogin, this->cookiesFile );

		bsoncxx::document::value doc = bsoncxx::from_json ( beforeLogin );
		bsoncxx::document::element error = doc.view ()["error"];
		if ( error ) {
//			std::cout << error.get_utf8 ().value << std::endl;
			WrongCredentials ex ( this->username, this->password, baseLink );
			throw ex;
		}
//		std::string error = bsoncxx::to_json ( doc.view ()["error"] );
//		std::cout << error << std::endl;

		std::string afterLogin = client.getUrl ( "https://www.tribalwars.net/en-dk/page/play/" + worldId, this->cookiesFile );
		loginFile << afterLogin;
		loginFile.close();
		std::regex reg ( "token=[a-z0-9]*" );
		std::smatch match;
		std::regex_search ( afterLogin, match, reg );
		std::string s = match[0].str ();
		std::fstream finalLogin;
		std::string finalLoginString;
		finalLogin.open ( "finalLogin.html", std::ios::out );
		finalLoginString = client.getUrl ( baseLink + "/login.php?" + s, this->cookiesFile );
		finalLogin << finalLoginString;
		finalLogin.close();

		//Set csrf token
		std::string resultJson = Utils::extractGameDataJson ( finalLoginString );
		try {
			bsoncxx::document::value docGameData = bsoncxx::from_json ( resultJson );
			std::string token ( docGameData.view ()["csrf"].get_utf8 ().value );
			this->csrf = token;
			//Set player id of that account
			std::string tempId ( docGameData.view ()["player"]["id"].get_utf8().value );
			this->playerId = std::stoi (tempId);
		}
		catch ( bsoncxx::exception &ex ) {
//			Game::logCallback ( "Cannot extract csrf token. Most likely wrong world" );
			WorldInactive newEx ( worldId );
			throw newEx;
		}
	}

	catch(curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	catch(curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
}

Account::Account ( std::string username, std::string password )
: username(username), password(password) {
	this->cookiesFile = username + "-cookies.txt";
}
std::string Account::getBuildingInfo ( World &world, int villageId, std::string buildingName, std::string mode ) {
	std::string villageSlug = world.findVillage ( villageId )->getUrlSlug ();
	Building &building = ( *world.findVillage ( villageId ) )[buildingName];
	std::string buildingSlug = building.getUrlSlug ();
	std::string requestURL = world.getLink() + "/game.php?" + villageSlug + buildingSlug;
	//append mode if applicable
	if ( mode.size () > 0 ) {
		requestURL += "&mode=" + mode;
	}
//	std::cout << requestURL << std::endl;
	try {
		Client client;
		std::string resultHtml = client.getUrl ( requestURL, this->cookiesFile );
		return resultHtml;
	}
	catch(curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
		return "false";
	}
	catch(curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
		return "false";
	}
	return requestURL;
}

//OI! hardcoded recruitment in barracks only.
//TODO: add argument with building name or reference so it can recruit from them too
int Account::recruitUnit ( World &world, int villageId, std::string unitName, int count ) {
	std::string villageSlug = world.findVillage ( villageId )->getUrlSlug ();
	std::string requestURL = world.getLink() + "/game.php?" + villageSlug + "&screen=barracks&ajaxaction=train&mode=train";
//	std::cout << requestURL << std::endl;
	std::string postString = "units[" + unitName + "]=" + std::to_string (count) + "&h=" + this->csrf;
	Client client;
	std::string postRecruit = client.postUrl ( requestURL, postString, this->cookiesFile );
	std::fstream recruitFile;
	recruitFile.open ( "recruitment.html", std::ios::out );
	recruitFile << postRecruit;
	recruitFile.close ();
	// TODO: do proper testing and error handling for that
	bsoncxx::document::value doc = bsoncxx::from_json ( postRecruit );
	bsoncxx::document::element errorElement = doc.view ()["error"];
	try {
		//checking if errorElement contains anything
		std::string errorMsg ( errorElement.get_utf8 ().value ); //This throws if there is no error
		std::cout << "Recruitment order of " << count << " " << unitName << " failed. ";
		std::cout << errorMsg << std::endl;
		throw std::string ("fit");
	}
	catch ( bsoncxx::exception ex ) {
		//If setting errorMsg throws then there is no error.
	}
	// Finding id of created order. It will be the last one found
	std::regex regId ( "cancelOrder\\((\\d+)\\)" );
	std::smatch matchId;

	std::string::const_iterator searchStart( postRecruit.cbegin() );
	std::string newOrderId;
	while ( regex_search ( searchStart, postRecruit.cend(), matchId, regId ) ) {
		newOrderId = matchId.str (1);
		searchStart = matchId.suffix().first;
	}
	return std::stoi ( newOrderId );
}
std::string Account::startScavenging ( World &world, int villageId,
std::vector<std::pair<std::string, int>> units, int option, int carryMax ) {
	std::string villageSlug = world.findVillage ( villageId )->getUrlSlug ();
	std::string requestURL = world.getLink() + "/game.php?" + villageSlug + "&screen=scavenge_api&ajaxaction=send_squads";
//	std::cout << requestURL << std::endl;
	std::string postString = "squad_requests[0][village_id]=" + std::to_string (villageId);
	postString += "&squad_requests[0][candidate_squad][carry_max]=" + std::to_string (carryMax);
	for ( std::pair<std::string, int> unit : units ) {
		postString += "&squad_requests[0][candidate_squad][unit_counts][" + unit.first + "]=" + std::to_string (unit.second);
	}
	postString += "&squad_requests[0][option_id]=" + std::to_string ( option );
	postString += "&squad_requests[0][use_premium]=false";
	postString += "&h=" + this->csrf;
//	std::cout << postString << std::endl;
	Client client;
	std::string postScavenge = client.postUrl ( requestURL, postString, this->cookiesFile );
	std::fstream scavengeFile;
	scavengeFile.open ( "scavengingOrders.html", std::ios::out );
	scavengeFile << postScavenge;
	scavengeFile.close ();
	return postScavenge;
}


std::string Account::upgradeBuilding ( World &world, int villageId, std::string buildingName ) {
	std::string villageSlug = world.findVillage ( villageId )->getUrlSlug ();
	std::string requestURL = world.getLink() + "/game.php?" + villageSlug
	+ "&screen=main&ajaxaction=upgrade_building&id=" + buildingName + "&type=main&h=" + this->csrf;
//	std::cout << requestURL << std::endl;
	Client client;
//	client.headersList.push_back ( "TribalWars-Ajax: 1" );
	std::string postUpgrade = client.getUrl ( requestURL, this->cookiesFile );
	std::fstream upgradeFile;
	upgradeFile.open ( "upgrade.html", std::ios::out );
	upgradeFile << postUpgrade;
	upgradeFile.close ();
	std::regex regError ( "error_box" );
	std::smatch matchError;
	if ( std::regex_search ( postUpgrade, matchError, regError ) ) {
		std::cout << "Found error_box in response. There was some sort of error when upgrading building." << std::endl;
		throw "YEET";
	}
	return postUpgrade;
}

//Make it so I dont duplicate 99% of code;
std::string Account::getPlayerInfo ( World &world, int villageId ) {
	std::string villageSlug = world.findVillage ( villageId )->getUrlSlug ();
	std::string requestURL = world.getLink() + "/game.php?" + villageSlug + "screen=unit_info&ajax=data";
	std::cout << requestURL << std::endl;
	try {
		curlpp::Cleanup myCleanup;
		curlpp::Easy myRequest;
		myRequest.setOpt<curlpp::options::Url> ( requestURL );
		myRequest.setOpt ( new curlpp::options::Verbose(true) );
//		myRequest.setOpt ( new curlpp::options::CookieJar ( this->cookiesFile ) );
		myRequest.setOpt ( new curlpp::options::CookieFile ( this->cookiesFile ) );
		std::list<std::string> header;
//		header.push_back ("Content-Type: application/x-www-form-urlencoded");
		header.push_back ("Accept: application/json, text/javascript, */*; q=0.01");
		header.push_back ("User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0");
		header.push_back ("X-Requested-With: XMLHttpRequest");
		header.push_back ("TribalWars-Ajax: 1");
		myRequest.setOpt(new curlpp::options::HttpHeader(header));
		std::stringstream ss;
		myRequest.setOpt ( new curlpp::options::WriteStream ( &ss ) );
		myRequest.perform ();
		std::string resultJson = ss.str();
		bsoncxx::document::value doc = bsoncxx::from_json ( resultJson );
		resultJson = bsoncxx::to_json ( doc.view ()["game_data"]["player"].get_document().view () );
		return resultJson;
	}
	catch(curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
		return "false";
	}

	catch(curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
		return "false";
	}
}
std::string Account::getVillageInfo ( World &world, int villageId ) {
	std::string villageSlug = world.findVillage ( villageId )->getUrlSlug ();
	std::string requestURL = world.getLink() + "/game.php?" + villageSlug + "screen=unit_info&ajax=data";
	std::cout << requestURL << std::endl;
	try {
		curlpp::Cleanup myCleanup;
		curlpp::Easy myRequest;
		myRequest.setOpt<curlpp::options::Url> ( requestURL );
		myRequest.setOpt ( new curlpp::options::Verbose(true) );
//		myRequest.setOpt ( new curlpp::options::CookieJar ( this->cookiesFile ) );
		myRequest.setOpt ( new curlpp::options::CookieFile ( this->cookiesFile ) );
		std::list<std::string> header;
//		header.push_back ("Content-Type: application/x-www-form-urlencoded");
		header.push_back ("Accept: application/json, text/javascript, */*; q=0.01");
		header.push_back ("User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0");
		header.push_back ("X-Requested-With: XMLHttpRequest");
		header.push_back ("TribalWars-Ajax: 1");
		myRequest.setOpt(new curlpp::options::HttpHeader(header));
		std::stringstream ss;
		myRequest.setOpt ( new curlpp::options::WriteStream ( &ss ) );
		myRequest.perform ();
		std::string resultJson = ss.str();
		bsoncxx::document::value doc = bsoncxx::from_json ( resultJson );
		resultJson = bsoncxx::to_json ( doc.view ()["game_data"]["village"].get_document().view () );
		return resultJson;
	}
	catch(curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
		return "false";
	}

	catch(curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
		return "false";
	}
}


