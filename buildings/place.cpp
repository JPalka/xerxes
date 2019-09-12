#include "place.h"

#include <game/worldsettings.h>
#include <game/account.h>
#include <game/player.h>
#include <game/game.h>

#include <units/unitfactory.h>
#include <misc/utils.h>

#include <libxml++/libxml++.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/HTMLparser.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/view_or_value.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/exception/exception.hpp>

#include <orders/scavengeorder.h>
#include <exceptions/insufficentunits.h>

Place::Place (std::string xmlData, WorldSettings *worldSettings) : Building (xmlData, worldSettings), scavengingModule(this) {
	//Initialize unit vector
	xmlpp::DomParser parser;
	parser.parse_memory ( worldSettings->unitInfoXmlString );
	xmlpp::Document *doc = parser.get_document ();
	xmlpp::Node::NodeList newRoot = doc->get_root_node ()->get_children ();
	for ( xmlpp::Node *unitNode: newRoot ) {
		xmlpp::Document newDocument;
		newDocument.create_root_node_by_import ( unitNode );
		std::string unitXmlData = newDocument.write_to_string ();
		if ( unitNode->get_name () == "text" ) { //Skip text nodes
			continue;
		}
//		std::cout << unitNode->get_name () << " ";
		std::unique_ptr<Unit> tempUnit = UnitFactory::createUnit ( unitNode->get_name(), worldSettings->unitInfoXmlString, worldSettings );
		this->units.push_back ( std::move ( tempUnit ) );
	}
//	std::cout << std::endl;
}

std::string Place::getUrlSlug () {
	return "&screen=place";
}
std::string Place::toString () {
	std::string result = Building::toString ();
	//Unit counts
	int count = 0;
	for ( std::unique_ptr<Unit> &unit : this->units ) {
		result += unit->name + " - " + std::to_string (unit->unitCount);
		result += (count % 2 == 0 ) ? " \t" : "\n";
		count++;
	}
	return result;
}
Place::Place ( const Place &source ) : Building ( source ), scavengingModule(source.scavengingModule) {
	//Deep copy unit vector
	for ( auto it = source.units.begin (); it != source.units.end (); it++ ) {
		std::unique_ptr<Unit> unitTemp ( (*it)->clone() );
		this->units.push_back ( std::move ( unitTemp ) );
	}

	this->scavengingModule.parentPlace = this;
}

std::unique_ptr<Building> Place::clone() {
	return std::make_unique<Place> ( *this );
}

Place &Place::parseData ( std::string htmlData ) {
	xmlDoc *doc = htmlReadDoc ( (xmlChar*)htmlData.c_str (), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING );
	xmlNode *r = xmlDocGetRootElement ( doc );
	xmlpp::Element* root = new xmlpp::Element(r);
	Glib::ustring xpath = "//table[@id=\"units_home\"]/tr";

	//Gets units available at home
	xmlpp::Node::NodeSet rows = root->find ( xpath );

	//Gets units from this village only for now
	xmlpp::Node::NodeList cells = rows[1]->get_children ( "td" ); //rows[0] is table headers row
	auto cellIterator = ++++++cells.begin(); //skip first 3 cells
	for ( ; cellIterator != cells.end (); cellIterator++ ) { //Skipping first cell with source village
		std::string cellClass = dynamic_cast<xmlpp::Element*> ( *cellIterator )->get_attribute_value ( "class" );
		std::regex regUnitType ( "unit-item-([a-z]*)");
		std::smatch matchUnitType;
		if ( std::regex_search ( cellClass, matchUnitType, regUnitType ) ) {
//			std::cout << "unit name here : " << matchUnitType.str(1) << std::flush;
			//Get number of units of that type
			int unitCount = std::stoi ( dynamic_cast<xmlpp::Element*> ( *cellIterator )->get_first_child_text ()->get_content () );
			this->getUnit ( matchUnitType.str(1) )->unitCount = unitCount;
		}
		else {
			std::cout << "Error parsing cell data in army overview screen" << std::endl;
			throw std::string ( "Wololo" );
		}
	}
//	std::string source; //Where units are from
//	source = dynamic_cast<xmlpp::Element*> ( (*cellIterator) )->get_first_child_text ()->get_content ();
//	std::cout << source << std::endl;
	return *this;
}

Unit *Place::getUnit ( std::string unitName ) {
	for ( auto &unit : this->units ) {
		if ( unit->name == unitName ) {
			return unit.get ();
		}
	}
	std::cout << "Couldnt find unit: " << unitName << ". Dont do it again\n";
	throw std::string ( "Awwww" );
}

