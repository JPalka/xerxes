#include "barracks.h"
#include <misc/utils.h>
#include <game/village.h>
#include <orders/recruitorder.h>
#include <buildings/place.h>
#include <game/player.h>
#include <game/account.h>
#include <game/worldsettings.h>

Barracks::Barracks (std::string xmlData , WorldSettings *worldSettings) : Building(xmlData, worldSettings) {

}

std::string Barracks::getUrlSlug () {
	return "&screen=barracks";
}

//std::string Barracks::toString() {
//	std::string result = Building::toString ();
//	std::vector<Order*> orders = this->getOrders ( *this );
//	for ( auto it = orders.rbegin (); it != orders.rend (); it++ ) {
//		result += (*it)->text + " - " + Utils::timestampToString ( (*it)->dueTime ) + "\n";
//	}
//	return result;
//}

Barracks &Barracks::parseData ( std::string htmlData ) {
	xmlDoc *doc = htmlReadDoc ( (xmlChar*)htmlData.c_str (), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING );
	xmlNode *r = xmlDocGetRootElement ( doc );
	xmlpp::Element* root = new xmlpp::Element(r);
	Glib::ustring xpath = "//div[@id=\"trainqueue_wrap_barracks\"]/table";
	xmlpp::Node::NodeSet queueTable = root->find ( xpath );
	if ( queueTable.empty () ) {
//		std::cout << "No recruit orders to read" << std::endl;
		return *this;
	}
	//Read first trs that contains currently processed recruitment order
	xmlpp::Node::NodeList rows = queueTable[0]->get_children ( "tr" );
	for ( auto it = ++rows.begin (); it != rows.end (); it++ ) { //Skipping headers row
		xmlpp::Node::NodeList cells = (*it)->get_children ( "td" );
		auto currentCellIterator = cells.begin ();
		// Get what is being built
		xmlpp::Node *unitDiv = (*currentCellIterator)->get_first_child ( "div" );
		std::string unitTypeDivClass = dynamic_cast<xmlpp::Element*> ( unitDiv )->get_attribute_value ( "class" );
		//std::regex regUnitType ( "(\w+)" );
		//Regex that takes unit name (last word) from string like "unit_sprite unit_sprite_smaller spear"
		std::regex regUnitType ( "(\\w+)$" );
		std::smatch matchUnitType;
		std::string unitType;
		if ( std::regex_search ( unitTypeDivClass, matchUnitType, regUnitType) ) {
			unitType = matchUnitType.str ();
//			std::cout << "constructed unit type: " << matchUnitType.str() << std::endl;
		}
		else {
			std::cout << "Cannot match unit type from div id. Olala" << std::endl;
			throw std::string ("Oof");
		}
		//Get number of dudes being built
		//Get all text nodes of first table row cell...kill me.
		xmlpp::Node::NodeList textNodes = dynamic_cast<xmlpp::Element*> ( *currentCellIterator )->get_children ( "text" );
		std::string textNumber = dynamic_cast<xmlpp::TextNode*>(*(textNodes.rbegin ()))->get_content ();
//		std::cout << textNumber << std::endl;
		std::regex regUnitCount ( "(\\d+)" );
		std::smatch matchUnitCount;
		int constructedUnitCount = 0;
		if ( std::regex_search ( textNumber, matchUnitCount, regUnitCount ) ) {
			constructedUnitCount = std::stoi ( matchUnitCount.str() );
//			std::cout << "constructed unit count: " << constructedUnitCount << std::endl;
		}
		else {
			std::cout << "Cannot match unit count." << std::endl;
			throw std::string ( "Ojej" );
		}
		currentCellIterator++; //skip duration (3rd) cell
		//Now get completion date
		currentCellIterator++;
//		std::cout << dynamic_cast<xmlpp::Element*> ( *currentCellIterator )->get_first_child_text ()->get_content () << "\n";
		std::string dueDate = dynamic_cast<xmlpp::Element*> ( *currentCellIterator )->get_first_child_text ()->get_content ();
		long int timestamp = 0;
		// Now converting this stupid date format into timestamp
		timestamp = Utils::decodeBuildDate ( dueDate );
//		std::cout << "Due date: " << timestamp << std::endl;
		//Get cancellation link slug and extract info from it
		currentCellIterator++;
		xmlpp::Element *aTag = dynamic_cast<xmlpp::Element*> ( (*currentCellIterator)->get_first_child ( "a" ) );
		std::string cancelLink = aTag->get_attribute_value ( "href" );
		std::regex regId ( "id=\\d+" );
		std::regex removeId ( "id=" );
		std::smatch matchId;
		int orderId;
		if ( std::regex_search ( cancelLink, matchId, regId ) ) {
			orderId = std::stoi ( std::regex_replace ( matchId[0].str(), removeId, "" ) );
		}
		else {
			std::cout << "Couldnt extract recruit order id from cancel link. Something is super wrong" << std::endl;
			throw "Yeet";
		}
//		std::cout << "order id: " << orderId << std::endl;

		//Create single unit orders from parsed data;
		Place &placeRef = static_cast<Place&> ( this->parentVillage->operator [] ("place") );
		for ( int i = 0; i < constructedUnitCount; i++ ) {
			std::unique_ptr<RecruitOrder> order ( new RecruitOrder ( *placeRef.getUnit ( unitType ), *this ) );
			order->dueTime = timestamp - i * this->getRecruitTime ( unitType );
			order->id = orderId;
			order->cancelSlug = cancelLink;
			order->text = "Recruiting " + unitType;
			this->addOrder ( std::move(order) );
		}
	}

	//Now get rest of recruitment queue
	xpath = "//div[@id=\"trainqueue_wrap_barracks\"]/table/tbody[@id=\"trainqueue_barracks\"]";
	xmlpp::Node::NodeSet queueTbody = root->find ( xpath );
	if ( queueTbody.empty () ) {
		std::cout << "No further recruit orders to read" << std::endl;
		return *this;
	}
	xmlpp::Node::NodeList tbodyRows = queueTbody[0]->get_children ( "tr" );
	//TODO:: GET RID OF CODE DUPLICATION
	for ( auto it = tbodyRows.begin (); it != tbodyRows.end (); it++ ) {
		xmlpp::Node::NodeList cells = (*it)->get_children ( "td" );
		auto currentCellIterator = cells.begin ();
		// Get what is being built
		xmlpp::Node *unitDiv = (*currentCellIterator)->get_first_child ( "div" );
		if ( unitDiv == nullptr ) {
			//Row doesnt contain building order. skip it
			continue;
		}
		std::string unitTypeDivClass = dynamic_cast<xmlpp::Element*> ( unitDiv )->get_attribute_value ( "class" );
		//std::regex regUnitType ( "(\w+)" );
		//Regex that takes unit name (last word) from string like "unit_sprite unit_sprite_smaller spear"
		std::regex regUnitType ( "(\\w+)$" );
		std::smatch matchUnitType;
		std::string unitType;
		if ( std::regex_search ( unitTypeDivClass, matchUnitType, regUnitType) ) {
			unitType = matchUnitType.str ();
//			std::cout << "constructed unit type: " << matchUnitType.str() << std::endl;
		}
		else {
			std::cout << "Cannot match unit type from div id. Olala" << std::endl;
			throw std::string ("Oof");
		}
		//Get number of dudes being built
		//Get all text nodes of first table row cell...kill me.
		xmlpp::Node::NodeList textNodes = dynamic_cast<xmlpp::Element*> ( *currentCellIterator )->get_children ( "text" );
		std::string textNumber = dynamic_cast<xmlpp::TextNode*>(*(textNodes.rbegin ()))->get_content ();
//		std::cout << textNumber << std::endl;
		std::regex regUnitCount ( "(\\d+)" );
		std::smatch matchUnitCount;
		int constructedUnitCount = 0;
		if ( std::regex_search ( textNumber, matchUnitCount, regUnitCount ) ) {
			constructedUnitCount = std::stoi ( matchUnitCount.str() );
//			std::cout << "constructed unit count: " << constructedUnitCount << std::endl;
		}
		else {
			std::cout << "Cannot match unit count." << std::endl;
			throw std::string ( "Ojej" );
		}
		currentCellIterator++; //skip duration (3rd) cell
		//Now get completion date
		currentCellIterator++;
//		std::cout << dynamic_cast<xmlpp::Element*> ( *currentCellIterator )->get_first_child_text ()->get_content () << "\n";
		std::string dueDate = dynamic_cast<xmlpp::Element*> ( *currentCellIterator )->get_first_child_text ()->get_content ();
		long int timestamp = 0;
		// Now converting this stupid date format into timestamp
		timestamp = Utils::decodeBuildDate ( dueDate );
//		std::cout << "Due date: " << timestamp << std::endl;
		//Get cancellation link slug and extract info from it
		currentCellIterator++;
		xmlpp::Element *aTag = dynamic_cast<xmlpp::Element*> ( (*currentCellIterator)->get_first_child ( "a" ) );
		std::string cancelLink = aTag->get_attribute_value ( "href" );
		std::regex regId ( "id=\\d+" );
		std::regex removeId ( "id=" );
		std::smatch matchId;
		int orderId;
		if ( std::regex_search ( cancelLink, matchId, regId ) ) {
			orderId = std::stoi ( std::regex_replace ( matchId[0].str(), removeId, "" ) );
		}
		else {
			std::cout << "Couldnt extract recruit order id from cancel link. Something is super wrong" << std::endl;
			throw "Yeet";
		}
//		std::cout << "order id: " << orderId << std::endl;

		//Create single unit orders from parsed data;
		Place &placeRef = static_cast<Place&> ( this->parentVillage->operator [] ("place") );
		for ( int i = 0; i < constructedUnitCount; i++ ) {
			std::unique_ptr<RecruitOrder> order ( new RecruitOrder ( *placeRef.getUnit ( unitType ), *this ) );
			order->dueTime = timestamp - i * this->getRecruitTime ( unitType );
			order->id = orderId;
			order->cancelSlug = cancelLink;
			order->text = "Recruiting " + unitType;
			this->addOrder ( std::move(order) );
		}
	}
	return *this;
}

