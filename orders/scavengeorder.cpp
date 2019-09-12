#include "scavengeorder.h"
#include <buildings/building.h>
#include <buildings/place.h>

ScavengeOrder::ScavengeOrder ( Place &source, std::vector<std::pair<std::string, int> > unitsSend, Resources loot, int level )
: source(source), units(unitsSend), loot(loot), level(level) {

}


bool ScavengeOrder::execute () {
	std::cout << "executing scavenging order: " << this->id << std::endl;

	//TODO: return units to source place

	//TODO: add resources to warehouse of village of source place
	return true;
}

bool ScavengeOrder::operator== ( const Building &rhs ) const {
	if ( (this->source.parentVillage->name == rhs.parentVillage->name) && this->source.name == rhs.name ) {
		return true;
	}
	return false;
}
