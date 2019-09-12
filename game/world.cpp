#include "world.h"

#include <misc/utils.h>
#include <game/game.h>
World::World() {

}

World::World ( std::string worldId, std::string worldLink )
: id(worldId), link ( worldLink ), name("World name") {
	Game::logCallback ( "Initializing world settings\n" );
	std::fstream worldSettingsFile ( this->id + "-worldSettings.xml",
	std::ios_base::in | std::ios_base::out | std::ios_base::binary );
	if ( !worldSettingsFile.good() ) {
		Game::logCallback ( "Cannot find WorldSettings file : " + this->id + "-worldSettings.xml - downloading\n" );
		this->downloadWorldSettings ();
		worldSettingsFile.open ( this->id + "-worldSettings.xml", std::ios_base::in | std::ios_base::binary );
	}
	if ( worldSettingsFile.is_open () ) {
		std::string worldSettingsString;
		std::stringstream ss;
		ss << worldSettingsFile.rdbuf ();
		worldSettingsString = ss.str();
		this->settings = WorldSettings ( worldSettingsString );
		this->settings.world = this;
		worldSettingsFile.close ();
	}
	else {
		Game::logCallback ("Cannot open worldSettings file.\n");
		throw "WUTWUT";
	}


	Game::logCallback ("Initializing unit info\n");
	// Blablabla
	std::fstream unitInfoFile ( this->id + "-unitInfo.xml", std::ios_base::in );
	if ( !unitInfoFile.good() ) {
		Game::logCallback ( "Cannot find UnitInfo file: " + this->id + "-unitInfo.xml - downloading\n" );
		this->downloadUnitInfo ();
		unitInfoFile.open ( this->id + "-unitInfo.xml", std::ios_base::in );
	}
	if ( unitInfoFile.is_open () ) {
		std::stringstream ss;
		ss << unitInfoFile.rdbuf ();
		std::string unitInfoString = ss.str();
		this->settings.unitInfoXmlString = unitInfoString;
	}
	else {
		Game::logCallback ( "Cannot open UnitInfo file. Preposterous.\n" );
		throw "yolo";
	}

	Game::logCallback ("Initializing buildings\n");
	//Check if buildingInfo file exists bla, bla bla...
	std::fstream buildingInfoFile ( this->id + "-buildingInfo.xml", std::ios_base::in );
	if ( !buildingInfoFile.good() ) {
		Game::logCallback ( "Cannot find BuildingInfo file: " + this->id + "-buildingInfo.xml - downloading\n");
		this->downloadBuildingInfo ();
		buildingInfoFile.open ( this->id + "-buildingInfo.xml", std::ios_base::in );
	}
	if ( buildingInfoFile.is_open () ) {
		std::string buildingInfoString;
		std::stringstream ss;
		ss << buildingInfoFile.rdbuf ();
		buildingInfoString = ss.str();
		this->buildings = BuildingFactory::getAvailableBuildings ( buildingInfoString, &this->settings );
		this->setBuildingOrderHandler ();
		Game::logCallback ( "Building count: " + std::to_string (this->buildings.size ()) + "\n" );
	}
	else {
		Game::logCallback ("Cannot open BuildingInfo file. Preposterous.\n");
		throw "yolo";
	}

	Game::logCallback ("Initializing player list\n");
	//Check if playerList file exists for given world. If not then download one.
	std::fstream playerListFile ( this->id + "-playerList.txt", std::ios_base::in | std::ios_base::out );
	if ( !playerListFile.good() ) {
		Game::logCallback ( "Cannot find PlayerList file : " + this->id + "-playerList.txt - downloading\n" );
		this->downloadPlayerList ();
		playerListFile.open ( this->id + "-playerList.txt", std::ios_base::in );
	}
	//Fill world player list from file
	if ( playerListFile.is_open () ) {
		std::string playerString;
		while ( std::getline ( playerListFile, playerString ) ) {
			std::vector<std::string> arr;
			boost::algorithm::split ( arr, playerString, boost::is_any_of (",") );
			Player player (	std::stoul ( arr[0] ), arr[1] );
			this->players.push_back ( player );
		}
		//Add Barbarian player. it will have id 0 and serve as placeholder owner for barbarian and bonus villages
		Player barbarian ( 0, "Barbarian" );
		this->players.push_back ( barbarian );
		this->players.shrink_to_fit ();
	}
	else {
		Game::logCallback ("Cannot open PlayerList file. Unacceptable.\n");
		throw "yolo";
	}
	playerListFile.close ();
	Game::logCallback ("Player count: " + std::to_string (this->players.size ()) + "\n");
	//Check if villageList file exists for given world. If not then download one.
	std::fstream villageListFile ( this->id + "-villageList.txt", std::ios_base::in );
	if ( !villageListFile.good() ) {
		Game::logCallback ( "Cannot find VillageList file: " + this->id + "-villageList.txt - downloading\n");
		this->downloadVillageList ();
		villageListFile.open ( this->id + "-villageList.txt", std::ios_base::in );
	}
	if ( villageListFile.is_open () ) {
		Game::logCallback ("Initalizing Villages: ");
		std::string villageString;
		int villageCount = Utils::countFileLines ( villageListFile );
		int i = 1;
		while ( std::getline ( villageListFile, villageString ) ) {
			std::vector<std::string> arr;
			boost::algorithm::split ( arr, villageString, boost::is_any_of (",") );
			Player &playerRef = *findPlayer ( std::stoul ( arr[4] ) );
			Coordinate coords ( std::stoi ( arr[2] ), std::stoi ( arr[3] ) );
			Village village ( std::stoul ( arr[0] ), arr[1], coords, playerRef, this->buildings );
			this->villages.push_back ( village );
			//update progressbar on 1st, last and every 1% of villages
			if ( (i == 1) || (i == villageCount) || ((i % (int)(std::ceil(villageCount/100.0)) ) == 0) ) {
				Game::logProgressBar ( i, villageCount );
			}
			i++;
		}
		this->villages.shrink_to_fit ();
	}
	else {
		Game::logCallback ("Cannot open VillageList file. Unacceptable.\n");
		throw "yolo";
	}

	Game::logCallback ("Villages count: " + std::to_string (this->villages.size ()) + "\n");
}

