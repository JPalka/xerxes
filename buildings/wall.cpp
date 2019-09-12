#include "wall.h"

Wall::Wall (std::string xmlData, WorldSettings *worldSettings) : Building (xmlData, worldSettings) {

}

std::string Wall::getUrlSlug () {
	return "&screen=wall";
}
