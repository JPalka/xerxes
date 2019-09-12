#ifndef SMITH_H
#define SMITH_H

#include <buildings/building.h>

class Smith : public Building
{
	public:
		Smith(std::string xmlData, WorldSettings *worldSettings);
		virtual std::string getUrlSlug () override;
};

#endif // SMITH_H
