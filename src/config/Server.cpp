#include "Server.hpp"

Server::Server() { this->fields["listen"] = new ListenField(); }

Server::~Server() {}

Server::Server(const Server &) {}

Server &Server::operator=(const Server &) { return (*this); }
