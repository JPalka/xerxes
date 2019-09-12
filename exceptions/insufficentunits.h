#ifndef INSUFFICENTUNITS_H
#define INSUFFICENTUNITS_H

#include <exceptions/gameexception.h>

class InsufficentUnits : GameException {
	public:
		InsufficentUnits ( ) {

		}
		virtual const char *what() const noexcept override {
			return "Not enough units";
		}
};

#endif // INSUFFICENTUNITS_H
