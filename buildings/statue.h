#ifndef STATUE_H
#define STATUE_H

#include <buildings/building.h>

class Statue : public Building
{
	public:
		Statue(std::string xmlData, WorldSettings *worldSettings);
		virtual std::string getUrlSlug () override;
};

#endif // STATUE_H
