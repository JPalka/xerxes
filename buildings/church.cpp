#include "church.h"

Church::Church (std::string xmlData , WorldSettings *worldSettings) : Building(xmlData, worldSettings) {

}

std::string Church::getUrlSlug () {
	return "&screen=church";
}
