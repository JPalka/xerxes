#ifndef ARCHER_H
#define ARCHER_H

#include <units/unit.h>

class WorldInfo;
class Archer : public Unit
{
	public:
		Archer ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // ARCHER_H
