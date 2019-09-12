#include "worldsettings.h"

#include <game/game.h>

WorldSettings::WorldSettings () {

}

WorldSettings::WorldSettings ( std::string xml ) {
	xmlpp::DomParser parser;
//	parser.set_validate ( false );
	parser.set_substitute_entities ();
	parser.parse_memory ( Glib::ustring ( xml ) );
	if ( parser ) {
		xmlpp::Document *doc = parser.get_document ();
		xmlpp::Element *root = doc->get_root_node ();
		this->speed = std::stod ( dynamic_cast<xmlpp::Element*> ( root->find ( "//speed" )[0] )->get_first_child_text ()->get_content () );
		this->unitSpeed = std::stod ( dynamic_cast<xmlpp::Element*> ( root->find ( "//unit_speed" )[0] )->get_first_child_text ()->get_content () );
		this->morale = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//moral" )[0] )->get_first_child_text ()->get_content () );
		this->canDestroyBuildings = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//build/destroy" )[0] )->get_first_child_text ()->get_content () );
		this->commandCancelTime = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//commands/command_cancel_time" )[0] )->get_first_child_text ()->get_content () );
		this->tradeCancelTime = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//misc/trade_cancel_time" )[0] )->get_first_child_text ()->get_content () );
		this->knight = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//game/knight" )[0] )->get_first_child_text ()->get_content () );
		this->archers = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//game/archer" )[0] )->get_first_child_text ()->get_content () );
		this->watchtower = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//game/watchtower" )[0] )->get_first_child_text ()->get_content () );
		xmlpp::TextNode *node = dynamic_cast<xmlpp::Element*> ( root->find ( "//game/stronghold" )[0] )->get_first_child_text ();
		if ( node != NULL )
			this->stronghold = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//game/stronghold" )[0] )->get_first_child_text ()->get_content () );
		this->church = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//game/church" )[0] )->get_first_child_text ()->get_content () );
		this->hauls = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//game/hauls")[0] )->get_first_child_text ()->get_content () );
		this->night = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//night/active" )[0] )->get_first_child_text ()->get_content () );
		this->nightStartHour = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//night/start_hour" )[0] )->get_first_child_text ()->get_content () );
		this->nightEndHour = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//night/end_hour" )[0] )->get_first_child_text ()->get_content () );
		this->nightDefFactor = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//night/def_factor" )[0] )->get_first_child_text ()->get_content () );
		this->noobProtectDays = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//newbie/days" )[0] )->get_first_child_text ()->get_content () );
		this->ratio = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//newbie/ratio" )[0] )->get_first_child_text ()->get_content () );
		this->ratioDays = std::stoi ( dynamic_cast<xmlpp::Element*> ( root->find ( "//newbie/ratio_days" )[0] )->get_first_child_text ()->get_content () );
	}
	Game::logCallback ("Finished parsing world settings\n");
}
