#include "watchtower.h"

Watchtower::Watchtower ( std::string xmlData, WorldSettings *worldSettings ) : Building ( xmlData, worldSettings ) {

}

std::string Watchtower::getUrlSlug () {
	return "&screen=watchtower";
}
