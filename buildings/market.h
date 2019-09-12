#ifndef MARKET_H
#define MARKET_H

#include <buildings/building.h>

class Market : public Building
{
	public:
		Market(std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug () override;
		virtual std::unique_ptr<Building> clone() override;
		int getTotalMerchants ();
};

#endif // MARKET_H
