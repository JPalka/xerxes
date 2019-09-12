#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <fstream>
#include <regex>

#include <game/village.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/view_or_value.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/exception/exception.hpp>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

#include <client/client.h>

/*
 * Contains all account specific information. Login/password also items/flags/premium points
 */
class Player;
class World;
class Account
{
	public:
		std::string username;
		std::string password;
		std::string cookiesFile;
		std::string csrf;
		unsigned long int playerId;
		//Tries to log into game, sets cookies/tokens or whatever
		bool login (std::string baseLink , std::string worldId);
		Account ( std::string username = "dummyName", std::string password = "dummyPass" );
		std::string getVillageInfo ( World &world, int villageId );
		std::string getPlayerInfo ( World &world, int villageId );
		std::string getBuildingInfo ( World &world, int villageId, std::string buildingName, std::string mode = "" );
		//Returns id of recruit order. TODO: change it so it returns response body like upgradeBuilding
		int recruitUnit ( World &world, int villageId, std::string unitName, int count );
		//Returns response body
		std::string upgradeBuilding ( World &world, int villageId, std::string buildingName );
		std::string startScavenging(World &world, int villageId, std::vector<std::pair<std::string, int> > units, int option, int carryMax);
};

#endif // ACCOUNT_H
