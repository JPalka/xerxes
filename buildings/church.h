#ifndef CHURCH_H
#define CHURCH_H

#include <buildings/building.h>

class Church : public Building
{
	public:
		Church ( std::string xmlData, WorldSettings *worldSettings );
		virtual std::string getUrlSlug ( ) override;
};

#endif // CHURCH_H
