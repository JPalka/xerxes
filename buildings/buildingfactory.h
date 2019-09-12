#ifndef BUILDINGFACTORY_H
#define BUILDINGFACTORY_H

#include <buildings/building.h>
#include <buildings/barracks.h>
#include <buildings/church.h>
#include <buildings/farm.h>
#include <buildings/garage.h>
#include <buildings/headquarters.h>
#include <buildings/hide.h>
#include <buildings/iron.h>
#include <buildings/market.h>
#include <buildings/place.h>
#include <buildings/smith.h>
#include <buildings/snob.h>
#include <buildings/stable.h>
#include <buildings/statue.h>
#include <buildings/stone.h>
#include <buildings/storage.h>
#include <buildings/wall.h>
#include <buildings/watchtower.h>
#include <buildings/wood.h>
#include <buildings/university.h>

#include <regex>
#include <iostream>

#include <memory>
#include <vector>

class WorldSettings;
class BuildingFactory {
	public:
		BuildingFactory();
		static std::unique_ptr<Building> createBuilding ( std::string name, std::string xmlData, WorldSettings *worldSettings );
		static std::vector<std::unique_ptr<Building>> getAvailableBuildings ( std::string xmlData, WorldSettings *WorldSettings );
};

#endif // BUILDINGFACTORY_H
