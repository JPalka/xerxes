#include "stone.h"
#include <game/worldsettings.h>

Stone::Stone (std::string xmlData , WorldSettings *worldSettings) : Building ( xmlData, worldSettings ) {

}

double Stone::getProduction () {
	//Level 0 buildings have fixed production like 30/h or something. TODO: check if world speed affects that
	if ( this->level == 0 ) {
		return 30;
	}
	return std::round ( 30 * std::pow ( 1.163118, this->level - 1 ) * this->worldSettings->speed );
}

std::string Stone::getUrlSlug () {
	return "&screen=stone";
}

std::string Stone::toString() {
	std::string result = Building::toString ();
	result += "Production: " + std::to_string ( (int)this->getProduction () ) + " per hour\n";
	return result;
}

std::unique_ptr<Building> Stone::clone() {
	return std::make_unique<Stone> ( *this );
}
