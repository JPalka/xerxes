#include "storage.h"

#include <misc/utils.h>
#include <game/game.h>

#include <exceptions/insufficentresources.h>

Storage::Storage (std::string xmlData, WorldSettings *worldSettings) : Building ( xmlData, worldSettings ) {
	//setting default storage. AFAIK 3k of each resource, verify.
	this->storage = Resources ( 3000, 3000, 3000 );

}

std::unique_ptr<Building> Storage::clone() {
	return std::make_unique<Storage> ( *this );
}

Resources Storage::getStorage () {
	return this->storage;
}
Resources Storage::getCapacity () {
	long int capacity = 0;
	capacity = std::round ( 1000 * std::pow( 1.2294934, this->level - 1 ) );
	Resources result;
	result.wood = capacity;
	result.stone = capacity;
	result.iron = capacity;
	return result;
}

Storage &Storage::addResources ( const Resources &rhs ) {
	this->storage = this->storage + rhs;
	//if resources in storage exceed max then set to max
	Resources max {this->getCapacity ()};
	if ( this->storage.wood > max.wood ) {
		this->storage.wood = max.wood;
	}
	if ( this->storage.stone > max.stone ) {
		this->storage.stone = max.stone;
	}
	if ( this->storage.iron > max.iron ) {
		this->storage.iron = max.iron;
	}
	return *this;
}

Storage &Storage::substractResources ( const Resources &rhs ) {
	if ( this->storage >= rhs ) {
		this->storage = this->storage - rhs;
		return *this;
	}
	Game::logCallback ( "Storage cannot go into negatives\n" );
	throw InsufficentResources();
}

std::string Storage::getUrlSlug () {
	return "&screen=storage";
}

Storage &Storage::parseData ( std::string htmlData ) {
	xmlDoc *doc = htmlReadDoc ( (xmlChar*)htmlData.c_str (), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING );
	xmlNode *r = xmlDocGetRootElement ( doc );
	xmlpp::Element* root = new xmlpp::Element(r);
	Glib::ustring xpath = "//span[@id=\"wood\"]";
	xmlpp::Node::NodeSet tempNode = root->find ( xpath );
	int woodStored = std::stoi ( dynamic_cast<xmlpp::Element*> ( *tempNode.begin () )->get_first_child_text ()->get_content () );
	xpath = "//span[@id=\"stone\"]";
	tempNode = root->find ( xpath );
	int stoneStored = std::stoi ( dynamic_cast<xmlpp::Element*> ( *tempNode.begin () )->get_first_child_text ()->get_content () );
	xpath = "//span[@id=\"iron\"]";
	tempNode = root->find ( xpath );
	int ironStored = std::stoi ( dynamic_cast<xmlpp::Element*> ( *tempNode.begin () )->get_first_child_text ()->get_content () );
	this->storage = Resources ( woodStored, stoneStored, ironStored );
	return *this;
}
