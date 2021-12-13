#include "pch.h"

#include "GoogleDriveModule.h"

//{"installed":{"client_id":"436850445452-d7e3ao235vf2tddf7e7snem6f91h6v3b.apps.googleusercontent.com","project_id":"fs-54291","auth_uri":"https://accounts.google.com/o/oauth2/auth","token_uri":"https://oauth2.googleapis.com/token","auth_provider_x509_cert_url":"https://www.googleapis.com/oauth2/v1/certs","client_secret":"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","redirect_uris":["urn:ietf:wg:oauth:2.0:oob","http://localhost"]}}
//{"installed":{"client_id":"436850445452-dlodnklip33tn4bcc8qq65i58usnu4u8.apps.googleusercontent.com", "project_id" : "fs-54291", "auth_uri" : "https://accounts.google.com/o/oauth2/auth", "token_uri" : "https://oauth2.googleapis.com/token", "auth_provider_x509_cert_url" : "https://www.googleapis.com/oauth4/v1/certs", "client_secret" : "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", "redirect_uris" : ["urn:ietf:wg:oauth:2.0:oob", "http://localhost"] }}

GoogleDriveModule::GoogleDriveModule()
{    
	//m_curl = curl_easy_init();
}

void GoogleDriveModule::LogIn()
{
	//boost::asio::ip::tcp::iostream stream;
	
/*	ShellExecute(NULL, L"open",
		L"https://accounts.google.com/o/oauth2/auth?scope={https://www.googleapis.com/auth/drive}&redirect_uri={https://localhost/index.html}&response_type=code&client_id={436850445452-dlodnklip33tn4bcc8qq65i58usnu4u8.apps.googleusercontent.com}",
		NULL, NULL, SW_SHOW);//*/

    
    const std::string host = "scooterlabs.com";
    const std::string target = "/echo?input=test";

    // I/O контекст, необходимый для всех I/O операций
    boost::asio::io_context ioc;

    // Resolver для определения endpoint'ов
    boost::asio::ip::tcp::resolver resolver(ioc);
    // Tcp сокет, использующейся для соединения
    boost::asio::ip::tcp::socket socket(ioc);

    // Резолвим адрес и устанавливаем соединение
    boost::asio::connect(socket, resolver.resolve(host, "80"));

    // Дальше необходимо создать HTTP GET реквест с указанием таргета
    http::request<http::string_body> req(http::verb::get, target, 11);
    // Задаём поля HTTP заголовка
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Отправляем реквест через приконекченный сокет
    http::write(socket, req);

    // Часть, отвечающая за чтение респонса
    {
        boost::beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(socket, buffer, res);

        //std::cout << res << std::endl;
    }
    // Закрываем соединение
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    //*/
    return;
}


