#include "coordinate.h"

float Coordinate::getDistance ( const Coordinate &rhs ) {
	float result;
	result = std::sqrt (std::pow (std::abs (this->x - rhs.x), 2) + std::pow (std::abs (this->y - rhs.y), 2));
	return result;
}

std::string Coordinate::toString () {
	std::stringstream ss;
	ss << this->x << "|" << this->y;
	std::string result;
	ss >> result;
	return result;
}

Coordinate::Coordinate ( int x, int y ) : x(x), y(y) {

}

Coordinate::Coordinate ( std::string coords ) {
	std::stringstream ss;
	ss << coords;
	int x, y;
	ss >> x;
	ss.ignore ( 1 );
	ss >> y;
	this->x = x;
	this->y = y;
}

int Coordinate::getContinent () {
	return (y / 100) * 10 + ( x / 100 );
}
