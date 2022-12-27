#include "Server.hpp"

int Server::processListen(std::string value)
{
    std::cout << "processListen" << value << std::endl;
    return (0);
}

int Server::processServerName(std::string value)
{
    std::cout << "processServerName" << value << std::endl;
    return (0);
}

int Server::processRoot(std::string value)
{
    std::cout << "processRoot" << value << std::endl;
    return (0);
}

int Server::processIndex(std::string value)
{
    std::cout << "processIndex" << value << std::endl;
    return (0);
}

int Server::processErrorPage(std::string value)
{
    std::cout << "processErrorPage" << value << std::endl;
    return (0);
}

int Server::processClientMaxBodySize(std::string value)
{
    std::cout << "processClientMaxBodySize" << value << std::endl;
    return (0);
}

int Server::processLocation(std::string value)
{
    std::cout << "processLocation" << value << std::endl;
    return (0);
}

Server::Server()
{
    this->propertyMap["listen"] = &Server::processListen;
    this->propertyMap["server_name"] = &Server::processServerName;
    this->propertyMap["root"] = &Server::processRoot;
    this->propertyMap["index"] = &Server::processIndex;
    this->propertyMap["error_page"] = &Server::processErrorPage;
    this->propertyMap["client_max_body_size"] =
        &Server::processClientMaxBodySize;
    this->propertyMap["location"] = &Server::processLocation;

    this->mandatoryProperties.push_back("listen");
    this->mandatoryProperties.push_back("server_name");
    this->mandatoryProperties.push_back("root");
    this->mandatoryProperties.push_back("index");
}

Server::~Server() {}

Server::Server(const Server &) {}

Server &Server::operator=(const Server &) { return (*this); }
