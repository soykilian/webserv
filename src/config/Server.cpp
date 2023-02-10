#include "Server.hpp"

Server::Server()
{
    this->fields["listen"] = new ListenField();
    this->fields["location"] = new Location();
}

Server::~Server() {}

Server::Server(const Server &) {}

Server &Server::operator=(const Server &) { return (*this); }
