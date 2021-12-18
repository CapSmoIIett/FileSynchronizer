#pragma once

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>

//#include <boost/asio/ssl>

//#include <boost/python.hpp>

using tcp = boost::asio::ip::tcp;   
namespace ssl = boost::asio::ssl;      
namespace http = boost::beast::http;


class GoogleDriveModule
{
public:
	GoogleDriveModule();
	static void LogIn();
	static void GetAccessToken(std::string code);
	
	static  std::string Get(std::string host, std::string target);
	static  std::string Post(std::string host, std::string target);
};

