#ifndef MILITIA_H
#define MILITIA_H

#include <units/infantry.h>

class Militia : public Infantry
{
	public:
		Militia ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // MILITIA_H
