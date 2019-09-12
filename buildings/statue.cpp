#include "statue.h"

Statue::Statue (std::string xmlData, WorldSettings *worldSettings) : Building(xmlData, worldSettings) {

}

std::string Statue::getUrlSlug () {
	return "&screen=statue";
}
