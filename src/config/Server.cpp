#include "Server.hpp"

Server::Server()
{
    this->fields["listen"] = new ListenField();
    this->fields["location"] = new Location();
}

Server::~Server() {}

Server::Server(const Server &) {}

Server &Server::operator=(const Server &) { return (*this); }

void Server::validate()
{
    for (std::map<std::string, Base *>::iterator it = this->fields.begin();
         it != this->fields.end(); it++)
    {
    }
}

int Server::getPort() const
{
    return (dynamic_cast<ListenField *>(this->fields.at("listen"))->getPort());
}

std::string Server::getHost() const
{
    return (dynamic_cast<ListenField *>(this->fields.at("listen"))->getHost());
}

std::ostream &operator<<(std::ostream &out, Server const &server)
{
    out << "Server: ";
    out << "Port: " << server.getPort() << " Host: " << server.getHost();
    return (out);
}
