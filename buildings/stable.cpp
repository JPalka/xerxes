#include "stable.h"

Stable::Stable (std::string xmlData, WorldSettings *worldSettings) : Building ( xmlData, worldSettings ) {

}

std::string Stable::getUrlSlug () {
	return "&screen=stable";
}
