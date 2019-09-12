#include <fstream>
#include <memory>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/view_or_value.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/exception/exception.hpp>

#include <misc/coordinate.h>
#include <misc/utils.h>
#include <game/game.h>

#include <buildings/buildingfactory.h> //Temp

#include <xerxes/xerxes.h>

using namespace curlpp::options;

void testCoordinate () {
	Coordinate coordFromInt ( 637, 623 );
	Coordinate coordFromStr ( "636|626" );
	std::cout << coordFromInt.toString () << "///" << coordFromStr.toString () << std::endl;
	std::cout << "Distance between points: " << coordFromInt.getDistance ( coordFromStr ) << std::endl;
}

int main () {
	Xerxes xerxes;
	xerxes.setCredentials ( std::make_pair ( "Korenchkin", "rickenbacker1" ) );
	xerxes.start ();
//	std::cout << "Creating game instance\n";
//	Game gameInstance;
//	WorldList list ( "https://tribalwars.net" );
//	std::map<std::string, std::string>::iterator iter = list.worldLinks.find ( "en109" );
//	if ( iter == list.worldLinks.end () ) {
//		std::cout << "Cannot find world: en109" << std::endl;
//		throw "gogo";
//	}
//	gameInstance.initialize ( "en109" );
////	std::cout << "DONE AND DONE\n";
//	gameInstance.setAccount ( "Korenchkin", "rickenbacker1" );

//	std::vector<std::pair<std::string,int>> fellowship;
//	fellowship.push_back ( std::make_pair ( "spear", 300 ) );
//	Place &rallyPoint = static_cast<Place&> ( gameInstance.currentWorld->findVillage ( 16530 )->getBuilding ( "place" ) );
//	rallyPoint.scavengingModule.sendScavengeParty ( fellowship, 1 );
//	rallyPoint.scavengingModule.sendScavengeParty ( fellowship, 2 );
//	rallyPoint.scavengingModule.sendScavengeParty ( fellowship, 3 );
//	rallyPoint.scavengingModule.sendScavengeParty ( fellowship, 4 );
//	std::cout << gameInstance.currentWorld->findVillage ( 16530 )->toString ()<< std::endl;


//	Headquarters &hq = static_cast<Headquarters&> ( gameInstance.currentWorld->findVillage ( 16530 )->getBuilding ( "main" ) );
//	hq.upgradeBuilding ( "barracks" );
//	sleep ( 5 );
//	hq.upgradeBuilding ( "wood" );
	//test upgradeBuilding in account
//	std::cout << gameInstance.activeAccount.upgradeBuilding ( *gameInstance.currentWorld, 16530, "main" ) << std::endl;
//	std::cout << gameInstance.activeAccount.upgradeBuilding ( *gameInstance.currentWorld, 16530, "barracks" ) << std::endl;
//	static_cast<Barracks&> ( gameInstance.currentWorld->findVillage ( 16530 )->getBuilding ("barracks") ).recruitUnit ( "spear", 5 );
//	while ( true ) {
//		sleep ( 20 );
//		gameInstance.currentWorld->refresh ();
//		std::cout << "Current time: " << Utils::timestampToString (Utils::getTimestamp ()) << std::endl;
//		std::cout << "Orders queue count: " << gameInstance.currentWorld->ordersQueue.size () << std::endl;
//	}


	//Listing AND REMOVING order queue in current world
//	while ( !gameInstance.currentWorld->ordersQueue.empty () ) {
//		std::time_t time = gameInstance.currentWorld->ordersQueue.top ()->dueTime ;
//		std::cout << std::ctime ( &time ) << " - " << gameInstance.currentWorld->ordersQueue.top ()->text << "\n";
//		gameInstance.currentWorld->ordersQueue.pop ();
//	}
//	gameInstance.activeAccount.recruitUnit ( *gameInstance.currentWorld, 16530, std::string("spear"), 1 );

//	Account botAcc ( "Korenchkin", "rickenbacker1" );
//	botAcc.login ( iter->second, gameInstance.currentWorld->id );
////	std::string buildingInfo = botAcc.getBuildingInfo ( gameInstance.currentWorld, 16530, "main" );
////	std::ofstream x ( "buildingInfo.html" );
////	x << buildingInfo;
////	x.close ();
////	std::cout << botAcc.getVillageInfo ( gameInstance.currentWorld, 42422 ) << std::endl;
////	std::cout << botAcc.getPlayerInfo ( gameInstance.currentWorld, 42422 ) << std::endl;
////	std::cout << doc.view ()["game_data"]["village"]["wood_float"].get_double() << std::endl;

//	std::string buildingInfo = botAcc.getBuildingInfo ( *gameInstance.currentWorld, 16530, "main" );
//	std::string buildingInfoJson = Utils::extractBuildingMainJson ( buildingInfo );

//	//Gets GameData json from html. Passing buildingInfo because GameData can be extracted from (almost?) any page accessed while logged in
//	std::string gameDataJson = Utils::extractGameDataJson ( buildingInfo );

////	gameInstance.currentWorld->findVillage ( 16530 )->csrfToken = csrfToken.get_utf8 ().value;

//	gameInstance.currentWorld->findVillage ( 16530 )->updateBuildingsLevels ( buildingInfoJson );
//	std::cout << "Updated building levels.\n";
//	(*gameInstance.currentWorld->findVillage ( 16530 ))["main"].parseData ( buildingInfo );
//	//Storage can parse its info from pretty much every page because its info is in top bar.
//	(*gameInstance.currentWorld->findVillage ( 16530 ))["storage"].parseData ( buildingInfo );
//	//Same goes for farm
//	(*gameInstance.currentWorld->findVillage ( 16530 ))["farm"].parseData ( buildingInfo );
//	Wood &wood = static_cast<Wood&>( (*gameInstance.currentWorld->findVillage ( 16530 ))["wood"] );
//	Stone &stone = static_cast<Stone&>( (*gameInstance.currentWorld->findVillage ( 16530 ))["stone"] );
//	Iron &iron = static_cast<Iron&>( (*gameInstance.currentWorld->findVillage ( 16530 ))["iron"] );
//	std::cout << "wood production: ";
//	std::cout << wood.getProduction () << std::endl;
//	std::cout << "stone production: ";
//	std::cout << stone.getProduction () << std::endl;
//	std::cout << "iron production: ";
//	std::cout << iron.getProduction () << std::endl;
//	Storage &storage = static_cast<Storage&>( (*gameInstance.currentWorld->findVillage ( 16530 ))["storage"] );
//	std::cout << "Current storage :\n";
//	std::cout << "wood: " << storage.getStorage ().wood << "/" << storage.getCapacity ().wood << std::endl;
//	std::cout << "stone: " << storage.getStorage ().stone << "/" << storage.getCapacity ().stone << std::endl;
//	std::cout << "iron: " << storage.getStorage ().iron << "/" << storage.getCapacity ().iron << std::endl;

//	std::string placeInfo = botAcc.getBuildingInfo ( *gameInstance.currentWorld, 16530, "place" );
//	(*gameInstance.currentWorld->findVillage (16530))["place"].parseData ( placeInfo );

//	Place &rallyPoint = static_cast<Place&> ( (*gameInstance.currentWorld->findVillage ( 16530 ))["place"] );
//	std::cout << "unitTypeCount: " << rallyPoint.units.size () << std::endl;
//	for ( auto &x : rallyPoint.units ) {
//		std::cout << x->name << ": " << x->unitCount << std::endl;
//	}

//	std::string barracksInfo = botAcc.getBuildingInfo ( *gameInstance.currentWorld, 16530, "barracks" );

//	std::ofstream x ( "barracksInfo.html" );
//	x << barracksInfo;
//	x.close ();
//	(*gameInstance.currentWorld->findVillage (16530))["barracks"].parseData ( barracksInfo );

//	Farm &farm = static_cast<Farm&> ( (*gameInstance.currentWorld->findVillage (16530))["farm"] );
//	std::cout << "Farm capacity: ";
//	std::cout << farm.getCurrentPopulation () << "/" << farm.getMaxPopulation () << std::endl;

//	std::cout << (*gameInstance.currentWorld->findVillage (16530)).toString () << std::endl;

	//Listing AND REMOVING order queue in current world
//	while ( !gameInstance.currentWorld->ordersQueue.empty () ) {
//		std::time_t time = gameInstance.currentWorld->ordersQueue.top ()->dueTime ;
//		std::cout << std::ctime ( &time ) << " - " << gameInstance.currentWorld->ordersQueue.top ()->text << "\n";
//		gameInstance.currentWorld->ordersQueue.pop ();
//	}
//	botAcc.recruitUnit ( *gameInstance.currentWorld, 16530, std::string("spear"), 2 );
//	while ( true ) {
//		sleep ( 10 );
//		std::cout << "Current time: ";
//		std::cout << Utils::timestampToString ( Utils::getTimestamp () );
//		gameInstance.currentWorld->refresh ();
//		Storage &storage = static_cast<Storage&>( (*gameInstance.currentWorld->findVillage ( 16530 ))["storage"] );
//		std::cout << "Current storage :\n";
//		std::cout << "wood: " << std::round(storage.getStorage ().wood) << "/" << storage.getCapacity ().wood << std::endl;
//		std::cout << "stone: " << std::round(storage.getStorage ().stone) << "/" << storage.getCapacity ().stone << std::endl;
//		std::cout << "iron: " << std::round(storage.getStorage ().iron) << "/" << storage.getCapacity ().iron << std::endl;
//		for ( auto &x : rallyPoint.units ) {
//			std::cout << x->name << ": " << x->unitCount << std::endl;
//		}
//		std::cout << "Wall level: " << gameInstance.currentWorld->findVillage ( 16530 )->getBuilding ( "wall" ).level << std::endl;
//	}
//	std::cout << "building debug info: \n";
//	for ( auto &x : gameInstance.currentWorld.findVillage ( 16530 )->buildings ) {
//		std::cout << x->name << " level: " << x->level << std::endl;
//	}
	return 0;
}
