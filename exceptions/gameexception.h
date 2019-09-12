#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <exception>
#include <string>

class GameException : public std::exception {
	public:
		std::string text;
		GameException ( std::string text = "") : text(text) {
		}
		virtual const char *what() const noexcept override {
			return "Base GameException";
		}
};
#endif // GAMEEXCEPTION_H
