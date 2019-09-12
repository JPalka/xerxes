#include "client.h"

Client::Client() {
	//setup default request headers
	this->headersList.push_back ("Content-Type: application/x-www-form-urlencoded");
	this->headersList.push_back ("Accept: application/json, text/javascript, */*; q=0.01");
	this->headersList.push_back ("User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0");
//		header.push_back ("Referer: https://www.plemiona.pl/");
	this->headersList.push_back ("X-Requested-With: XMLHttpRequest");
}

std::string Client::getUrl ( std::string url, std::string cookiesFilePath ) {
	std::string result;
	this->easyCurl.setOpt<curlpp::options::HttpHeader> ( this->headersList );
	this->easyCurl.setOpt<curlpp::options::Verbose> ( false );
	this->easyCurl.setOpt<curlpp::options::Url> ( url );
	this->easyCurl.setOpt<curlpp::options::FollowLocation> ( true );
	if ( cookiesFilePath.length () > 0 ) {
		this->easyCurl.setOpt ( new curlpp::options::CookieJar ( cookiesFilePath ) );
		this->easyCurl.setOpt ( new curlpp::options::CookieFile ( cookiesFilePath ) );
	}
	std::stringstream os;
	this->easyCurl.setOpt ( new curlpp::options::WriteStream( &os ) );
	this->easyCurl.perform ();
	if ( cookiesFilePath.length () > 0 ) {
		this->easyCurl.setOpt ( new curlpp::options::CookieJar ( cookiesFilePath ) );
		this->easyCurl.setOpt ( new curlpp::options::CookieFile ( cookiesFilePath ) );
	}
//	std::cout << curlpp::infos::CookieList::get ( this->easyCurl ) << std::endl;
	if ( curlpp::infos::ResponseCode::get ( this->easyCurl ) != 200 ) {
		throw "Response code is not 200. Sumting Wong: " + curlpp::infos::ResponseCode::get ( this->easyCurl );
	}
	result = os.str();
//	this->easyCurl.reset ();
	return result;
}

std::string Client::postUrl ( std::string url, std::string postData , std::string cookiesFilePath ) {
	std::string result;
	this->easyCurl.setOpt<curlpp::options::HttpHeader> ( this->headersList );
	this->easyCurl.setOpt<curlpp::options::Verbose> ( false );
	this->easyCurl.setOpt<curlpp::options::Url> ( url );
	this->easyCurl.setOpt<curlpp::options::FollowLocation> ( true );
	if ( cookiesFilePath.length () > 0 ) {
		this->easyCurl.setOpt ( new curlpp::options::CookieJar ( cookiesFilePath ) );
		this->easyCurl.setOpt ( new curlpp::options::CookieFile ( cookiesFilePath ) );
	}
	this->easyCurl.setOpt(new curlpp::options::PostFields( postData ));
	this->easyCurl.setOpt(new curlpp::options::PostFieldSize ( postData.size () ) );
	std::stringstream os;
	this->easyCurl.setOpt ( new curlpp::options::WriteStream( &os ) );
	this->easyCurl.perform ();
	if ( cookiesFilePath.length () > 0 ) {
		this->easyCurl.setOpt ( new curlpp::options::CookieJar ( cookiesFilePath ) );
		this->easyCurl.setOpt ( new curlpp::options::CookieFile ( cookiesFilePath ) );
	}
	if ( curlpp::infos::ResponseCode::get ( this->easyCurl ) != 200 ) {
		throw "Response code is not 200. Sumting Wong: " + curlpp::infos::ResponseCode::get ( this->easyCurl );
	}
	result = os.str();
//	this->easyCurl.reset ();
	return result;
}