int Place::getUnitsPopulation() {
	int result = 0;
	//Counting units at home
	for ( std::unique_ptr<Unit> &unit : this->units ) {
		result += unit->pop * unit->unitCount;
	}
	//Counting units on missions
	std::vector<Order*> orders = this->getOrders (*this);
	for ( Order *order : orders ) {
		for ( std::pair<std::string, int> &unit : dynamic_cast<ScavengeOrder*> ( order )->units ) {
			result += this->getUnit ( unit.first )->pop * unit.second;
		}
	}
	return result;
}

bool Place::ScavengingModule::sendScavengeParty ( std::vector<std::pair<std::string, int>> units, int scavengingLevel ) {

	//checking if there is enough units to send and calculating total carry capacity of units
	int carryMax = 0;
	for ( std::pair<std::string, int> &unit : units ) {
		if ( this->parentPlace->getUnit ( unit.first )->unitCount < unit.second ) {
			Game::logCallback ("Not enough units to send scavenging.\n");
			InsufficentUnits ex;
			throw ex;
		}
		carryMax += this->parentPlace->getUnit ( unit.first )->carry * unit.second;
	}
	std::string responseJson = this->parentPlace->parentVillage->owner->ownerAccount
	->startScavenging ( *this->parentPlace->worldSettings->world, this->parentPlace->parentVillage->id, units, scavengingLevel, carryMax );

	bsoncxx::document::value doc = bsoncxx::from_json ( responseJson );
	if ( doc.view ()["squad_responses"].type () == bsoncxx::type::k_array ) {
//		std::cout << "its an array" << std::endl;
		bsoncxx::array::view subarr{doc.view ()["squad_responses"].get_array().value};
		for ( bsoncxx::array::element ele : subarr ) {
//			std::cout << "success: " << ele["success"].get_bool().value << std::endl;
			if ( ele["success"].get_bool().value == false ) {
//				std::cout << "msg: " << ele["error"].get_utf8().value << std::endl;
				GameException ex ( ele["error"].get_utf8().value.data() );
				throw ex;
			}
		}
	}
	//Now get id of created mission. Its mostly useless because game cannot cancel scavenging.
	int newId = doc.view ()["villages"][std::to_string (this->parentPlace->parentVillage->id)]["options"][std::to_string (scavengingLevel)]["scavenging_squad"]["unit_id"].get_int32().value;
	//Calculate loot and duration of scavenge
	double newLoot = carryMax * this->scavengingLevels[scavengingLevel-1].lootFactor;
//	 (((newLoot) ^ 2 * 100) ^ durationExponent + durationInitialSeconds) * durationFactor
	int newDuration = ( std::pow ( std::pow ( newLoot, 2 ) * 100, this->scavengingLevels[scavengingLevel-1].durationExponent )
	+ this->scavengingLevels[scavengingLevel-1].durationInitialSeconds ) * this->scavengingLevels[scavengingLevel-1].durationFactor;
	Resources newResources ( newLoot / 3.0, newLoot / 3.0, newLoot / 3.0 );
//	std::cout << "newLoot: " << newLoot << std::endl;

	//Deduct units from place
	for ( std::pair<std::string, int> &unit : units ) {
		this->parentPlace->getUnit ( unit.first )->unitCount -= unit.second;
	}
	//Create scavengeOrder
	std::unique_ptr<ScavengeOrder> mission ( new ScavengeOrder ( *this->parentPlace, units, newResources, scavengingLevel ) );
	mission->dueTime = Utils::getTimestamp () + newDuration;
//	std::cout << "Mission duration in seconds: " + std::to_string (newDuration);
//	std::cout << "Finish time of scavenging mission: " << Utils::timestampToString (mission->dueTime) << std::endl;
	mission->id = newId;
	mission->text = "Scavenging mission " + this->scavengingLevels[scavengingLevel-1].name;
	this->parentPlace->addOrder ( std::move ( mission) );

	return true;
}

