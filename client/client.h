#ifndef CLIENT_H
#define CLIENT_H

//#include <game/account.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <regex>


/*
 * Handles http requests to tribalwars server
 */
class Client {
		curlpp::Cleanup cleanup;
		curlpp::Easy easyCurl;
		std::list<std::string> headersList;

	public:
		Client();
		std::string getUrl ( std::string url , std::string cookiesFilePath = "");
		std::string postUrl ( std::string url, std::string postData, std::string cookiesFilePath = "");
		std::string getFile ( std::string	url );
};

#endif // CLIENT_H
