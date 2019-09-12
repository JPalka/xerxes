#include "hide.h"

Hide::Hide (std::string xmlData, WorldSettings *worldSettings) : Building (xmlData, worldSettings) {

}

std::string Hide::getUrlSlug () {
	return "&screen=hide";
}
