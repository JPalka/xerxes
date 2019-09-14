#ifndef ARMY_H
#define ARMY_H

#include <string>
#include <map>
#include <tuple>

class Place;
class Village;
class Resources;
class Army {
	private:
		std::map<std::string, int> units;
		Place *home;
	public:
		enum class Strength {
			GeneralOffensive, CavalryOffensive, ArcherOffensive,
			GeneralDefensive, CavalryDefensive, ArcherDefensive
		};
		Army ();
		void setHome ( Place *newHome );
		void setHome ( Village *newHome );
		int addUnit ( std::string unitName, int unitCount );
		int removeUnit ( std::string unitName, int unitCount );
		std::string toString ();
		bool hasNoble ();
		bool hasScout ();
		bool canAttack ();
		std::tuple<double, double, double> getAttack (); // Get infantry, cavalry, archer Attack respectively
		std::tuple<double, double, double> getDefence (); // Get infantry, cavalry, archer Defence respectively
		double getLoot (); // Get total carrying capacity of army
		double getSpeed (); // Get army speed in minutes/square
		Resources getCost (); // Get total cost of units in army
};

#endif // ARMY_H
