#ifndef WALL_H
#define WALL_H

#include <buildings/building.h>

class Wall : public Building
{
	public:
		Wall(std::string xmlData, WorldSettings *worldSettings);
		virtual std::string getUrlSlug () override;
};

#endif // WALL_H
