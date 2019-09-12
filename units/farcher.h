#ifndef FARCHER_H
#define FARCHER_H

#include <units/archer.h>

class Farcher : public Archer
{
	public:
		Farcher ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // FARCHER_H
