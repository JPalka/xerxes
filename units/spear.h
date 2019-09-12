#ifndef SPEAR_H
#define SPEAR_H

#include <units/infantry.h>

class Spear : public Infantry
{
	public:
		Spear ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // SPEAR_H
