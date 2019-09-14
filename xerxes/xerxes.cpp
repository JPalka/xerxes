#include "xerxes.h"
#include <game/game.h>
#include <xerxes/ui.h>
#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include <functional>
#include <thread>
#include <exceptions/wrongcredentials.h>
#include <exceptions/worldinactive.h>
#include <exceptions/insufficentresources.h>
#include "misc/army.h"

Xerxes::Xerxes() {
	this->logFile.open ("xerxes.log");
}

void Xerxes::initializeGame ( std::string baseServerLink ) {
	this->gameInstance = std::make_unique<Game> ( baseServerLink,
	std::bind( &Xerxes::logString, this, std::placeholders::_1 ),
	std::bind( &Xerxes::logProgressBar, this, std::placeholders::_1, std::placeholders::_2 )
	);
}

void Xerxes::logString ( std::string text ) {
	this->logFile << text;
	this->interface.printMessage ( text );
}

void Xerxes::logProgressBar ( int count, int total ) {
	this->interface.printProgressBar ( count, total );
}

void Xerxes::start () {
	this->initializeGame ( "https://www.tribalwars.net/" ); // Hardcoded .net for now
	this->interface.printMessage ( "Initialized game instance\n" );

	//Select world
	std::string worldId = "";
	worldId = this->interface.createWorldSelectionMenu ( this->gameInstance->getWorldList () );
	this->interface.printMessage ( "Initializing world " + worldId + "\n");
	this->gameInstance->initialize ( worldId );
	while ( true ) {
		try {
//			this->interface.printMessage ( "Username: "+ this->credentials.first + "\n");
//			this->interface.printMessage ( "Password: "+ this->credentials.second + "\n");
			// If credentials are incomplete just throw WrongCredentials right away.
			if ( this->credentials.first.empty () || this->credentials.second.empty () ) {
				WrongCredentials ex ( "", "", "" );
				throw ex;
			}
			this->gameInstance->setAccount ( this->credentials.first, this->credentials.second );
			break;
		}
		catch ( WrongCredentials &ex ) {
			this->logString ( ex.what () );
			this->logString ( "\n" );
			int action = this->interface.createLoginForm ( this->credentials );
			if ( action == UI::Action::CANCEL_ACTION ) {
				this->logString ( "Cannot continue without logging in.\n" );
				return;
			}
		}
		catch ( WorldInactive &ex ) {
			this->logString ( ex.what () + ex.worldName );
			break;
		}
	}
	this->logString ( "Finished initialization. Press any (just be reasonable) key to start.\n");
	timeout (-1);
	getch();
	timeout (0);
	Player *player = this->gameInstance->currentWorld->findPlayer ( this->gameInstance->activeAccount.playerId );
	std::thread bwordThread ( &Xerxes::runBword, this );
	this->interface.createVillageOverview ( this->gameInstance->currentWorld->findVillages ( *player ).front () );
	this->stopSignal = 1; //Stop b-word loop
	bwordThread.join ();
}

void Xerxes::runBword() {
	//MAIN B-WORD LOOP THAT WILL PERFORM ACTIONS AND WHATNOTS
	Player *player = this->gameInstance->currentWorld->findPlayer ( this->gameInstance->activeAccount.playerId );
	std::vector <Village*> villages = this->gameInstance->currentWorld->findVillages ( *player );
	//Test creation of army
	Army testArmy;
	testArmy.setHome ( villages[0] );
	testArmy.addUnit ( "spear", 20 );
	testArmy.addUnit ( "sword", 10 );
//	Storage &storageRef = static_cast<Storage&> (villages[0]->getBuilding ("storage"));
	while ( !this->stopSignal ) {
		sleep(5);
		this->logString ( "Army stats: \n" );
		this->logString ( testArmy.toString () );
		this->gameInstance->currentWorld->refresh ();
//		if ( villages[0]->getBuilding ("smith") )
//		Resources res { 15000, 15000, 15000 };
//		try {
//			storageRef.substractResources ( res );
//			this->logString ( "Substracted 15k of resources\n" );
//		}
//		catch ( InsufficentResources &ex ) {
//			this->logString ( "Not enough resources\n" );
//		}
//		this->logString ( "Upgrading storage\n" );
//		static_cast<Headquarters&> (villages[0]->getBuilding (("main"))).upgradeBuilding ("storage");
	}
}

void Xerxes::setCredentials ( std::pair<std::string, std::string> newCredentials ) {
	this->credentials = newCredentials;
}
void Xerxes::setCredentials ( std::string username, std::string password ) {
	this->credentials.first = username;
	this->credentials.second = password;
}