bool World::downloadPlayerList () {
	Client client;
	std::string result = client.getUrl ( this->link + "/map/player.txt.gz" );
	std::stringstream ss;
	ss << result;
	boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
	in.push ( boost::iostreams::gzip_decompressor() );
	in.push ( ss );
	std::ofstream playerListFile ( this->id + "-playerList.txt",
	std::ios_base::out | std::ios_base::binary | std::ios::trunc );
	boost::iostreams::copy(in, playerListFile);
	playerListFile.close ();
	return true;
}

bool World::downloadVillageList () {
	Client client;
	std::string result = client.getUrl ( this->link + "/map/village.txt.gz" );
	std::stringstream ss;
	ss << result;
	boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
	in.push ( boost::iostreams::gzip_decompressor() );
	in.push ( ss );
	std::ofstream villageListFile ( this->id + "-villageList.txt",
	std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );
	boost::iostreams::copy(in, villageListFile);
	villageListFile.close ();
	return true;
}

bool World::downloadWorldSettings () {
	Client client;
	std::string result = client.getUrl ( this->link + "/interface.php?func=get_config" );
	std::ofstream worldSettingsFile ( this->id + "-worldSettings.xml", std::ios_base::out | std::ios_base::trunc );
	worldSettingsFile << result;
	worldSettingsFile.close ();
	return true;
}

bool World::downloadBuildingInfo () {
	Client client;
	std::string result = client.getUrl ( this->link + "/interface.php?func=get_building_info" );
	std::ofstream buildingInfoFile ( this->id + "-buildingInfo.xml", std::ios_base::out | std::ios_base::trunc );
	buildingInfoFile << result;
	buildingInfoFile.close ();
	return true;
}

