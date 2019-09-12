#include "farm.h"
#include <game/village.h>
#include <buildings/buildingfactory.h>
Farm::Farm (std::string xmlData , WorldSettings *worldSettings) : Building ( xmlData, worldSettings ) {
	this->currentPopulation = 0;
}

std::string Farm::getUrlSlug () {
	return "&screen=farm";
}

Farm &Farm::parseData ( std::string htmlData ) {
	xmlDoc *doc = htmlReadDoc ( (xmlChar*)htmlData.c_str (), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING );
	xmlNode *r = xmlDocGetRootElement ( doc );
	xmlpp::Element* root = new xmlpp::Element(r);
	Glib::ustring xpath = "//span[@id=\"pop_current_label\"]";
	xmlpp::Node::NodeSet tempNode = root->find ( xpath );
	int currentPop = std::stoi ( dynamic_cast<xmlpp::Element*> ( *tempNode.begin () )->get_first_child_text ()->get_content () );
	this->currentPopulation = currentPop;
	return *this;
}

int Farm::getMaxPopulation() {
	return 240 * std::pow ( 1.172103, this->level - 1 );
}

/* Doesnt include building/units in construction queues. TODO: Correct that when I do proper Orders queue impl
 * For now it needs to read current population from html.
 * TODO: make it so it scans order queue + all buildings + all units in village and adds up their pop costs
 */
int Farm::getCurrentPopulation() {
	int buildings = 0;
	//Get building populations
	for ( std::unique_ptr<Building> &building : this->parentVillage->buildings ) {
		//Skip university and church until I figure out what to do with it
		if ( building->name == "university" || building->name == "church" ) {
			continue;
		}
		buildings += building->getPopulation ();
	}
	int units = static_cast<Place&> ( this->parentVillage->getBuilding ("place") ).getUnitsPopulation ();

	return buildings + units;
}
std::string Farm::toString () {
	std::string result = Building::toString ();
	result+= "Population: " + std::to_string (this->getCurrentPopulation ()) + "/" + std::to_string (this->getMaxPopulation ()) + "\n";
	return result;
}
std::unique_ptr<Building> Farm::clone() {
	return std::make_unique<Farm> ( *this );
}
