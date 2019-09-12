#ifndef WORLDLIST_H
#define WORLDLIST_H

#include <string>
#include <map>
#include <regex>
#include <iostream>


class WorldList
{
	public:
		std::map<std::string, std::string> worldLinks;
		std::string linkSlug = "/backend/get_servers.php";
		WorldList ( std::string baseServer );
};

#endif // WORLDLIST_H
