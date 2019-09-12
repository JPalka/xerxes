#ifndef UNITFACTORY_H
#define UNITFACTORY_H

#include <memory>

#include <units/unit.h>
#include <units/archer.h>
#include <units/axe.h>
#include <units/catapult.h>
#include <units/cavalry.h>
#include <units/farcher.h>
#include <units/heavy.h>
#include <units/light.h>
#include <units/marcher.h>
#include <units/militia.h>
#include <units/paladin.h>
#include <units/ram.h>
#include <units/spear.h>
#include <units/spy.h>
#include <units/sword.h>
#include <units/snobunit.h>

class WorldSettings;
class UnitFactory {
	public:
		UnitFactory();
		static std::unique_ptr<Unit> createUnit (std::string name, std::string xmlData , WorldSettings *worldSettings);
};

#endif // UNITFACTORY_H
