#include "Server.hpp"
#include <Fields.hpp>
#include <Utils.hpp>
#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include <netinet/ip.h>
#include <unistd.h>
#include <vector>

Server::Server()
{
    this->fields["listen"]           = new ListenField();
    this->fields["location"]         = new Location();
    this->fields["server_name"]      = new ServerNameField();
    this->fields["root"]             = new RootField();
    this->fields["error_page"]       = new ErrorPageField();
    this->fields["client_body_size"] = new ClientBodySizeField();
    this->fields["index"]            = new IndexField();
    this->fields["methods"]          = new AllowedMethodsField();
    this->fields["post_folder"]      = new LoadFolderField();
    this->next                       = NULL;
}

Server::~Server() {}

Server::Server(const Server &) {}

Server &Server::operator=(const Server &) { return (*this); }

void Server::setDefaultErrPage()
{
    ErrorPageField *defErrPage =
        dynamic_cast<ErrorPageField *>(this->fields["error_page"]);

    if (!defErrPage->isSet())
        defErrPage->setValue("./error_page/error.html");
}

bool Server::validate()
{
    if (!this->fields["listen"]->isSet())
    {
        std::cout << "Listen field is not set" << std::endl;
        return false;
    }
    if (!this->fields["error_page"]->isSet())
        this->setDefaultErrPage();

    return true;
}

bool Server::appendServer(Server *server)
{
    Server *curr = this;

    while (curr->next)
        curr = curr->next;
    curr->next = server;
    return true;
}

int Server::getPort() const
{
    return (dynamic_cast<ListenField *>(this->fields.at("listen"))->getPort());
}

std::string Server::getHost() const
{
    return (dynamic_cast<ListenField *>(this->fields.at("listen"))->getHost());
}

std::string Server::getServerName() const
{
    return (dynamic_cast<ServerNameField *>(this->fields.at("server_name"))
                ->getValue());
}

std::string Server::getRoot() const
{
    return (dynamic_cast<RootField *>(this->fields.at("root"))->getValue());
}

std::string Server::getIndex() const
{
    return (dynamic_cast<IndexField *>(this->fields.at("index"))->getValue());
}

std::string Server::getErrorPage() const
{
    return (dynamic_cast<ErrorPageField *>(this->fields.at("error_page"))
                ->getValue());
}

int Server::getClientBodySize() const
{
    return (
        dynamic_cast<ClientBodySizeField *>(this->fields.at("client_body_size"))
            ->getValue());
}

bool Server::isAllowedMethod(std::string method) const
{
    return (dynamic_cast<AllowedMethodsField *>(this->fields.at("methods"))
                ->validate(method));
}

std::ostream &operator<<(std::ostream &out, Server const &server)
{
    out << "Server: " << std::endl;
    out << "\tPort: " << server.getPort() << std::endl;
    out << "\t Host: " << server.getHost() << std::endl;
    out << "\t ServerName: " << server.getServerName() << std::endl;
    out << "\t Root: " << server.getRoot() << std::endl;
    out << "\t ErrorPage: " << server.getErrorPage() << std::endl;
    out << "\t ClientBodySize: " << server.getClientBodySize() << std::endl;
    out << std::endl;
    return (out);
}
std::string Server::getFileEnd() const
{
    return (dynamic_cast<LoadFolderField *>(this->fields.at("post_folder"))
                ->getValue());
}

std::vector<Location *> Server::findLocationsByPath(std::string path) const
{
    std::vector<Location *> res = std::vector<Location *>();
    for (size_t i = 0; i < this->locations.size(); i++)
    {
        if ((path.substr(0, this->locations[i]->getValue().length()) ==
             this->locations[i]->getValue()))
            res.push_back(this->locations[i]);
    }

    return res;
}

Location *Server::findLongestLocationByPath(std::string path) const
{
    Server   *curr            = const_cast<Server *>(this);
    size_t    longestMatch    = 0;
    Location *longestMatchLoc = NULL;

    while (curr)
    {
        for (size_t i = 0; i < curr->locations.size(); i++)
        {
            if ((path.substr(0, curr->locations[i]->getValue().length()) ==
                 curr->locations[i]->getValue()))
            {
                if (curr->locations[i]->getValue().length() > longestMatch)
                {
                    longestMatch    = curr->locations[i]->getValue().length();
                    longestMatchLoc = curr->locations[i];
                }
            }
        }
        curr = curr->next;
    }

    return longestMatchLoc;
}

std::string Server::getResponseFile(std::string route) const
{
    std::string responseFile = ft::concatPath(this->getRoot(), route);
    std::string indexFile    = ft::concatPath(responseFile, this->getIndex());

#ifdef DEBUG
    std::cout << "******************DEBUG:**********************" << std::endl;

    for (size_t i = 0; i < this->locations.size(); i++)
    {
        std::cout << "******************LOCATIONS:**********************"
                  << std::endl;
        if ((route.substr(0, this->locations[i]->getValue().length()) ==
             this->locations[i]->getValue()))
            std::cout << "Location: " << this->locations[i]->getValue()
                      << std::endl;
        std::cout << "**************************************************"
                  << std::endl;
    }

    std::cout << "**************************************************"
              << std::endl;
#endif // DEBUG
    //

    // Get the longest match for the location
    Location *longestMatchLoc = NULL;

    longestMatchLoc = this->findLongestLocationByPath(route);

    if (longestMatchLoc)
    {
        if (longestMatchLoc->getRoot().length() > 0)
            responseFile = ft::concatPath(
                longestMatchLoc->getRoot(),
                ft::removeRootFromPath(longestMatchLoc->getValue(), route));
        if (access(responseFile.c_str(), F_OK) == -1)
            responseFile.clear();
        if (ft::isDirectory(responseFile))
            indexFile =
                ft::concatPath(responseFile, longestMatchLoc->getIndex());
    }

    if (access(responseFile.c_str(), F_OK) == -1)
    {
        responseFile.clear();
        return (responseFile);
    }
    if (ft::isDirectory(responseFile))
    {
        responseFile = indexFile;
        if (access(responseFile.c_str(), F_OK) == -1)
            responseFile.clear();
    }

    return (responseFile);
}

int Server::server_listen()
{
    struct sockaddr_in address;
    int                fd;
    int                yes = 1;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Cannot create socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr =
        htonl(static_cast<uint32_t>(inet_addr(this->getHost().c_str())));
    address.sin_port = htons(this->getPort());
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(fd, 3) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "=> Server is listening on port " << this->getPort()
              << " <=" << std::endl;
    return (fd);
}
