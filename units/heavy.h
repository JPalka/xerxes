#ifndef HEAVY_H
#define HEAVY_H

#include <units/cavalry.h>

class Heavy : public Cavalry
{
	public:
		Heavy (std::string htmlData, WorldSettings *worldSettings);
};

#endif // HEAVY_H