bool Place::ScavengingModule::parseData ( std::string htmlData ) {
	std::regex regVillageJson ( "var village = (\\{.+\\})" );
	std::smatch matchVillageJson;
	if ( !std::regex_search ( htmlData, matchVillageJson, regVillageJson ) ) {
		std::cout << "cannot find village variable in scavenging html. Its over." << std::endl;
		throw "Oompaloompa";
	}
	//contains info about unlocked scavenging levels and scavenging missions
	std::string villageJson = matchVillageJson.str (1);

	bsoncxx::document::value doc = bsoncxx::from_json ( villageJson );
	for ( auto v : doc.view()["options"].get_document ().view () ) {
		int missionId;
		int missionLevel; // Scavenging level
		std::vector<std::pair<std::string, int>> missionUnits;
		Resources missionLoot;
		int missionReturnTime;

		missionLevel = std::stoi ( std::string (v.key()) );
//		std::cout << "level id: " << v.key() << std::endl;
//		std::cout << "is_locked: " << (v.get_document().view()["is_locked"].get_bool().value ? "true" : "false") << std::endl;

		switch ( v.get_document().view()["unlock_time"].type () ) {
			case bsoncxx::type::k_null:
//				std::cout << "unlock_time is null" << std::endl;
				break;
			default:
				std::cout << "unlock_time is something else. scavenging level is being updated" << std::endl;
		}
		if ( v.get_document().view()["scavenging_squad"].type() == bsoncxx::type::k_document ) {
//			for ( auto mission : v.get_document().view()["scavenging_squad"].get_document().view() ) {
////				std::cout << mission.key() << std::endl;
//			}
//			std::cout << "mission id: " << v.get_document().view()["scavenging_squad"]["unit_id"].get_int32().value << std::endl;
			missionId = v.get_document().view()["scavenging_squad"]["unit_id"].get_int32().value;
//			std::cout << "due time: " << Utils::timestampToString (v.get_document().view()["scavenging_squad"]["return_time"].get_int32().value) << std::endl;
			missionReturnTime = v.get_document().view()["scavenging_squad"]["return_time"].get_int32().value;

			missionLoot.wood = v.get_document().view()["scavenging_squad"]["loot_res"]["wood"].get_int32().value;
			missionLoot.stone = v.get_document().view()["scavenging_squad"]["loot_res"]["stone"].get_int32().value;
			missionLoot.iron = v.get_document().view()["scavenging_squad"]["loot_res"]["iron"].get_int32().value;
//			std::cout << "loot: " << missionLoot.toString () << std::endl;
			for ( auto unit : v.get_document().view()["scavenging_squad"]["unit_counts"].get_document().view() ) {
				missionUnits.push_back ( std::make_pair ( std::string(unit.key()), unit.get_int32().value ) );
//				std::cout << unit.key() << "-" << unit.get_int32().value << " ";
			}
			//Create scavengeOrder
			std::unique_ptr<ScavengeOrder> mission ( new ScavengeOrder ( *this->parentPlace, missionUnits, missionLoot, missionLevel ) );
			mission->dueTime = missionReturnTime;
			mission->id = missionId;
			mission->text = "Scavenging mission " + std::to_string (this->parentPlace->parentVillage->id);
			this->parentPlace->addOrder ( std::move ( mission) );
		}
	}
	return true;
}

Place::ScavengingModule::ScavengingModule ( Place *parentPlace ) : parentPlace(parentPlace) {
	//Maybe use enums for this kind of stuff
	// hauls=3 means scavenging is enabled
	this->enabled = (this->parentPlace->worldSettings->hauls == 3) ? true : false;

	//Init scavenging levels array.
	double durationFactorTemp = std::pow(this->parentPlace->worldSettings->unitSpeed, -0.45) / this->parentPlace->worldSettings->speed;
	this->scavengingLevels[0].name = "Lackadaisacal looters";
	this->scavengingLevels[0].id = 1;
	this->scavengingLevels[0].durationFactor = durationFactorTemp;
	this->scavengingLevels[0].unlockCost = Resources ( 25, 30, 25 );
	this->scavengingLevels[0].unlockDurationSeconds = 30;
	this->scavengingLevels[0].lootFactor = 0.1;

	this->scavengingLevels[1].name = "Humble haulers";
	this->scavengingLevels[1].id = 2;
	this->scavengingLevels[1].durationFactor = durationFactorTemp;
	this->scavengingLevels[1].unlockCost = Resources ( 250, 300, 250 );
	this->scavengingLevels[1].unlockDurationSeconds = 3600;
	this->scavengingLevels[1].lootFactor = 0.25;

	this->scavengingLevels[2].name = "Clever collectors";
	this->scavengingLevels[2].id = 3;
	this->scavengingLevels[2].durationFactor = durationFactorTemp;
	this->scavengingLevels[2].unlockCost = Resources ( 1000, 1200, 1000 );
	this->scavengingLevels[2].unlockDurationSeconds = 10800;
	this->scavengingLevels[2].lootFactor = 0.5;

	this->scavengingLevels[3].name = "Great Gatherers";
	this->scavengingLevels[3].id = 4;
	this->scavengingLevels[3].durationFactor = durationFactorTemp;
	this->scavengingLevels[3].unlockCost = Resources ( 10000, 12000, 10000 );
	this->scavengingLevels[3].unlockDurationSeconds = 21600;
	this->scavengingLevels[3].lootFactor = 0.75;
}
