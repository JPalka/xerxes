#include "utils.h"

#include <regex>
#include <chrono>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <string>

long Utils::decodeBuildDate ( std::string dateString ) {
	// Now converting this stupid date format into timestamp
	// Example "today at 14:17:03" "tomorrow at 14:17:03" "on 30.08. at 01:03:35"
	using namespace std::chrono;
	std::regex regHour ( "(?:(?:([01]?\\d|2[0-3]):)?([0-5]?\\d):)?([0-5]?\\d)" ); //Stolen from SO, matches HH:MM:SS time format
	std::smatch matchHour;
	std::regex_search ( dateString, matchHour, regHour );

	std::regex regDate ( ".+?(?= at)" ); // Matches everything before " at"
	std::smatch matchDate;
	std::regex_search ( dateString, matchDate, regDate );

	//Convert completion hour to seconds after midnight
	std::tm t;
	std::istringstream ss( matchHour[0] );
	ss >> std::get_time(&t, "%H:%M:%S");
	int secondsAfterMidnight = t.tm_hour * 3600 + t.tm_min * 60 + t.tm_sec;
	long int timestamp = 0;
	//Convert "today" or "tomorrow" to seconds after 1970
	if ( matchDate[0].str() == "today" ) {
		std::time_t tt;
		std::time ( &tt );
		std::tm t;
		t = *std::gmtime ( &tt );
		//Reset hours/minutes/seconds
		t.tm_hour = 0;
		t.tm_min = 0;
		t.tm_sec = 0;
		std::time_t tempTime = mktime ( &t );
		timestamp = duration_cast<seconds> ( system_clock::from_time_t ( tempTime ).time_since_epoch () ).count();
	}
	else {
		//TODO: remove code duplication like normal human being
		if ( matchDate[0].str() == "tomorrow" ) {
			std::time_t tt;
			std::time ( &tt );
			std::tm t;
			t = *std::gmtime ( &tt );
			//Add one day
			t.tm_mday += 1;
			//Reset hours/minutes/seconds
			t.tm_hour = 0;
			t.tm_min = 0;
			t.tm_sec = 0;
			std::time_t tempTime = mktime ( &t );
			timestamp = duration_cast<seconds> ( system_clock::from_time_t ( tempTime ).time_since_epoch () ).count();
		}
		else {
			std::cout << "later construction dates not supported. yet" << std::endl;
			throw "Ugabuga";
		}
	}
	timestamp += secondsAfterMidnight;
	return timestamp;
}

std::string Utils::timestampToString ( long int timestamp ) {
	std::time_t time = timestamp;
//	std::stringstream ss;
//	ss << std::ctime ( &time );
	std::string result ( std::ctime (&time) );
	result.pop_back (); //Remove last character that happens to be '\n';
//	result = ss.str();
	return result;
}

long int Utils::getTimestamp() {
	using namespace std::chrono;
	return duration_cast<seconds> ( system_clock::now ().time_since_epoch () ).count();
}

std::string Utils::extractGameDataJson ( std::string htmlData ) {
	std::regex regGameData ( "TribalWars\\.updateGameData\\((.*)\\)" );
	std::smatch matchGameData;
	std::string gameDataJson = "";
	if ( std::regex_search ( htmlData, matchGameData, regGameData ) ) {
		gameDataJson = matchGameData.str(1);
	};

	return gameDataJson;
}
std::string Utils::extractBuildingMainJson ( std::string htmlData ) {
	std::regex reg ( "BuildingMain\\.buildings = \\{(.*)\\}" );
	std::regex remove ( "BuildingMain\\.buildings = " );
	std::smatch match;
	std::regex_search ( htmlData, match, reg );
	std::string buildingInfoJson = std::regex_replace ( match[0].str(), remove, "" );
	return buildingInfoJson;
}


void Utils::printText(std::string text) {
	std::cout << text << std::endl;
}

//Snatched from StackOverflow. TODO: check for potential pitfalls
int Utils::countFileLines ( std::istream &is ) {
	// skip when bad
	if( is.bad() ) return 0;
	// save state
	std::istream::iostate state_backup = is.rdstate();
	// clear state
	is.clear();
	std::istream::streampos pos_backup = is.tellg();

	is.seekg(0);
	size_t line_cnt;
	size_t lf_cnt = std::count(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(), '\n');
	line_cnt = lf_cnt;
	// if the file is not end with '\n' , then line_cnt should plus 1
	is.unget();
	if( is.get() != '\n' ) { ++line_cnt ; }

	// recover state
	is.clear() ; // previous reading may set eofbit
	is.seekg(pos_backup);
	is.setstate(state_backup);

	return line_cnt;
}
