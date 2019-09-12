#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <game/village.h>

class Account;
class Player {
	public:
		unsigned long int id;
		std::string name; //Urlencoded
		std::string tribe; //TEMPORARY type
		//Think what to do about it, does Player store references to his villages or
		//just have village store its owner OR BOTH
//		std::vector<Village*> villages;
		Account *ownerAccount = nullptr;
		Player ();
		Player ( unsigned long int id, std::string name );
};

#endif // PLAYER_H
