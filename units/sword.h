#ifndef SWORD_H
#define SWORD_H

#include <units/infantry.h>

class Sword : public Infantry
{
	public:
		Sword ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // SWORD_H
