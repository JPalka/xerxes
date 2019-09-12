#include "buildorder.h"

#include <buildings/buildingfactory.h>
#include <game/game.h>

BuildOrder::BuildOrder ( Building &target, Building &source ) : target(target), source(source) {

}

bool BuildOrder::execute () {
	Game::logCallback ( "Constructed " + this->target.name + " lvl " +
	std::to_string (this->newLevel) + " in village " + this->source.parentVillage->name);
//	std::cout << "Executing build order " << this->id << ". Constructed " << this->target.name
//	<< " lvl" << this->newLevel << " in village " << this->source.parentVillage->name << std::endl;

	this->target.setLevel ( this->newLevel );
	return true;
}

bool BuildOrder::operator== ( const Building &rhs ) const {
	if ( (this->source.parentVillage->name == rhs.parentVillage->name) && this->source.name == rhs.name ) {
		return true;
	}
	return false;
}
