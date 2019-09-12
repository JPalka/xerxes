#ifndef BARRACKS_H
#define BARRACKS_H

#include <buildings/building.h>

class Barracks : public Building
{
	public:
		Barracks ( std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug ( ) override;
//		virtual std::string toString () override;
		Barracks &parseData ( std::string htmlData );
		virtual std::unique_ptr<Building> clone () override;
		int getRecruitTime ( std::string unitName );
		bool recruitUnit ( std::string unitName, int count );
};

#endif // BARRACKS_H
