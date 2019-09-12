#include "unitfactory.h"

#include <game/worldsettings.h>

UnitFactory::UnitFactory () {

}

std::unique_ptr<Unit> UnitFactory::createUnit ( std::string name, std::string xmlData, WorldSettings *worldSettings ) {
	xmlpp::DomParser parser;
	parser.parse_memory ( xmlData );
	xmlpp::Document *doc = parser.get_document ();
	xmlpp::Node *newRoot = doc->get_root_node ()->get_first_child ( name );
	xmlpp::Document newDocument;
	newDocument.create_root_node_by_import ( newRoot );
	xmlData = newDocument.write_to_string ();
	if ( name == "axe" ) {
		return std::make_unique<Axe> ( Axe ( xmlData, worldSettings ) );
	}
	if ( name == "catapult" ) {
		return std::make_unique<Catapult> ( Catapult ( xmlData, worldSettings ) );
	}
	if ( name == "archer" ) {
		return std::make_unique<Farcher> ( Farcher ( xmlData, worldSettings ) );
	}
	if ( name == "heavy" ) {
		return std::make_unique<Heavy> ( Heavy ( xmlData, worldSettings ) );
	}
	if ( name == "light" ) {
		return std::make_unique<Light> ( Light ( xmlData, worldSettings ) );
	}
	if ( name == "marcher" ) {
		return std::make_unique<Marcher> ( Marcher ( xmlData, worldSettings ) );
	}
	if ( name == "militia" ) {
		return std::make_unique<Militia> ( Militia ( xmlData, worldSettings ) );
	}
	if ( name == "knight" ) {
		return std::make_unique<Paladin> ( Paladin ( xmlData, worldSettings ) );
	}
	if ( name == "ram" ) {
		return std::make_unique<Ram> ( Ram ( xmlData, worldSettings ) );
	}
	if ( name == "spear" ) {
		return std::make_unique<Spear> ( Spear ( xmlData, worldSettings ) );
	}
	if ( name == "spy" ) {
		return std::make_unique<Spy> ( Spy ( xmlData, worldSettings ) );
	}
	if ( name == "sword" ) {
		return std::make_unique<Sword> ( Sword ( xmlData, worldSettings ) );
	}
	if ( name == "snob" ) {
		return std::make_unique<SnobUnit> ( SnobUnit ( xmlData, worldSettings ) );
	}
	return nullptr;
}
