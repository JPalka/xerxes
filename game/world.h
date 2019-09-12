#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <queue>
#include <functional>

#include <game/player.h>
#include <game/village.h>
#include <game/worldsettings.h>
#include <game/orderprocessor.h>
//#include <buildings/building.h>
#include <buildings/buildingfactory.h>
#include <units/unitfactory.h>
#include <orders/order.h>

#include <client/client.h>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/algorithm/string.hpp>

class World {
	public:
		std::string id;
		std::string link;
		std::string name;
//		std::string playerListFile = "";
		std::vector<Player> players;
		std::vector<Village> villages;
		std::vector<std::unique_ptr<Building>> buildings; //Default building set for every village.
		OrderProcessor orderProcessor;
		WorldSettings settings;
		int tribes;//PLACEHOLDER for vector<Tribe>
		World ();
		World ( std::string worldId, std::string worldLink );
		bool downloadPlayerList ();
		bool downloadVillageList ();
		bool downloadWorldSettings ();
		bool downloadBuildingInfo ();
		bool downloadUnitInfo ();
		std::string getPlayerList ( std::string fileName );
		Player *findPlayer ( unsigned long playerId );
		Player *findPlayer ( std::string playerName );
		std::string getLink ();
		Village *findVillage ( unsigned int villageId );
		std::vector<Village*> findVillages ( Player &owner ); //Get villages owned by specific player
//		bool addOrder ( Order &newOrder );
		std::priority_queue<std::unique_ptr<Order>, std::vector<std::unique_ptr<Order>>, Order::OrderByDueTime> ordersQueue;
		World &setBuildingOrderHandler ();
		//Delete copy constructor so it tells me when its used
		World ( const World& ) = delete;
		//Force creation of move assignment operator until I decide how to handle copying/moving World objects
		World &operator= ( World&& ) = default;

		World &refresh ( ); //Refresh simulation to current time
		long int getMaxStep(); //Gets maximum time step forward game can make. its based on timestamp of Order on top of queue
		World &stepTime ( long newTime ); // Steps world to specified time
};

#endif // WORLD_H
