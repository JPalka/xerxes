#ifndef WOOD_H
#define WOOD_H

#include <buildings/building.h>

class Wood : public Building
{
	public:
		Wood ( std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug () override;
		virtual std::string toString () override;
		std::unique_ptr<Building> clone() override;
		double getProduction (); //Gets resource production per hour
};

#endif // WOOD_H
