#ifndef WRONGCREDENTIALS_H
#define WRONGCREDENTIALS_H

#include <exceptions/gameexception.h>

class WrongCredentials : GameException {
		std::string username, password, servername;
	public:
		WrongCredentials ( std::string username,std::string password, std::string servername )
		:	username(username), password(password), servername(servername) {

		}
		virtual const char *what() const noexcept override {
			return "Wrong username/password.";
		}
};

#endif // WRONGCREDENTIALS_H

