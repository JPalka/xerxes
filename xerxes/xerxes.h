#ifndef XERXES_H
#define XERXES_H

#include <memory>
#include <xerxes/ui.h>
#include <fstream>

//Class that does all the automated majiggers and whatnots
class Game;
class Xerxes {
	private:
		UI interface;
		std::pair<std::string, std::string> credentials; // username,password pair
		std::unique_ptr<Game> gameInstance;
		void initializeGame ( std::string baseServerLink );
		void createLoginForm();
		std::ofstream logFile;
		int stopSignal = 0; //Ugly thing to be able to terminate thread from other thread.
	public:
		void start();
		void runBword ();
		void setCredentials ( std::pair<std::string, std::string> newCredentials );
		void setCredentials ( std::string username, std::string password );
		Xerxes();
		void logString ( std::string text );
		void logProgressBar ( int count, int total );
};

#endif // XERXES_H
