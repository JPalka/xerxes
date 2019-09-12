#ifndef CAVALRY_H
#define CAVALRY_H

#include <units/unit.h>

class Cavalry : public Unit
{
	public:
		Cavalry ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // CAVALRY_H
