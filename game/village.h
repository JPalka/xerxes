#ifndef VILLAGE_H
#define VILLAGE_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <fstream>

#include <buildings/building.h>
#include <misc/coordinate.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/view_or_value.hpp>
#include <bsoncxx/stdx/optional.hpp>

class Player;

class Village {
	public:
		unsigned long int id;
		std::string name;
		Coordinate location;
		Player *owner;
		std::vector<std::unique_ptr<Building>> buildings;
		//Allows accessing building with villageObj["buildingName"] syntax
		Building &operator[] ( std::string buildingName );
		Village ( unsigned long int id, std::string name, Coordinate location, Player &owner, std::vector<std::unique_ptr<Building>> &buildings );
		Village ( const Village &rhs );
		Village &setBuildings ( const std::vector<std::unique_ptr<Building>> &newBuildings );
		long int lastUpdated;
		std::string getUrlSlug();
		bool updateBuildingsLevels ( std::string jsonData );
		std::string toString();
		Village &calculateResources ( long int newTime ); // Calculates resources up to passed timestamp
		Building &getBuilding ( std::string buildingName );
};

#endif // VILLAGE_H
