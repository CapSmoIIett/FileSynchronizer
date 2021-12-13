#pragma once

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>

	namespace http = boost::beast::http;

class GoogleDriveModule
{
public:
	GoogleDriveModule();
	static void LogIn();
	void GetAccessToken();
	
	//CURL *m_curl;
	//CURLcode res;
};
