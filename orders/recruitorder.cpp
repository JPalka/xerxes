#include "recruitorder.h"

// Again including unitfactory just so I dont have to include every unit separately. Think of something better
#include <units/unitfactory.h>
#include <game/game.h>
#include <buildings/buildingfactory.h>

RecruitOrder::RecruitOrder ( Unit &unit, Building &source ) : unit(unit), source(source) {

}

bool RecruitOrder::execute() {
	Game::logCallback ( "Recruited " + this->unit.name + " in " + this->source.parentVillage->name );
//	std::cout << "Executing recruit order " << this->id << ". Recruited " << this->unit.name
//	<< "in village " << this->source.parentVillage->name << std::endl;
	//Increase count of the unit pointed to by reference field
	this->unit.unitCount++;
	return true;
}

bool RecruitOrder::operator== ( const Building &rhs ) const{
	if ( (this->source.parentVillage->name == rhs.parentVillage->name) && this->source.name == rhs.name ) {
		return true;
	}
	return false;
}
