#include "university.h"
//Its not good. Should delegate to base Building constructor but parsing code is rubbish. Fix it sometime.
University::University (std::string xmlData , WorldSettings *worldSettings) {
	this->worldSettings = worldSettings;
	xmlpp::DomParser parser;
	parser.parse_memory ( xmlData );
	if ( parser ) {
		xmlpp::Document *doc = parser.get_document ();
		xmlpp::Node *root = doc->get_root_node ();
		this->name = root->get_name ();
		this->maxLevel = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//max_level")[0] )->get_first_child_text ()->get_content () );
		this->minLevel = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//min_level")[0] )->get_first_child_text ()->get_content () );
		this->level = this->minLevel;
	}
	else {
		std::cout << "Error parsing xmlData in Building::University constructor\n";
		throw "REEEEEEE";
	}
}
