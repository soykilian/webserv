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

    void setDefaultErrPage();

    bool validate();

    // Getters for all the properties in the map
    int getPort() const;
    int getClientBodySize() const;
    std::string getHost() const;
    std::string getRoot() const;
    std::string getServerName() const;
    std::string getErrorPage() const;

    int server_listen();
    void server_polling();
};

std::ostream &operator<<(std::ostream &out, Server const &server);

#endif // !SERVER_HPP
