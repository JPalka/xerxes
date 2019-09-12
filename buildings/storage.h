#ifndef STORAGE_H
#define STORAGE_H

#include <buildings/building.h>

class Storage : public Building
{
	private:
		Resources storage;
	public:
		Storage(std::string xmlData, WorldSettings *worldSettings);
		virtual std::string getUrlSlug () override;
		Resources getCapacity();
		Resources getStorage();
		virtual Storage &parseData ( std::string ) override;
		virtual std::unique_ptr<Building> clone () override;
		Storage &addResources(const Resources &rhs);
		Storage &substractResources(const Resources &rhs);
};

#endif // STORAGE_H
