#include "smith.h"

Smith::Smith (std::string xmlData , WorldSettings *worldSettings) : Building(xmlData, worldSettings)
{

}

std::string Smith::getUrlSlug () {
	return "&screen=smith";
}
