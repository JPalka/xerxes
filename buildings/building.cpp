#include "building.h"

#include <misc/utils.h>
#include <orders/order.h>
#include <orders/buildorder.h>
#include <orders/recruitorder.h>
#include <orders/scavengeorder.h>

Building::Building (std::string xmlData , WorldSettings *worldSettings) : worldSettings(worldSettings) {
	xmlpp::DomParser parser;
	parser.parse_memory ( xmlData );
	if ( parser ) {
		xmlpp::Document *doc = parser.get_document ();
		xmlpp::Node *root = doc->get_root_node ();
		this->name = root->get_name ();
		this->maxLevel = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//max_level")[0] )->get_first_child_text ()->get_content () );
		this->minLevel = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//min_level")[0] )->get_first_child_text ()->get_content () );
		this->basePop = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//pop")[0] )->get_first_child_text ()->get_content () );
		this->woodFactor = std::stod ( dynamic_cast <xmlpp::Element*> ( root->find ( "//wood_factor")[0] )->get_first_child_text ()->get_content () );
		this->stoneFactor = std::stod ( dynamic_cast <xmlpp::Element*> ( root->find ( "//stone_factor")[0] )->get_first_child_text ()->get_content () );
		this->ironFactor = std::stod ( dynamic_cast <xmlpp::Element*> ( root->find ( "//iron_factor")[0] )->get_first_child_text ()->get_content () );
		this->popFactor = std::stof ( dynamic_cast <xmlpp::Element*> ( root->find ( "//pop_factor")[0] )->get_first_child_text ()->get_content () );
		this->buildTime = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//build_time")[0] )->get_first_child_text ()->get_content () );
		this->buildTimeFactor = std::stof ( dynamic_cast <xmlpp::Element*> ( root->find ( "//build_time_factor")[0] )->get_first_child_text ()->get_content () );
		Resources res;
		res.wood = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//" + this->name + "/wood")[0] )->get_first_child_text ()->get_content () );
		res.stone = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//" + this->name + "/stone")[0] )->get_first_child_text ()->get_content () );
		res.iron = std::stoi ( dynamic_cast <xmlpp::Element*> ( root->find ( "//" + this->name + "/iron")[0] )->get_first_child_text ()->get_content () );
		this->baseCost = res;
		this->level = this->minLevel;
		this->lastUpdated = Utils::getTimestamp ();
	}
	else {
		std::cout << "Error parsing xmlData in Building constructor\n";
		throw "REEEEEEE";
	}
}

Building &Building::setLevel ( int newLevel ) {
	if ( newLevel <= this->maxLevel ) {
		this->level = newLevel;
		this->lastUpdated = Utils::getTimestamp ();
	}
	else {
		std::cout << "Cannot set level over maximum level of building. Max level: " << this->maxLevel;
		std::cout << " new level: " << newLevel << std::endl;
		throw "GTFO";
	}
	return *this;
}
Building &Building::Upgrade () {
	this->level++; //TODO: do it properly
	return *this;
}
Building &Building::Degrade () {
	this->level--; //TODO: do it properly
	return *this;
}

Building::Building ( const Building &source ) {
	this->worldSettings = source.worldSettings;
	this->name = source.name;
	this->addOrder = source.addOrder;
	this->getOrders = source.getOrders;
	this->baseCost = source.baseCost;
	this->basePop = source.basePop;
	this->buildTime = source.buildTime;
	this->buildTimeFactor = source.buildTimeFactor;
	this->ironFactor = source.ironFactor;
	this->level = source.level;
	this->maxLevel = source.maxLevel;
	this->minLevel = source.minLevel;
	this->popFactor = source.popFactor;
	this->stoneFactor = source.stoneFactor;
	this->woodFactor = source.woodFactor;
	this->parentVillage = source.parentVillage;
	this->lastUpdated = source.lastUpdated;
}

std::string Building::toString() {
	std::string result = "";
	result += this->name + "\n";
	result += "Level: " + std::to_string (this->level) + "/" + std::to_string (this->maxLevel) + "\n";
	result += "Population used: " + std::to_string (this->getPopulation ()) + "\n";
	result += "Upgrade pop cost: " + std::to_string (this->getUpgradePopulationCost ()) + "\n";
	result += "Upgrade cost: " + this->getUpgradeCost ().toString () + "\n";
	result += "Last updated: " + Utils::timestampToString ( this->lastUpdated ) + "\n";
	result += "Url slug: " + this->getUrlSlug () + "\n";
	std::vector<Order*> orders = this->getOrders ( *this );
	for ( auto it = orders.rbegin (); it != orders.rend (); it++ ) {
		result += (*it)->text + " - " + Utils::timestampToString ( (*it)->dueTime ) + "\n";
	}

	return result;
}
//Base clone method
std::unique_ptr<Building> Building::clone () {
	std::unique_ptr<Building> result ( new Building() );
	result->worldSettings = this->worldSettings;
	result->name = this->name;
	result->addOrder = this->addOrder;
	result->getOrders = this->getOrders;
	result->baseCost = this->baseCost;
	result->basePop = this->basePop;
	result->buildTime = this->buildTime;
	result->buildTimeFactor = this->buildTimeFactor;
	result->ironFactor = this->ironFactor;
	result->level = this->level;
	result->maxLevel = this->maxLevel;
	result->minLevel = this->minLevel;
	result->popFactor = this->popFactor;
	result->stoneFactor = this->stoneFactor;
	result->woodFactor = this->woodFactor;
	result->lastUpdated = this->lastUpdated;
	return result;
}
//Get building upgrade cost
Resources Building::getUpgradeCost () {
	Resources result;
	result.wood = std::round ( this->baseCost.wood * std::pow ( this->woodFactor, this->level ) );
	result.stone = std::round ( this->baseCost.stone * std::pow ( this->stoneFactor, this->level ) );
	result.iron = std::round ( this->baseCost.iron * std::pow ( this->ironFactor, this->level ) );
	return result;
}

int Building::getUpgradePopulationCost() {
	return this->getPopulation (1) - this->getPopulation ();
}
int Building::getPopulation ( int levelOffset ) {
	int buildingLevel = this->level + levelOffset;
	if ( buildingLevel == 0 ) {
		return 0;
	}
	int pop = std::round ( this->basePop * std::pow ( this->popFactor, buildingLevel - 1 ) );
	return pop;
}
/*
		var exponent = Math.max(-13, (level - 1 - 14 / (level-1)));
		var baseTime = buildTime * 1.18 * Math.pow(buildTimeFactor, exponent);
		var time = baseTime * Math.pow(1.05, -hg);
*/
// FORMULA IS BOGUS. TODO: FIGURE IT OUT MYSELF.
int Building::getUpgradeTime ( int headquartersLevel = 1 ) {
	int result;
//	double exponent = std::max ( -13, ( this->level - 1 - 14 / (this->level-1 ) ) );
	result = this->buildTime * std::pow ( this->buildTimeFactor, this->level );
	result = std::round ( result * std::pow ( 1.05, headquartersLevel * -1 ) );
	return result;
}

std::string Building::getUrlSlug () {
	return "getUrlSlug() not implemented for base building type\n";
//	throw "SIO";
}

Building &Building::parseData ( std::string htmlData ) {
	std::string removeFukenWarning = htmlData;
	std::cout << "Placeholder for parseData method. Make it pure virtual when Im done with specific implementations\n";
	throw "You shall not pass";
	return *this;
}