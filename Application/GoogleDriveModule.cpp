#include "pch.h"

#include "GoogleDriveModule.h"
#include <format>
#include <boost/format.hpp>

//{"installed":{"client_id":"436850445452-d7e3ao235vf2tddf7e7snem6f91h6v3b.apps.googleusercontent.com","project_id":"fs-54291","auth_uri":"https://accounts.google.com/o/oauth2/auth","token_uri":"https://oauth2.googleapis.com/token","auth_provider_x509_cert_url":"https://www.googleapis.com/oauth2/v1/certs","client_secret":"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","redirect_uris":["urn:ietf:wg:oauth:2.0:oob","http://localhost"]}}
//{"installed":{"client_id":"436850445452-dlodnklip33tn4bcc8qq65i58usnu4u8.apps.googleusercontent.com", "project_id" : "fs-54291", "auth_uri" : "https://accounts.google.com/o/oauth2/auth", "token_uri" : "https://oauth2.googleapis.com/token", "auth_provider_x509_cert_url" : "https://www.googleapis.com/oauth4/v1/certs", "client_secret" : "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", "redirect_uris" : ["urn:ietf:wg:oauth:2.0:oob", "http://localhost"] }}

GoogleDriveModule::GoogleDriveModule()
{    
	//m_curl = curl_easy_init();
}

void GoogleDriveModule::LogIn()
{
	ShellExecute(NULL, L"open",
		L"https://accounts.google.com/o/oauth2/auth?scope=https://www.googleapis.com/auth/drive&redirect_uri=https://developers.google.com/oauthplayground&response_type=code&client_id=436850445452-vgn3r1ac6i6o4i68amnsd9qhjk9gam9d.apps.googleusercontent.com",
		NULL, NULL, SW_SHOW);//*/
    //auto s = Get("ip-api.com", "/json/");
    //auto s = Get("https://accounts.google.com/o/oauth2/auth", "?scope=https://www.googleapis.com/auth/drive&redirect_uri=https://developers.google.com/oauthplayground&response_type=code&client_id=436850445452-vgn3r1ac6i6o4i68amnsd9qhjk9gam9d.apps.googleusercontent.com");
    return;
}

void GoogleDriveModule::GetAccessToken(std::string code)
{
    const std::string str = "?code=%s&client_id=436850445452-vgn3r1ac6i6o4i68amnsd9qhjk9gam9d.apps.googleusercontent.com&client_secret=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&redirect_uri=https://developers.google.com/oauthplayground&grant_type=authorization_code";
    boost::format fmt = boost::format(str) % code;
    auto result = Get("https://www.googleapis.com/oauth2/v3/token", boost::str(fmt));
}

std::string GoogleDriveModule::Get(std::string host, std::string target)
{
    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::socket socket(ioc);

    boost::asio::connect(socket, resolver.resolve("http://www.google.com", "80"));

    http::request<http::string_body> req(http::verb::get, target, 11);
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(socket, req);

    boost::beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);
    std::string result = boost::beast::buffers_to_string(res.body().data());

    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    return result;
}

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree; 
using boost::property_tree::read_json; 
using boost::property_tree::write_json;

std::string GoogleDriveModule::Post(std::string host, std::string target)
{
    boost::asio::io_context ioc;
    tcp::resolver resolver(ioc);
    tcp::resolver::query query(host);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(ioc);
    boost::asio::connect(socket, endpoint_iterator);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);

    ptree root, info;
    root.put("code", "4/0AX4XfWhWuIThMXcJkqYMvPHgDOfGgyCuDr995Yy6I9SMb5c32_xMWT5MFjLpOFUCmdeIkQ");
    root.put("client_id", "436850445452-vgn3r1ac6i6o4i68amnsd9qhjk9gam9d.apps.googleusercontent.com");
    info.put("client_secret", "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");
    info.put("redirect_uri", "https://developers.google.com/oauthplayground");
    info.put("grant_type", "authorization_code");

    std::ostringstream buf;
    write_json(buf, root, false);
    std::string json = buf.str();

    request_stream << "POST /title/ HTTP/1.1 \r\n";
    request_stream << "Host:" << host << "\r\n";
    request_stream << "User-Agent: C/1.0";
    request_stream << "Content-Type: application/json; charset=utf-8 \r\n";
    request_stream << json << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    boost::asio::write(socket, request);

    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    return "";

}


