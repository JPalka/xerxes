#ifndef GARAGE_H
#define GARAGE_H

#include <buildings/building.h>

class Garage : public Building
{
	public:
		Garage ( std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug () override;
};

#endif // GARAGE_H
