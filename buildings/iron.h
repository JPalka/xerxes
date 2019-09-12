#ifndef IRON_H
#define IRON_H

#include <buildings/building.h>

class Iron : public Building
{
	public:
		Iron ( std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug () override;
		virtual std::string toString () override;
		virtual std::unique_ptr<Building> clone() override;
		double getProduction();
};

#endif // IRON_H
