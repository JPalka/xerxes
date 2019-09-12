#include "village.h"

#include <game/player.h>
#include <misc/utils.h>

#include <buildings/buildingfactory.h> // I include factory because it includes all buildings already

Building &Village::operator[] ( std::string buildingName ) {
	//find building with given name
	for ( size_t i = 0; i < this->buildings.size (); i++ ) {
		if ( this->buildings[i]->name == buildingName ) {
			return *this->buildings[i];
		}
	}
	std::cout << "No building with name: " << buildingName << ". Tragedy\n";
	throw "Ugabuga";
}

Building &Village::getBuilding ( std::string buildingName ) {
	//find building with given name
	for ( size_t i = 0; i < this->buildings.size (); i++ ) {
		if ( this->buildings[i]->name == buildingName ) {
			return *this->buildings[i];
		}
	}
	std::cout << "No building with name: " << buildingName << ". Tragedy\n";
	throw "Ugabuga";
}

Village::Village ( unsigned long id, std::string name, Coordinate location, Player &owner, std::vector<std::unique_ptr<Building>> &buildings ) :
id(id), name(name), location(location), owner(&owner)  {
	this->setBuildings ( buildings );
	this->lastUpdated = Utils::getTimestamp ();
}

Village::Village ( const Village &rhs ) : id(rhs.id), name(rhs.name), location(rhs.location), owner(rhs.owner) {
	this->setBuildings ( rhs.buildings );
	this->lastUpdated = rhs.lastUpdated;
}

Village &Village::setBuildings ( const std::vector<std::unique_ptr<Building>> &newBuildings ) {
	for ( size_t i = 0; i < newBuildings.size (); i++ ) {
		std::unique_ptr<Building> building ( newBuildings[i]->clone() );
		building->parentVillage = this;
		this->buildings.push_back ( std::move ( building ) );
	}
	return *this;
}

std::string Village::getUrlSlug () {
	std::string urlSlug = "village=" + std::to_string ( this->id ) + "&";
	return urlSlug;
}

bool Village::updateBuildingsLevels ( std::string jsonData ) {
	std::fstream test ( "testfile.txt", std::ios::out );
	test << jsonData;
	test.close();
	bsoncxx::document::value doc = bsoncxx::from_json ( jsonData );
	for ( auto iter = doc.view ().begin (); iter != doc.view ().end (); iter++ ) {
//		std::cout << "Building: " << iter->key () << std::endl;
		std::string buildName ( iter->key () );
		(*this)[buildName].setLevel ( std::stoi ( (std::string)(*iter)["level"].get_utf8().value ) );
	}
	return true;
}

std::string Village::toString() {
	std::string result = "";
	result += "Village id: ";
	result += std::to_string ( this->id );
	result += "\n";
	result += "Village name: ";
	result += this->name + "\n";
	result += "Location: " + this->location.toString () + "\n";
	result += "Owner: " + this->owner->name + "\n";
	result += "Last updated: " + Utils::timestampToString ( this->lastUpdated );
	result += "Buildings:\n";
	for ( std::unique_ptr<Building> &building : this->buildings ) {
		result += building->name + ": " + std::to_string (building->level) + "\n";
	}
	return result;
}

Village &Village::calculateResources ( long newTime ) {
	Storage &store = static_cast<Storage&> ( this->getBuilding ("storage") );
	if ( newTime < store.lastUpdated ) {
		std::cout << "New time smaller than time of last update of warehouse. Cannot step back in time doctor" << std::endl;
		throw std::string ( "WhateverException" );
	}
	long int timeElapsed = newTime - store.lastUpdated;
	Resources resourcesPerSecond;
	resourcesPerSecond.wood = static_cast<Wood&> ( this->getBuilding ( "wood" ) ).getProduction () / 60. / 60.;
	resourcesPerSecond.stone = static_cast<Stone&> ( this->getBuilding ( "stone" ) ).getProduction () / 60. / 60.;
	resourcesPerSecond.iron = static_cast<Iron&> ( this->getBuilding ( "iron" ) ).getProduction () / 60. / 60.;
//	if ( this->id == 16530 ) { //DEBUG CODE REMOVE
//		std::cout << "Seconds elapsed since last tick: " << timeElapsed << std::endl;
//		std::cout << "RPS: " << resourcesPerSecond.toString () << std::endl;
//	}

	Resources resourcesGathered = resourcesPerSecond * timeElapsed;
	store.addResources ( resourcesGathered );
	store.lastUpdated = newTime;
	return *this;
}
