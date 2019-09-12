#include "snob.h"

Snob::Snob (std::string xmlData , WorldSettings *worldSettings) : Building ( xmlData, worldSettings ) {

}

std::string Snob::getUrlSlug () {
	return "&screen=snob";
}
