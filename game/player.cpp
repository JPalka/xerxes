#include "player.h"

Player::Player ( ) {

}

Player::Player ( unsigned long id, std::string name ) : id(id), name(name) {
	this->tribe = "Wololo";
}
