#include "unit.h"
#include <game/worldsettings.h>

Unit::Unit ( std::string xmlData, WorldSettings *worldSettings ) : worldSettings( worldSettings ) {
	xmlpp::DomParser parser;
	parser.parse_memory ( xmlData );
	if ( parser ) {
		xmlpp::Document *doc = parser.get_document ();
		xmlpp::Node *root = doc->get_root_node ();
		this->name = root->get_name ();
		this->unitCount = 0;
		this->buildTime = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//build_time")[0] )->get_first_child_text ()->get_content () );
		this->pop = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//pop")[0] )->get_first_child_text ()->get_content () );
		this->speed = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//speed")[0] )->get_first_child_text ()->get_content () );
		this->attack = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//attack")[0] )->get_first_child_text ()->get_content () );
		this->defence = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//defense")[0] )->get_first_child_text ()->get_content () );
		this->defenceCavalry = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//defense_cavalry")[0] )->get_first_child_text ()->get_content () );
		this->defenceArcher = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//defense_archer")[0] )->get_first_child_text ()->get_content () );
		this->carry = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//carry")[0] )->get_first_child_text ()->get_content () );
	}
	else {
		std::cout << "Error parsing xmlData in Unit constructor\n";
		throw "REEEEEEE";
	}
}

std::unique_ptr<Unit> Unit::clone () {
	return std::make_unique<Unit> ( *this );
}