std::unique_ptr<Building> Barracks::clone() {
	return std::make_unique<Barracks> ( *this );
}

int Barracks::getRecruitTime ( std::string unitName ) {
	Place &placeRef = static_cast<Place&> ( this->parentVillage->operator [] ("place") );
	int baseBuildTime = placeRef.getUnit ( unitName )->buildTime;
	int finalBuildTime = std::round ( 2.0/3.0 * baseBuildTime * std::pow ( 1.06, this->level * -1 ) );
	return finalBuildTime;
}

bool Barracks::recruitUnit ( std::string unitName, int count ) {
	//TODO: check if unit can be recruited at all

	//First submit recruitment order on game server
	int orderId = this->parentVillage->owner->ownerAccount->recruitUnit ( *this->worldSettings->world, this->parentVillage->id, unitName, count );

	//Now create orders locally;
	Place &placeRef = static_cast<Place&> ( this->parentVillage->operator [] ("place") );
	for ( int i = 0; i < count; i++ ) {
		std::unique_ptr<RecruitOrder> order ( new RecruitOrder ( *placeRef.getUnit ( unitName ), *this ) );
		order->dueTime = Utils::getTimestamp () + (i+1) * this->getRecruitTime ( unitName );
		order->id = orderId;
		order->cancelSlug = "Whatever Im not using it anyway";
		order->text = "Recruiting " + unitName;
		this->addOrder ( std::move(order) );
	}
	return true;
}
