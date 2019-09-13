#include "buildorder.h"

#include "buildings/buildingfactory.h"
#include "game/game.h"

BuildOrder::BuildOrder ( Building &target, Building &source ) : target(target), source(source) {

}

bool BuildOrder::execute () {
	Game::logCallback ( "Constructed " + this->target.name + " lvl " +
	std::to_string (this->target.level+1) + " in village " + this->source.parentVillage->name);
	this->target.setLevel ( this->target.level + 1 );
	return true;
}

bool BuildOrder::operator== ( const Building &rhs ) const {
	if ( (this->source.parentVillage->name == rhs.parentVillage->name) && this->source.name == rhs.name ) {
		return true;
	}
	return false;
}
