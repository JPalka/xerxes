#ifndef PALADIN_H
#define PALADIN_H

#include <units/infantry.h>

class Paladin : public Infantry
{
	public:
		Paladin ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // PALADIN_H
