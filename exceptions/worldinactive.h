#ifndef WORLDINACTIVE_H
#define WORLDINACTIVE_H

#include <exceptions/gameexception.h>

class WorldInactive : GameException {
	public:
		std::string worldName;
		WorldInactive ( std::string worldName )
		:	worldName(worldName) {

		}
		virtual const char *what() const noexcept override {
			return "World is not joinable for whatever reason.";
		}
};

#endif // WORLDINACTIVE_H
