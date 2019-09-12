#ifndef AXE_H
#define AXE_H

#include <units/infantry.h>

class Axe : public Infantry
{
	public:
		Axe ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // AXE_H
