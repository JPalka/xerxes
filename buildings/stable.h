#ifndef STABLE_H
#define STABLE_H

#include <buildings/building.h>

class Stable : public Building
{
	public:
		Stable(std::string xmlData, WorldSettings *worldSettings);
		virtual std::string getUrlSlug () override;
};

#endif // STABLE_H