bool World::downloadUnitInfo () {
	Client client;
	std::string result = client.getUrl ( this->link + "/interface.php?func=get_unit_info" );
	std::ofstream buildingInfoFile ( this->id + "-unitInfo.xml", std::ios_base::out | std::ios_base::trunc );
	buildingInfoFile << result;
	buildingInfoFile.close ();
	return true;
}

// Gets worlds playerlist from localfile
std::string World::getPlayerList ( std::string fileName ) {
	return fileName;
}

Player *World::findPlayer( unsigned long int playerId ) {
	for ( size_t i = 0; i < this->players.size (); i++ ) {
		if ( this->players[i].id == playerId ) {
			return &this->players[i];
		}
	}
	Game::logCallback ("Cannot find player with id: " + std::to_string (playerId) + "\n");
	throw "another yolo exception. must get my act together.\n";
}
Player *World::findPlayer( std::string playerName ) {
	for ( size_t i = 0; i < this->players.size (); i++ ) {
		if ( this->players[i].name == playerName ) {
			return &this->players[i];
		}
	}
	Game::logCallback ("Cannot find player with name: " + playerName + "\n");
	throw "another yolo exception. must get my act together.\n";
}


std::string World::getLink () {
	return this->link;
}

Village *World::findVillage ( unsigned int villageId ) {
	for ( size_t i = 0; i < this->villages.size (); i++ ) {
		if ( this->villages[i].id == villageId ) {
			return &this->villages[i];
		}
	}
	return nullptr;
}

std::vector<Village *> World::findVillages ( Player &owner ) {
	std::vector<Village*> result;
	for ( size_t i = 0; i < this->villages.size (); i++ ) {
		if ( this->villages[i].owner->id == owner.id ) {
			result.push_back ( &this->villages[i] );
		}
	}
	return result;
}

World &World::setBuildingOrderHandler () {
	//Set listener for each building ( same one that is )
	for ( auto &building : this->buildings ) {
//		building->addOrder = [this] ( std::unique_ptr<Order> newOrder ) {
//			this->ordersQueue.push ( std::move(newOrder) );
//			return true;
//		};
		building->addOrder = std::bind( &OrderProcessor::addOrder, &this->orderProcessor, std::placeholders::_1 );
		building->getOrders = std::bind ( &OrderProcessor::getOrders, &this->orderProcessor, std::placeholders::_1 );
	}
	return *this;
}

World &World::refresh() {
	long int newTime = Utils::getTimestamp ();
	long int maxStep = this->getMaxStep ();

	while (true) {
		if ( maxStep == -1 || maxStep >= newTime ) {
			this->stepTime ( newTime );
			//We have stepped to current time. exit
			break;
		}
		else {
			//Step by max step and recalculate step
			this->stepTime ( maxStep );
			maxStep = this->getMaxStep ();
		}
	}
	return *this;
}

World &World::stepTime ( long int newTime ) {
	//Recalculate resource incomes for villages
	for ( Village &village : this->villages ) {
		village.calculateResources ( newTime );
	}
	this->orderProcessor.processOrders ( newTime );
//	if ( !this->ordersQueue.empty () && ( this->ordersQueue.top()->dueTime == newTime ) ) {
//		//Makeshift processing of Order
//		this->ordersQueue.top ()->execute ();
//		this->ordersQueue.pop (); // Yeet finished order
//	}
	return *this;
}

long int World::getMaxStep () {
	if ( this->orderProcessor.empty () ) {
		return -1; //if there are no orders due then maximum time step is unlimited
	}
	long int earliestOrderTimestamp = this->orderProcessor.top ().dueTime;
	return earliestOrderTimestamp;
}

bool operator< ( const std::unique_ptr<Order> &lhs, const std::unique_ptr<Order> &rhs ) {
	return *lhs < *rhs;
}
bool operator> ( const std::unique_ptr<Order> &lhs, const std::unique_ptr<Order> &rhs ) {
	return *lhs > *rhs;
}
