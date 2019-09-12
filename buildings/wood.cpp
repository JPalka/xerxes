#include "wood.h"
#include <game/worldsettings.h>
#include <cmath>

Wood::Wood ( std::string xmlData, WorldSettings *worldSettings ) : Building ( xmlData, worldSettings ) {

}

double Wood::getProduction () {
	//Level 0 buildings have fixed production like 30/h or something. TODO: check if world speed affects that
	if ( this->level == 0 ) {
		return 30;
	}
	return std::round ( 30 * std::pow ( 1.163118, this->level - 1 ) * this->worldSettings->speed );
}

std::string Wood::getUrlSlug () {
	return "&screen=wood";
}

std::string Wood::toString() {
	std::string result = Building::toString ();
	result += "Production: " + std::to_string((int)this->getProduction ()) + " per hour\n";
	return result;
}

std::unique_ptr<Building> Wood::clone() {
	return std::make_unique<Wood> ( *this );
}
