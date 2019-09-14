#include "army.h"

#include "game/game.h"
#include "game/village.h"
#include "buildings/place.h"

Army::Army() {

}

void Army::setHome ( Place *newHome ) {
	if ( newHome != nullptr ) {
		this->home = newHome;
	}
}

void Army::setHome ( Village *newHome ) {
	if ( newHome != nullptr ) {
		this->home = &static_cast<Place&> (newHome->getBuilding ( "place" ));
	}
}

int Army::addUnit ( std::string unitName, int unitCount ) {
	this->units[unitName] += std::abs ( unitCount );
	return this->units[unitName];
}

int Army::removeUnit ( std::string unitName, int unitCount ) {
	this->units[unitName] -= std::abs ( unitCount );
	if ( this->units[unitName] < 0 ) {
		this->units[unitName] = 0;
	}
	return this->units[unitName];
}

bool Army::hasNoble () {
	return this->units["snob"] > 0;
}

bool Army::hasScout () {
	return this->units["spy"] > 0;
}
// Armies with militia(filthy peasantry) cannot attack
bool Army::canAttack () {
	return this->units["militia"] == 0;
}

std::tuple<double, double, double> Army::getAttack () {
	// Cannot calculate strength of an army without home
	if ( this->home == nullptr ) {
		return std::make_tuple ( 0, 0, 0 );
	}
	double infantry {0}, cavalry {0}, archer {0};
	for ( std::pair<std::string, int> unit : this->units ) {
		try {
			Unit *unitPtr = this->home->getUnit ( unit.first );
			//TODO: check unit type and add to infantry, cavalry, archer variables;
			infantry += unitPtr->attack * unit.second;
		}
		catch (...) { //TODO: catch InvalidUnit exception when I make it
		}
	}
	return std::make_tuple ( infantry, cavalry, archer );
}

std::tuple<double, double, double> Army::getDefence () {
	// Cannot calculate strength of an army without home
	if ( this->home == nullptr ) {
		return std::make_tuple ( 0, 0, 0 );
	}
	double infantry {0}, cavalry {0}, archer {0};
	for ( std::pair<std::string, int> unit : this->units ) {
		try {
			Unit *unitPtr = this->home->getUnit ( unit.first );
			infantry += unitPtr->defence * unit.second;
			cavalry += unitPtr->defenceCavalry * unit.second;
			archer += unitPtr->defenceArcher * unit.second;
		}
		catch (...) { //TODO: catch InvalidUnit exception when I make it
		}
	}
	return std::make_tuple ( infantry, cavalry, archer );
}

double Army::getLoot () {
	if ( this->home == nullptr ) {
		return 0;
	}
	double lootCapacity = 0;
	for ( std::pair<std::string, int> unit : this->units ) {
		try {
			Unit *unitPtr = this->home->getUnit ( unit.first );
			lootCapacity += unitPtr->carry * unit.second;
		}
		catch (...) {//TODO: catch specific exception
		}
	}
	return lootCapacity;
}

double Army::getSpeed () {
	if ( this->home == nullptr ) {
		return 0;
	}
	double minimumSpeed = 0; // Slowest unit actually has the biggest number
	for ( std::pair<std::string,int> unit : this->units ) {
		try {
			Unit *unitPtr = this->home->getUnit ( unit.first );
			if ( unitPtr->speed > minimumSpeed ) {
				minimumSpeed = unitPtr->speed;
			}
		}
		catch (...) { //TODO: catch InvalidUnit exception when I make it
		}
	}
	return minimumSpeed;
}

Resources Army::getCost () {
	if ( this->home == nullptr ) {
		return Resources {0, 0, 0};
	}
	Resources totalCost {0, 0, 0};
	for ( std::pair<std::string, int> unit : this->units ) {
		try {
			Unit *unitPtr = this->home->getUnit ( unit.first );
			totalCost += unitPtr->cost * unit.second;
		}
		catch (...) { //TODO: catch InvalidUnit exception when I make it
		}
	}
	return totalCost;
}

std::string Army::toString () {
	std::string result {""};
	result += "Units: ";
	for ( auto unit : this->units ) {
		result += unit.first + " - " + std::to_string (unit.second) + "\n";
	}
	std::tuple<double, double, double> attack = this->getAttack ();
	std::tuple<double, double, double> defence = this->getDefence ();
	result += "Attack power: " + std::to_string ( std::get<0>(attack) ) + "/" + std::to_string ( std::get<1>(attack) );
	result += "/" + std::to_string ( std::get<2>(attack) ) + "\n";
	result += "Defence power: " + std::to_string ( std::get<0>(defence) ) + "/" + std::to_string ( std::get<1>(defence) );
	result += "/" + std::to_string ( std::get<2>(defence) ) + "\n";
	result += "Carry capacity: " + std::to_string ( this->getLoot () ) + "\n";
	result += "Speed: " + std::to_string ( this->getSpeed () ) + " minutes per tile\n";
	return result;
}
