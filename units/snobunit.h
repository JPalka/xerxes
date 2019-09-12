#ifndef SNOB_UNIT_H
#define SNOB_UNIT_H

#include <units/infantry.h>

class SnobUnit : public Infantry
{
	public:
		SnobUnit( std::string htmlData, WorldSettings *worldSettings );
};

#endif // SNOB_UNIT_H
