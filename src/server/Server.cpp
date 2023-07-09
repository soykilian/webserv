#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <Fields.hpp>
#include <Utils.hpp>
#include <arpa/inet.h>
#include <cstdio>
#include <dirent.h>
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
    this->fields["allowed_methods"]  = new AllowedMethodsField();
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

size_t Server::getClientBodySize() const
{
    return (
        dynamic_cast<ClientBodySizeField *>(this->fields.at("client_body_size"))
            ->getValue());
}

bool Server::isClientBodySizeSet() const
{
    return (
        dynamic_cast<ClientBodySizeField *>(this->fields.at("client_body_size"))
            ->isSet());
}

bool Server::isAllowedMethod(std::string method) const
{
    this->fields.at("allowed_methods")->isSet();
    return (
        dynamic_cast<AllowedMethodsField *>(this->fields.at("allowed_methods"))
            ->validate(method));
}

bool Server::isAutoindexOn() const
{
    return (dynamic_cast<AutoindexField *>(this->fields.at("autoindex"))
                ->validate());
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

Location *findLongestLocationInVecto(std::vector<Server const *> server)
{
    size_t    longestMatch    = 0;
    Location *longestMatchLoc = NULL;

    for (size_t i = 0; i < server.size(); i++)
    {
        for (size_t j = 0; j < server[i]->locations.size(); j++)
        {
            if (server[i]->locations[j]->getValue().length() > longestMatch)
            {
                longestMatch    = server[i]->locations[j]->getValue().length();
                longestMatchLoc = server[i]->locations[j];
            }
        }
    }
    return longestMatchLoc;
}

std::vector<const Server *>
Server::getServerByHost(std::string server_name) const
{
    Server const               *current = this;
    std::vector<const Server *> res     = std::vector<const Server *>();

    while (current)
    {
        if (current->getServerName() == server_name)
            res.push_back(current);
        current = current->next;
    }
    return res;
}

std::ostream &operator<<(std::ostream &out, Server const &server)
{
    out << "Server: " << std::endl;
    out << "\t Port: " << server.getPort() << std::endl;
    out << "\t Host: " << server.getHost() << std::endl;
    out << "\t ServerName: " << server.getServerName() << std::endl;
    out << "\t Root: " << server.getRoot() << std::endl;
    out << "\t ErrorPage: " << server.getErrorPage() << std::endl;
    out << "\t ClientBodySize: " << server.getClientBodySize() << std::endl;
    out << "\t Index: " << server.getIndex() << std::endl;
    out << "\t AllowedMethods: "
        << dynamic_cast<AllowedMethodsField *>(
               server.fields.at("allowed_methods"))
               ->getValue()
        << std::endl;
    out << "\t PostFolder: "
        << dynamic_cast<LoadFolderField *>(server.fields.at("post_folder"))
               ->getValue()
        << std::endl;
    out << "\t Locations: " << std::endl;
    for (size_t i = 0; i < server.locations.size(); i++)
        out << "\t\t" << *server.locations[i] << std::endl;
    out << std::endl;
    return (out);
}

std::string Server::getFileEnd() const
{
    return (dynamic_cast<LoadFolderField *>(this->fields.at("post_folder"))
                ->getValue());
}

std::string Server::directoryListing(std::string responseFile,
                                     std::string route) const
{
    std::cout << "Directory listing" << std::endl;
    std::string htmlFile;
    htmlFile += "<!DOCTYPE html>\n";
    htmlFile += "<head>\n";
    htmlFile += "<title>Directory Listing</title>\n";
    htmlFile += "</head>\n";
    htmlFile += "<body>\n";
    htmlFile += "<ul>\n";
    DIR           *dir;
    struct dirent *ent;
    if ((dir = opendir(responseFile.c_str())) != NULL)
    {
        while ((ent = readdir(dir)))
        {
            htmlFile += "<li>";
            std::string dir_name(ent->d_name);
            htmlFile += "<a href=\"" + ft::concatPath(route, dir_name) + "\">" +
                        dir_name + "</a><br>\n";
            htmlFile += "</li>";
        }
        closedir(dir);
    }
    else
    {
        std::cout << "Error opening directory" << std::endl;
        return "";
    }
    htmlFile += "</ul>\n";
    htmlFile += "</body>\n";
    htmlFile += "</html>\n";
    return htmlFile;
}

std::string Server::getResponseFile(Request *req, Response *res,
                                    short *flag) const
{
    std::string     responseFile;
    std::string     indexFile;
    Location const *loc    = res->getLocation();
    Server const   *server = res->getServer();
    std::string     route;

    if (loc)
    {
        responseFile = ft::concatPath(
            loc->getRoot(),
            ft::removeRootFromPath(loc->getValue(), req->getRoute()));
        indexFile = ft::concatPath(
            responseFile,
            ft::removeRootFromPath(loc->getValue(), loc->getIndex()));
        route = ft::removeRootFromPath(loc->getValue(), req->getRoute());
    }
    else
    {
        responseFile = ft::concatPath(server->getRoot(), req->getRoute());
        indexFile    = ft::concatPath(responseFile, server->getIndex());
        route        = req->getRoute();
    }

    if (access(responseFile.c_str(), F_OK) == -1)
    {
        responseFile.clear();
        return (responseFile);
    }
    if (ft::isDirectory(responseFile))
    {
        std::string temp = responseFile;
        responseFile     = indexFile;
        if (temp == responseFile)
        {
            *flag = 1;
            return directoryListing(responseFile, route);
        }
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
