#include "headquarters.h"

#include <misc/utils.h>

#include <game/village.h>
#include <game/player.h>
#include <game/account.h>
#include <game/worldsettings.h>

#include <game/game.h>

Headquarters::Headquarters (std::string xmlData, WorldSettings *worldSettings) : Building (xmlData, worldSettings) {

}

std::string Headquarters::getUrlSlug () {
	return "screen=main";
}

std::unique_ptr<Building> Headquarters::clone() {
	return std::make_unique<Headquarters> ( *this );
}

Headquarters &Headquarters::parseData ( std::string htmlData ) {
	xmlDoc *doc = htmlReadDoc ( (xmlChar*)htmlData.c_str (), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING );
	xmlNode *r = xmlDocGetRootElement ( doc );
	xmlpp::Element* root = new xmlpp::Element(r);
	Glib::ustring xpath = "//tbody[@id=\"buildqueue\"]/tr[contains(@class,\"buildorder\")]";
	xmlpp::Node::NodeSet rows = root->find ( xpath );
	for ( size_t i = 0; i < rows.size (); i++ ) {
		xmlpp::Node::NodeList cells = rows[i]->get_children ( Glib::ustring ( "td" ) );
		std::string buildingName = dynamic_cast<xmlpp::Element*> (rows[i])->get_attribute_value ( "class" );
		std::smatch match;
		std::regex_search ( buildingName, match, std::regex ( "buildorder_(.*)" ) );
		buildingName = match.str(1);
		// Cell with date when building is complete.
		xmlpp::Node *dueTimeCell = *( ++++++cells.begin() ); //Im going to hell for this
		std::string dueDate = dynamic_cast<xmlpp::Element*> ( dueTimeCell )->get_first_child_text ()->get_content ();
		long int timestamp = 0;
		// Now converting this stupid date format into timestamp
		timestamp = Utils::decodeBuildDate ( dueDate );
		//Cell with cancel link and id of construction order. Also contains id of village.
		xmlpp::Node *cancelLinkCell = *( ++++++++cells.begin() ); //Living the meme #advancedc++++++++programming
		xmlpp::Element *aTag = dynamic_cast<xmlpp::Element*> ( cancelLinkCell->get_first_child ( "a" ) );
		std::string cancelLink = aTag->get_attribute_value ( "href" );
		std::smatch matchId;
		int orderId;
		if ( std::regex_search ( cancelLink, matchId, std::regex ( "id=(\\d+)" ) ) ) {
			orderId = std::stoi ( matchId.str (1) );
		}
		else {
			std::cout << "Couldnt extract order id from cancel link. Something is super wrong" << std::endl;
			throw "Yeet";
		}
		//Now create BuildOrder
		std::unique_ptr<BuildOrder> order ( new BuildOrder ( this->parentVillage->getBuilding (buildingName), *this ) );
		order->dueTime = timestamp;
		order->id = orderId;
		order->text = "Building " + buildingName;
		this->addOrder ( std::move(order) );
	}
	return *this;
}

std::vector<Building*> Headquarters::getConstructedBuildings() {
	std::vector<Building*> result;
	std::vector<Order*> orders = this->getOrders (*this);
	for ( Order *order : orders ) {
		result.push_back ( &static_cast<BuildOrder*>(order)->target );
	}
	result.shrink_to_fit ();
	return result;
}

std::string Headquarters::toString() {
	std::string result = Building::toString ();
	return result;
}

bool Headquarters::queueFull() {
	size_t maxQueueSize = 2; // Default maximum queue size
	if ( this->getOrders (*this).size() >= maxQueueSize ) {
		return true;
	}
	return false;
}

bool Headquarters::upgradeBuilding ( std::string buildingName ) {
	//Check if building queue is full
	if ( this->queueFull () ) {
		Game::logCallback ( "Building queue is full.\n" );
		return false;
	}
	//Checking if building can be upgraded.
	if ( !this->parentVillage->getBuilding ( buildingName ).canUpgrade () ) {
		Game::logCallback ( "Cannot upgrade building: " + buildingName + '\n' );
		return false;
	}
	//Get upgrade cost. Need to do this BEFORE submitting build order
	Resources upgradeCost = this->parentVillage->getBuilding ( buildingName ).getUpgradeCost ();
	//First submit upgrade order on game server
	std::string responseJson = this->parentVillage->owner->ownerAccount
	->upgradeBuilding ( *this->worldSettings->world, this->parentVillage->id, buildingName );

	bsoncxx::document::value doc = bsoncxx::from_json ( responseJson );
	int secondsComplete = doc.view ()["date_complete"].get_int32 ().value; // completion time in seconds
	std::string buildingOrders ( doc.view ()["building_orders"].get_utf8 ().value );

	// Finding id of created order. It will be the last one found
	std::regex regId ( "cancel\\((\\d+)" );
	std::smatch matchId;
	std::string::const_iterator searchStart( buildingOrders.cbegin() );
	std::string newOrderId;
	while ( regex_search ( searchStart, buildingOrders.cend(), matchId, regId ) ) {
		newOrderId = matchId.str (1);
		searchStart = matchId.suffix().first;
	}
	std::unique_ptr<BuildOrder> order ( new BuildOrder ( this->parentVillage->getBuilding (buildingName), *this ) );
	order->dueTime = Utils::getTimestamp () + secondsComplete;
	order->id = std::stoi ( newOrderId );
	order->text = "Building " + buildingName;
	order->cost = upgradeCost;
	this->addOrder ( std::move ( order ) );
	//Finally if everything went well deduce resources from parent village storage
	Storage &store = static_cast<Storage&> ( this->parentVillage->getBuilding ("storage") );
	store.substractResources ( upgradeCost );
	return true;
}
