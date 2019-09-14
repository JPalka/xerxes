#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>

class Resources
{
	public:
		double wood;
		double stone;
		double iron;
		float woodIncome;
		float stoneIncome;
		float ironIncome;
		Resources &operator- ( const Resources &rhs );
		Resources &operator+ ( const Resources &rhs );
		Resources &operator+= ( const Resources &rhs );
		bool operator== ( const Resources &rhs );
		bool operator>= ( const Resources &rhs );
		bool operator> ( const Resources &rhs );
		Resources operator* ( long int rhs );
		Resources();
		Resources ( double wood, double stone, double iron ) ;
		std::string toString();
};

#endif // RESOURCES_H
