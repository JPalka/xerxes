#include "headquarters.h"

#include <misc/utils.h>

#include <game/village.h>
#include <game/player.h>
#include <game/account.h>
#include <game/worldsettings.h>

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
	std::vector<std::unique_ptr<BuildOrder>> buildQueueParsed;
	for ( size_t i = 0; i < rows.size (); i++ ) {
		xmlpp::Node::NodeList cells = rows[i]->get_children ( Glib::ustring ( "td" ) );
		std::string buildingName = dynamic_cast<xmlpp::Element*> (rows[i])->get_attribute_value ( "class" );
		std::regex reg ( "buildorder_.*" );
		std::regex remove ( "buildorder_" );
		std::smatch match;
		std::regex_search ( buildingName, match, reg );
		buildingName = std::regex_replace ( match[0].str(), remove, "" );
//		std::cout << "name: " << buildingName << std::endl;
		//Cell with name and level of constructed building. Extracting only level of building being constructed
		xmlpp::Node *nameCell = *cells.begin();
		xmlpp::Node::NodeList nameCellChildren = nameCell->get_children ( Glib::ustring ( "text" ) );
		int levelConstructed = 0;
		for ( auto child: nameCellChildren ) {
			Glib::ustring temp ( dynamic_cast<xmlpp::TextNode*> (child)->get_content() );
			temp = g_ascii_strdown ( g_strstrip ( (gchar*)temp.data () ), -1 );
			if ( temp.length () > 0 ) {
				std::regex regLevel ( "level \\d*" );
				std::regex removeLevel ( "level " );
				std::smatch matchLevel;
				std::string temp2 ( temp );
				if ( std::regex_search ( temp2, matchLevel, regLevel ) ) {
					std::string levelStr ( std::regex_replace ( matchLevel[0].str(), removeLevel, "" ) );
					levelConstructed = std::stoi ( levelStr );
					break;
				}
			}
		}
//		std::cout << "level: " << levelConstructed << std::endl;
		// Cell with date when building is complete.
		xmlpp::Node *dueTimeCell = *( ++++++cells.begin() ); //Im going to hell for this
//		std::cout << dynamic_cast<xmlpp::Element*> ( dueTimeCell )->get_first_child_text ()->get_content () << "\n";
		std::string dueDate = dynamic_cast<xmlpp::Element*> ( dueTimeCell )->get_first_child_text ()->get_content ();
		long int timestamp = 0;
		// Now converting this stupid date format into timestamp
		timestamp = Utils::decodeBuildDate ( dueDate );
//		std::cout << "Due date: " << timestamp << std::endl;
		//Cell with cancel link and id of construction order. Also contains id of village.
		xmlpp::Node *cancelLinkCell = *( ++++++++cells.begin() ); //Living the meme #advancedc++++++++programming
		xmlpp::Element *aTag = dynamic_cast<xmlpp::Element*> ( cancelLinkCell->get_first_child ( "a" ) );
		std::string cancelLink = aTag->get_attribute_value ( "href" );
		std::regex regId ( "id=\\d+" );
		std::regex removeId ( "id=" );
		std::smatch matchId;
		int orderId;
		if ( std::regex_search ( cancelLink, matchId, regId ) ) {
			orderId = std::stoi ( std::regex_replace ( matchId[0].str(), removeId, "" ) );
		}
		else {
			std::cout << "Couldnt extract order id from cancel link. Something is super wrong" << std::endl;
			throw "Yeet";
		}
//		std::regex regVillage ( "village=\\d+" );
//		std::regex removeVillage ( "village=" );
//		std::smatch matchVillage;
//		int villageId;
//		if ( std::regex_search ( cancelLink, matchVillage, regVillage ) ) {
//			villageId = std::stoi ( std::regex_replace ( matchVillage[0].str(), removeVillage, "") );
//		}
//		else {
//			std::cout << "Village id not found in cancel link. Throwing a fit" << std::endl;
//			throw "fit";
//		}
//		std::cout << "Order id: " << orderId << std::endl;
//		std::cout << "Village id: " << villageId << std::endl;

		//Now create BuildOrder
		std::unique_ptr<BuildOrder> order ( new BuildOrder ( this->parentVillage->getBuilding (buildingName), *this ) );
		order->newLevel = levelConstructed;
		order->dueTime = timestamp;
		order->id = orderId;
		order->cancelSlug = cancelLink;
		order->text = "Building " + buildingName;
		buildQueueParsed.push_back ( std::move(order) );
	}

	if ( this->addOrder == nullptr ) {
		std::cout << "addorder is nullptr gtfo." << std::endl;
		throw "kek";
	}
	for ( auto &bo : buildQueueParsed ) {
		this->addOrder ( std::move ( bo ) );
	}
	return *this;
}

//std::string Headquarters::toString() {
//	std::string result = Building::toString ();
//	std::vector<Order*> orders = this->getOrders ( *this );
//	for ( auto it = orders.rbegin (); it != orders.rend (); it++ ) {
//		result += (*it)->text + " - " + Utils::timestampToString ( (*it)->dueTime ) + "\n";
//	}
//	return result;
//}

bool Headquarters::upgradeBuilding ( std::string buildingName ) {
	//TODO: check if building can be upgraded at all

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
//	std::cout << newOrderId << std::endl;
	std::unique_ptr<BuildOrder> order ( new BuildOrder ( this->parentVillage->getBuilding (buildingName), *this ) );
	order->newLevel = ++this->parentVillage->getBuilding (buildingName).level;
	order->dueTime = Utils::getTimestamp () + secondsComplete;
	order->id = std::stoi ( newOrderId );
	order->cancelSlug = "whatever. need to delete this field";
	order->text = "Building " + buildingName;
	this->addOrder ( std::move ( order ) );
	return true;
}
