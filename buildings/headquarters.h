#ifndef HEADQUARTERS_H
#define HEADQUARTERS_H

#include <buildings/building.h>
#include <orders/buildorder.h>


class Headquarters : public Building {
	public:

		Headquarters ( std::string xmlData, WorldSettings *worldSettings );
		Headquarters () = default;
		virtual std::string getUrlSlug() override;
		virtual std::unique_ptr<Building> clone () override;
		virtual Headquarters &parseData ( std::string ) override;
//		virtual std::string toString() override;
		bool upgradeBuilding ( std::string buildingName );
};

#endif // HEADQUARTERS_H
