#pragma once

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


#include "WFDFile.h"

using namespace boost::asio;

class TCPConnection
{
public: 
	static bool OpenConnection();

	static bool Connect(std::string ip);
 
	static void SendAllFiles(std::vector<WFDFile> fileList);
	static void SendFile(WFDFile file);

	static std::vector<WFDFile> GetAllFiles();
	static WFDFile GetFile();

	static std::string GetStr();
	static void SendStr(std::string);
};

