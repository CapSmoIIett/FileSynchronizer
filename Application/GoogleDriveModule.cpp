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
		L"https://accounts.google.com/o/oauth2/auth"
        L"?scope=https://www.googleapis.com/auth/drive"
        L"&redirect_uri=https://developers.google.com/oauthplayground"
        L"&response_type=code"
        L"&client_id=436850445452-vgn3r1ac6i6o4i68amnsd9qhjk9gam9d.apps.googleusercontent.com",
		NULL, NULL, SW_SHOW);//*/
    //auto s = Get("ip-api.com", "/json/");
    //auto s = Get("https://accounts.google.com/o/oauth2/auth", "?scope=https://www.googleapis.com/auth/drive&redirect_uri=https://developers.google.com/oauthplayground&response_type=code&client_id=436850445452-vgn3r1ac6i6o4i68amnsd9qhjk9gam9d.apps.googleusercontent.com");
    return;
}

void GoogleDriveModule::GetAccessToken(std::string code)
{
    const std::string str = "?code=%s"
        "&client_id=436850445452-vgn3r1ac6i6o4i68amnsd9qhjk9gam9d.apps.googleusercontent.com"
        "&client_secret=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"
        "&redirect_uri=https://developers.google.com/oauthplayground&grant_type=authorization_code";
    boost::format fmt = boost::format(str) % code;
    auto result = Get("https://oauth2.googleapis.com/token", boost::str(fmt));
}

std::string GoogleDriveModule::Get(std::string host, std::string target)
{
    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::socket socket(ioc);

    boost::asio::connect(socket, resolver.resolve(host, "80"));

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

    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::socket socket(ioc);

    auto const results = resolver.resolve(host, "80");

    // Make the connection on the IP address we get from a lookup
    boost::asio::connect(socket, results.begin(), results.end());

    //boost::asio::connect(socket, resolver.resolve(host, "80"));

    http::request<http::string_body> req(http::verb::post, target, 11);
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


