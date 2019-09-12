#ifndef HIDE_H
#define HIDE_H

#include <buildings/building.h>

class Hide : public Building
{
	public:
		Hide(std::string xmlData, WorldSettings *worldSettings);
		virtual std::string getUrlSlug () override;
};

#endif // HIDE_H
