#ifndef FARM_H
#define FARM_H

#include <buildings/building.h>

class Farm : public Building
{
	public:
		int currentPopulation;
		Farm(std::string xmlData, WorldSettings *worldSettings);
		virtual std::string getUrlSlug () override;
		virtual Farm &parseData ( std::string htmlData );
		virtual std::unique_ptr<Building> clone () override;
		virtual std::string toString () override;
		int getMaxPopulation ();
		int getCurrentPopulation ();
};

#endif // FARM_H
