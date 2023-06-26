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
std::string Response::addDate(std::string message)
{
    std::time_t currentTime = std::time(nullptr);
    std::string stringTime = std::ctime(&currentTime);
    std::stringstream ss(stringTime);
    std::string token;
    std::vector<std::string> tokens;
    while(ss >> token)
        tokens.push_back(token);
    message += tokens[0] + ", " + tokens[2] + " " + tokens[1] + " " + tokens[4] + " " + tokens[3] + " GMT";
    return message;
}
std::string Response::getErrorPage(std::string code)
{
    std::string   message;
    std::string   body;
    std::string   line;
    std::ifstream file;

    (void)code;

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
    message += std::to_string(body.length()) +"\n";
    message += "Date: ";
    message= addDate(message);
    message += "\n\n";
    message += body;

    return message;
}
bool Response::fileEdition(int flag)
{
    std::ofstream file;
    std::string route;
    std::string::size_type  idx =this->request->getRoute().find_last_of('/');
    route = this->server.getFileEnd() + "/" + this->request->getRoute().substr(idx+1);
    /*POST*/
    if (flag == 1)
    {
        file.open(route);
        if (!file.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return false;
        }
        file << this->request->getBody();
        file.close();
    }
    /*DELETE*/
    else
    {
       int result = std::remove(route.c_str());
       if (result != 0)
       {
           std::cout << "Error deleting file" << std::endl;
           return false;
       }
       else
           std::cout << "File successfully deleted" << std::endl;
    }
   return true;
}

std::string Response::getResponse()
{
    std::string message = "HTTP/1.1 200 OK\r\n";
    if (this->request->getMethod().compare("POST")==0)
        fileEdition(1);
    if (this->request->getMethod().compare("DELETE")==0)
        fileEdition(0);

    std::string fileName =
        this->server.getResponseFile(this->request->getRoute());
    std::string body;
    std::string line;

    std::cout << "Asking for route: " << this->request->getRoute() << std::endl;

    std::cout << "File to serve: " << fileName << std::endl;

    std::cout << "Method: " << this->request->getMethod() << std::endl;

    std::cout << "Is method allowed: "
              << this->server.isAllowedMethod(this->request->getMethod())
              << std::endl;

    // TODO ERROR PAGE 405
    if (!this->server.isAllowedMethod(this->request->getMethod()))
        return getErrorPage("405");

    // TODO ERROR PAGE 404
    if (fileName.empty())
        return getErrorPage("404");

    std::ifstream file(fileName);

    // TODO ERROR PAGE 404
    if (!file.is_open())
        return getErrorPage("404");

    while (std::getline(file, line))
        body += line;

    message += "Content-Type: ";
    message += ft::getMimeType(fileName);
    message += "\n";
    message += "Content-Length: ";
    message += std::to_string(body.length());
    message += "Date: ";
    message= addDate(message);
    message += "\n\n";
    message += body;
    return message;
}
