#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <memory>
#include <iostream>
#include <cmath>
#include <regex>
#include <chrono>
#include <sstream>
#include <locale>
#include <iomanip>
#include <ctime>
#include <functional>

#include <misc/resources.h>

#include <libxml++/libxml++.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/HTMLparser.h>

class Order;
class WorldSettings;
class Village;
class Building {
	public:
		std::string name;
		std::function<bool(std::unique_ptr<Order>)> addOrder;
		std::function<std::vector<Order*>(Building&)> getOrders;
		WorldSettings *worldSettings; // Needed to know world speed/other BS thats depending on world the building is in
		Village *parentVillage;
		int level;
		int maxLevel;
		int minLevel;
		Resources baseCost;
		int basePop;
		double woodFactor;
		double stoneFactor;
		double ironFactor;
		float popFactor;
		int buildTime;
		float buildTimeFactor;
		virtual Building &Upgrade ();
		virtual Building &Degrade ();
		virtual std::unique_ptr<Building> clone ();
		Building ( std::string xmlData, WorldSettings *worldSettings );
		Building () = default;
		Resources getUpgradeCost ();
		int getUpgradePopulationCost (); // Gets population needed to upgrade building
		int getPopulation ( int levelOffset = 0 ); // Gets population used by the building
		int getUpgradeTime ( int headquartersLevel );
		virtual bool canUpgrade (); // Check if building can be upgraded
		virtual std::string getUrlSlug ( ) ;
		virtual Building &setLevel ( int newLevel );
		virtual Building &parseData ( std::string htmlData );
		virtual std::string toString ();
		// Returns building level including lvl under construction
		int getLevel ( bool includePending = false );
		Building(const Building &source);
		bool operator== ( const Building &rhs );
		int lastUpdated;
};

#endif // BUILDING_H
