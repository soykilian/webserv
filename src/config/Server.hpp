#ifndef SERVER_HPP
#define SERVER_HPP

#include <Fields.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Server
{
  public:
    Server();
    ~Server();
    Server(const Server &);
    Server &operator=(const Server &);

    std::map<std::string, Base *> fields;
    std::vector<Location *> locations;

    void validate();

    int getPort() const;
    std::string getHost() const;
    int server_listen();
};

std::ostream &operator<<(std::ostream &out, Server const &server);

#endif // !SERVER_HPP
