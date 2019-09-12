#ifndef MARCHER_H
#define MARCHER_H

#include <units/archer.h>

class Marcher : public Archer
{
	public:
		Marcher ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // MARCHER_H
