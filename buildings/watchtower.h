#ifndef WATCHTOWER_H
#define WATCHTOWER_H

#include <buildings/building.h>

class Watchtower : public Building
{
	public:
		Watchtower ( std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug () override;
};

#endif // WATCHTOWER_H
