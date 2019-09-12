#ifndef GAME_H
#define GAME_H

#include <string>
#include <game/world.h>
#include <game/worldlist.h>
#include <game/account.h>
#include <game/onlineclient.h>
#include <misc/utils.h>
#include <client/client.h>
#include <memory>
#include <functional>

class Game {
	private:
		WorldList worldList;
	public:
		Account activeAccount;
		std::unique_ptr<World> currentWorld;
		static std::function<void(std::string)> logCallback;
//		static std::function<void(std::string)> logCallbackl; //With new line
		static std::function<void(int, int)> logProgressBar;
		std::string baseServer;
		Game &initialize (std::string worldId);
		Game (std::string baseServer = "https://www.tribalwars.net/" ,
		std::function<void(std::string)> logCallback = Utils::printText,
		std::function<void(int, int)> logProgressBar = nullptr ); //TODO: add generic Utils::printProgressBar
		Game &setAccount ( std::string username, std::string password );
		std::map<std::string, std::string> getWorldList ();
};

#endif // GAME_H
