#include "garage.h"

Garage::Garage (std::string xmlData, WorldSettings *worldSettings) : Building(xmlData, worldSettings){

}

std::string Garage::getUrlSlug () {
	return "&screen=garage";
}
