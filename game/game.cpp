#include "game.h"

#include <misc/utils.h>
#include <functional>

std::function<void(std::string)> Game::logCallback = Utils::printText;
std::function<void(int, int)> Game::logProgressBar = nullptr;

Game &Game::initialize ( std::string worldId ) {
	std::map<std::string, std::string>::iterator iter = this->worldList.worldLinks.find ( worldId );
	if ( iter == this->worldList.worldLinks.end () ) {
		logCallback ( "Cannot find world: " + worldId + "\n" );
		return *this;
	}
	Game::logCallback ( "Initializing world: " + worldId + "\n" );
//	logCallback ( "World link: " + iter->second );
	std::unique_ptr<World> tempWorld ( new World ( worldId, iter->second ) );
	this->currentWorld = std::move ( tempWorld );
	Game::logCallback ( "DONE\n" );
//	this->currentWorld.setBuildingOrderHandler ();
	return *this;
}

Game::Game ( std::string baseServer, std::function<void(std::string)> logCallback, std::function<void(int,int)> logProgressBar )
: worldList(baseServer) {
	Game::logCallback = logCallback;
	Game::logProgressBar = logProgressBar;
}

Game &Game::setAccount ( std::string username, std::string password ) {
	this->activeAccount = Account ( username, password );
	//log into account
	this->activeAccount.login ( this->currentWorld->getLink (), this->currentWorld->id );
	//Now assign active account to player object in current world
	Player *currentPlayer = this->currentWorld->findPlayer ( this->activeAccount.playerId );
	currentPlayer->ownerAccount = &(this->activeAccount);

	this->logCallback ( "Synchronizing village state\n" );
	std::vector<Village*> currentPlayerVillages = this->currentWorld->findVillages ( *currentPlayer );
	//Next do things that wont be necessary when I get to persisting all objects in database
	//Get state of buildings/levels/all that jazz for current player villages
	for ( Village *village : currentPlayerVillages ) {
		std::string buildingInfo = this->activeAccount.getBuildingInfo ( *this->currentWorld, village->id, "main" ); //HQ building
		std::string buildingInfoJson = Utils::extractBuildingMainJson ( buildingInfo );
//		//Gets GameData json from html. Passing buildingInfo because GameData can be extracted from (almost?) any page accessed while logged in
//		std::string gameDataJson = Utils::extractGameDataJson ( buildingInfo );
		village->updateBuildingsLevels ( buildingInfoJson );
		this->logCallback ( "Updated building levels.\n" );
		this->logCallback ( "Updating buildings: HQ, " );
		village->getBuilding ("main").parseData ( buildingInfo );
		//Storage can parse its info from pretty much every page because its info is in top bar.
		this->logCallback ( "Warehouse, " );
		village->getBuilding ("storage").parseData ( buildingInfo );
		//Same goes for farm
		this->logCallback ( "Farm, " );
		village->getBuilding ("farm").parseData ( buildingInfo );
		this->logCallback ( "Rally point, " );
		std::string placeInfo = this->activeAccount.getBuildingInfo ( *this->currentWorld, village->id, "place", "units" );
		village->getBuilding ("place").parseData ( placeInfo );
		std::string placeScavengingInfo = this->activeAccount.getBuildingInfo ( *this->currentWorld, village->id, "place", "scavenge");
		static_cast<Place&> (village->getBuilding ("place")).scavengingModule.parseData ( placeScavengingInfo );
		this->logCallback ( "Barracks.\n" );
		std::string barracksInfo = this->activeAccount.getBuildingInfo ( *this->currentWorld, village->id, "barracks" );
		village->getBuilding ("barracks").parseData ( barracksInfo );
	}

	return *this;
}

std::map<std::string, std::string> Game::getWorldList() {
	return this->worldList.worldLinks;
}
