#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include <buildings/building.h>

class University : public Building
{
	public:
		University ( std::string xmlData, WorldSettings *worldSettings );
};

#endif // UNIVERSITY_H
