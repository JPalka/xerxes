#include "buildingfactory.h"
#include <game/game.h>
BuildingFactory::BuildingFactory() {
}

std::unique_ptr<Building> BuildingFactory::createBuilding (std::string name, std::string xmlData , WorldSettings *worldSettings) {
	xmlpp::DomParser parser;
	parser.parse_memory ( xmlData );
	xmlpp::Document *doc = parser.get_document ();
	xmlpp::Node *newRoot = doc->get_root_node ()->get_first_child ( name );
	xmlpp::Document newDocument;
	newDocument.create_root_node_by_import ( newRoot );
	xmlData = newDocument.write_to_string ();
	if ( name == "main" ) {
		return std::make_unique<Headquarters> ( Headquarters ( xmlData, worldSettings ) );
	}
	if ( name == "barracks" ) {
		return std::make_unique<Barracks> ( Barracks ( xmlData, worldSettings ) );
	}
	if ( name == "church" ) {
		return std::make_unique<Church> ( Church ( xmlData, worldSettings ) );
	}
	if ( name == "farm" ) {
		return std::make_unique<Farm> ( Farm ( xmlData, worldSettings ) );
	}
	if ( name == "garage" ) {
		return std::make_unique<Garage> ( Garage ( xmlData, worldSettings ) );
	}
	if ( name == "hide" ) {
		return std::make_unique<Hide> ( Hide ( xmlData, worldSettings ) );
	}
	if ( name == "iron" ) {
		return std::make_unique<Iron> ( Iron ( xmlData, worldSettings ) );
	}
	if ( name == "market" ) {
		return std::make_unique<Market> ( Market ( xmlData, worldSettings ) );
	}
	if ( name == "place" ) {
		return std::make_unique<Place> ( Place ( xmlData, worldSettings ) );
	}
	if ( name == "smith" ) {
		return std::make_unique<Smith> ( Smith ( xmlData, worldSettings ) );
	}
	if ( name == "snob" ) {
		return std::make_unique<Snob> ( Snob ( xmlData, worldSettings ) );
	}
	if ( name == "stable" ) {
		return std::make_unique<Stable> ( Stable ( xmlData, worldSettings ) );
	}
	if ( name == "statue" ) {
		return std::make_unique<Statue> ( Statue ( xmlData, worldSettings ) );
	}
	if ( name == "stone" ) {
		return std::make_unique<Stone> ( Stone ( xmlData, worldSettings ) );
	}
	if ( name == "storage" ) {
		return std::make_unique<Storage> ( Storage ( xmlData, worldSettings ) );
	}
	if ( name == "wall" ) {
		return std::make_unique<Wall> ( Wall ( xmlData, worldSettings ) );
	}
	if ( name == "watchtower" ) {
		return std::make_unique<Watchtower> ( Watchtower ( xmlData, worldSettings ) );
	}
	if ( name == "wood" ) {
		return std::make_unique<Wood> ( Wood ( xmlData, worldSettings ) );
	}
	if ( name == "church" || name == "church_f" ) { //TREAT FIRST CHURCH AND NEXT CHURCHES THE SAME WAY FOR NOW
		return std::make_unique<Church> ( Church ( xmlData, worldSettings ) );
	}
	if ( name == "university" ) {
		return std::make_unique<University> ( University ( xmlData, worldSettings ) );
	}
	//add other types
	return nullptr;
}

//Returns array of buildings that can be built based on xml passed.
std::vector<std::unique_ptr<Building>> BuildingFactory::getAvailableBuildings ( std::string xmlData, WorldSettings *worldSettings ) {
	std::vector<std::unique_ptr<Building>> buildingArray;
	xmlpp::DomParser parser;
	parser.set_substitute_entities ( true ); //what does that even do? TODO: lookup
	parser.parse_memory ( Glib::ustring ( xmlData ) );
	if ( parser ) {
		xmlpp::Document *doc = parser.get_document ();
		xmlpp::Element *root = doc->get_root_node ();
		xmlpp::Node::NodeList buildingNames = root->get_children ();
		Game::logCallback ( "Creating buildings: " );
		for ( auto it = buildingNames.begin(); it != buildingNames.end(); it++ ) {
			std::string nodeName = dynamic_cast<xmlpp::Node*> (*it)->get_name();
			if ( nodeName != "text" ) {
				if ( std::next (it) != --buildingNames.end () ) {
					Game::logCallback ( nodeName + ", " );
				}
				else {
					Game::logCallback ( nodeName + "\n" );
				}
				std::unique_ptr<Building> building = BuildingFactory::createBuilding ( nodeName, xmlData, worldSettings );
				buildingArray.push_back ( std::move ( building ) );
//				std::cout << "Created " << nodeName << "\n";
			}
		}
	}
	return buildingArray;
}
