#include "resources.h"
#include <iostream> //TEMP

Resources &Resources::operator- ( const Resources &rhs ) {
	this->wood -= rhs.wood;
	this->stone -= rhs.stone;
	this->iron -= rhs.iron;
	return *this;
}
//TODO: change it to return new Resources object
Resources &Resources::operator+ ( const Resources &rhs ) {
	this->wood += rhs.wood;
	this->stone += rhs.stone;
	this->iron += rhs.iron;
	return *this;
}

Resources &Resources::operator+= ( const Resources &rhs ) {
	this->wood += rhs.wood;
	this->stone += rhs.stone;
	this->iron += rhs.iron;
	return *this;
}

bool Resources::operator== ( const Resources &rhs ) {
	if ( this->wood != rhs.wood )
		return false;
	if ( this->stone != rhs.stone )
		return false;
	if ( this->iron != rhs.iron )
		return false;
	return true;
}

bool Resources::operator>= ( const Resources &rhs ) {
	if ( this->wood < rhs.wood )
		return false;
	if ( this->stone < rhs.stone )
		return false;
	if ( this->iron < rhs.iron )
		return false;
	return true;
}

bool Resources::operator> ( const Resources &rhs ) {
	if ( this->wood <= rhs.wood ) {
		return false;
	}
	if ( this->stone <= rhs.stone ) {
		return false;
	}
	if ( this->iron <= rhs.iron ) {
		return false;
	}
	return true;
}

Resources Resources::operator* ( long rhs ) {
	Resources result ( this->wood * rhs, this->stone * rhs, this->iron * rhs );
	return result;
}

Resources::Resources () {

}
Resources::Resources ( double wood, double stone, double iron ): wood(wood), stone(stone), iron(iron) {

}

std::string Resources::toString () {
	std::string result = "";
	result += "Wood:" + std::to_string ((int)this->wood) + " Stone:" + std::to_string ((int)this->stone) + " Iron:" + std::to_string ((int)this->iron);
	return result;
}
//TEMPORARY << OVERLOAD FOR DEBADGERING PURPOSES.
std::ostream &operator<< ( std::ostream &os, Resources &rhs ) {
	os << "Wood: " << rhs.wood << std::endl;
	os << "Stone: " << rhs.stone << std::endl;
	os << "Iron: " << rhs.iron << std::endl;
	return os;
}
