#include "worldlist.h"
#include <client/client.h>

WorldList::WorldList ( std::string baseServer ) {
	Client client;
	std::string result = client.getUrl ( baseServer + linkSlug );
//	std::cout << result;
	std::regex reg ( "\"(.*?)\"" );
	std::sregex_iterator iter ( result.begin (), result.end (), reg );
	std::sregex_iterator end;
	while ( iter != end ) {
		std::pair<std::string, std::string> pair;
		pair.first = (*iter)[1];
		iter++;
		pair.second = (*iter)[1];
		this->worldLinks.insert ( pair );
		iter++;
	}
}
