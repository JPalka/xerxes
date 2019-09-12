#ifndef INSUFFICENTRESOURCES_H
#define INSUFFICENTRESOURCES_H

#include <exceptions/gameexception.h>

class InsufficentResources : GameException {
	public:
		InsufficentResources ( ) {

		}
		virtual const char *what() const noexcept override {
			return "Not enough resources";
		}
};

#endif // INSUFFICENTRESOURCES_H
