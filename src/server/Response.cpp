#include "Utils.hpp"
#include <Response.hpp>
#include <fstream>
#include <string>

Response::Response(Request *request)
    : request(request), server(request->getServer())
{
}

Response::Response() : server(empty) {}

Response::~Response() {}

Response &Response::operator=(const Response &other)
{
    this->request = other.request;
    return *this;
}

Response::Response(const Response &other)
    : request(other.request), server(other.request->getServer())
{
}

std::string Response::getErrorPage()
{
    std::string   message;
    std::string   body;
    std::string   line;
    std::ifstream file;

    if (!this->server.getErrorPage().empty())
    {
        file.open(this->server.getErrorPage());
        if (!file.is_open())
            file.open("./http/error.html");
    }
    else
        file.open("./http/error.html");

    if (!file.is_open())
    {
        std::cout << "Error opening error page" << std::endl;
        return "";
    }

    while (std::getline(file, line))
        body += line;

    message = "HTTP/1.1 404 Not Found\r\n";
    message += "Content-Type: text/html\n";
    message += "Content-Length: ";
    message += std::to_string(body.length());
    message += "\n\n";
    message += body;

    return message;
}

std::string Response::getResponse()
{
    std::string message = "HTTP/1.1 200 OK\r\n";
    std::string fileName =
        this->server.getResponseFile(this->request->getRoute());
    std::string body;
    std::string line;

    std::cout << "Asking for route: " << this->request->getRoute() << std::endl;

    std::cout << "File to serve: " << std::endl << fileName << std::endl;

    if (fileName.empty())
        return getErrorPage();

    std::ifstream file(fileName);

    while (std::getline(file, line))
        body += line;

    if (!file.is_open())
        return getErrorPage();

    message += "Content-Type: ";
    message += ft::getMimeType(fileName);
    message += "\n";
    message += "Content-Length: ";
    // message += std::to_string(this->request->getBody().length());
    message += std::to_string(body.length());
    message += "\n\n";
    // message += this->request->getBody();
    message += body;

    return message;
}
