#ifndef SNOB_H
#define SNOB_H

#include <buildings/building.h>

class Snob : public Building
{
	public:
		Snob ( std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug () override;
};

#endif // SNOB_H
