#include "market.h"

Market::Market (std::string xmlData , WorldSettings *worldSettings) : Building ( xmlData, worldSettings ) {

}

std::string Market::getUrlSlug () {
	return "&screen=market&mode=transports";
}

std::unique_ptr<Building> Market::clone() {
	return std::make_unique<Market> ( *this );
}

int Market::getTotalMerchants () {
	//Until lvl 11 of market amount of merchants==building level
	if ( this->level <= 10 ) {
		return this->level;
	}
	return std::pow ( this->level - 10, 2 ) + 10;
}
