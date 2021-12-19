#include "pch.h"
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include "TCPConnection.h"

size_t read_complete(char* buff, const boost::system::error_code& err, size_t bytes);

io_service service;

ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

//using namespace std;
//ip::tcp::socket* sock;

bool TCPConnection::OpenConnection()
{
	return true;
}

bool TCPConnection::Connect(std::string ip)
{
	return true;
}

void TCPConnection::SendStr(std::string str)
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr.s_addr);
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	
	send(newConnection, str.c_str(), str.size() + 1, NULL);
}

void TCPConnection::SendFile(WFDFile file)
{
	CT2CA pszConvertedAnsiString(file.GetOneString().GetBuffer());
	std::string str(pszConvertedAnsiString);
	
	SendStr(str);
}

void TCPConnection::SendAllFiles(std::vector<WFDFile> fileList)
{
	for (auto file : fileList)
		SendFile(file);
	SendFile(WFDFile());
}


std::string TCPConnection::GetStr()
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr.s_addr);
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	connect(Connection, (SOCKADDR*)&addr, sizeof(addr));
	char msg[256];
	recv(Connection, msg, sizeof(msg), NULL);

	std::string str(msg);

	return str;
}

WFDFile TCPConnection::GetFile()
{
	WFDFile file;

	std::string str = GetStr();

	file.GetFromWString(CString(str.c_str()));

	return file;
}

std::vector<WFDFile> TCPConnection::GetAllFiles()
{
	std::vector<WFDFile> answer;
	while (true)
	{
		WFDFile file = GetFile();
		if (file.fullName == "" &&
			file.name == "" &&
			file.type == "" &&
			file.size == "" &&
			file.date == "" &&
			file.attr == "")
			break;

		answer.push_back(file);
	}

	return answer;
}



size_t read_complete(char* buff, const boost::system::error_code& err, size_t bytes)
{
	if (err) return 0;
	bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
	return found ? 0 : 1;
}
