#ifndef LIGHT_H
#define LIGHT_H

#include <units/cavalry.h>

class Light : public Cavalry
{
	public:
		Light ( std::string htmlData, WorldSettings *worldSettings );
};

#endif // LIGHT_H
