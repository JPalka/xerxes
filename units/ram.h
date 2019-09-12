#ifndef RAM_H
#define RAM_H

#include <units/infantry.h>

class Ram : public Infantry
{
	public:
		Ram ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // RAM_H
