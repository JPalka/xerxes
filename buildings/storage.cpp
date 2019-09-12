#include "storage.h"

#include <misc/utils.h>

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

Storage &Storage::operator+ ( const Resources &rhs ) {
	this->storage = this->storage + rhs;
	return *this;
}

Storage &Storage::addResources ( const Resources &rhs ) {
	this->storage = this->storage + rhs;
	return *this;
}

Storage &Storage::operator- ( const Resources &rhs ) {
	if ( this->storage >= rhs ) {
		this->storage = this->storage - rhs;
		return *this;
	}
	std::cout << "Storage cannot go into negatives" << std::endl;
	throw "ding";
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
