#ifndef SPY_H
#define SPY_H

#include <units/unit.h>

class Spy : public Unit
{
	public:
		Spy ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // SPY_H
