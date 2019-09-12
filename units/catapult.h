#ifndef CATAPULT_H
#define CATAPULT_H

#include <units/infantry.h>

class Catapult : public Infantry
{
	public:
		Catapult( std::string htmlData, WorldSettings *worldSettings );
};

#endif // CATAPULT_H
