#ifndef STONE_H
#define STONE_H

#include <buildings/building.h>

class Stone : public Building
{
	public:
		Stone ( std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug () override;
		virtual std::string toString () override;
		double getProduction();
		virtual std::unique_ptr<Building> clone() override;
};

#endif // STONE_H
