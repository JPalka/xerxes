#ifndef WORLDSETTINGS_H
#define WORLDSETTINGS_H

#include <string>
#include <iostream>

#include <libxml++/libxml++.h>

class World;
class WorldSettings {
	public:
		double speed;
		double unitSpeed;
		int morale;
		bool canDestroyBuildings;
		int commandCancelTime;
		int tradeCancelTime;
		int knight;
		bool archers;
		bool watchtower;
		bool stronghold;
		bool church;
		bool night;
		int nightStartHour;
		int nightEndHour;
		double nightDefFactor;
		int noobProtectDays;
		int ratioDays;
		int ratio;
		std::string unitInfoXmlString;
		int hauls;
		World *world; // Possibly temporary
		WorldSettings ();
		WorldSettings ( std::string xml );
};

#endif // WORLDSETTINGS_H
