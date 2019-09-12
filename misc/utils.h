#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>


//Dumping ground for functions that do not belong anywhere yet.
namespace Utils {
		// Converting stupid date format into timestamp
		// Example "today at 14:17:03" "tomorrow at 14:17:03" "on 30.08. at 01:03:35"
	long int decodeBuildDate ( std::string dateString );
	std::string timestampToString ( long timestamp );
	long int getTimestamp();
	std::string extractGameDataJson ( std::string htmlData );
	std::string extractBuildingMainJson ( std::string htmlData );
	void printText ( std::string text );
	int countFileLines ( std::istream &is );
}

#endif // UTILS_H
