#ifndef PLACE_H
#define PLACE_H

#include <buildings/building.h>
#include <units/unitfactory.h>
class Unit;
class Place : public Building
{
	public:
		std::vector<std::unique_ptr<Unit>> units;
		Place ( std::string xmlData, WorldSettings *worldSettings );
		Place () = default;
		Place ( const Place &source );
		virtual std::string getUrlSlug () override;
		virtual std::unique_ptr<Building> clone () override;
		virtual Place &parseData ( std::string htmlData ) override;
		virtual std::string toString() override;
		Unit *getUnit ( std::string unitName );
		int getUnitsPopulation (); //Gets population taken by troops in a rally point
		class ScavengingModule {
			public:
				bool enabled;
				Place *parentPlace;
				bool sendScavengeParty (std::vector<std::pair<std::string, int> > units, int scavengingLevel );
				bool parseData ( std::string htmlData );
				ScavengingModule ( Place *parentPlace );
				struct ScavengingLevel {
					int id;
					std::string name;
					double lootFactor;
					Resources unlockCost;
					int unlockDurationSeconds;
					double durationExponent = 0.45;
					int durationInitialSeconds = 1800;
					double durationFactor;
					bool unlocked = false;
				}scavengingLevels[4];
		} scavengingModule;
};

#endif // PLACE_H
