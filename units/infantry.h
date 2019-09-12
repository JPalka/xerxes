#ifndef INFANTRY_H
#define INFANTRY_H

#include <units/unit.h>
class WorldSettings;
class Infantry : public Unit
{
	public:
		Infantry ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // INFANTRY_H
