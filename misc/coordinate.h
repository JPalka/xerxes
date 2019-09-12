#ifndef COORDINATE_H
#define COORDINATE_H

#include <string>
#include <sstream>
#include <math.h>

class Coordinate
{
	public:
		int x;
		int y;
		std::string continent; //Possibly think of somethink different
		float getDistance ( const Coordinate &rhs ); //Possibly make it static or sumting
		std::string toString ( );
		Coordinate ( int x = 0, int y = 0);
		Coordinate ( std::string coords );
		int getContinent();
};

#endif // COORDINATE_H
