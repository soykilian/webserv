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
    std::vector<Location *>       locations;

    Server *next;

    void setDefaultErrPage();

    bool validate();
    bool appendServer(Server *server);

    // Getters for all the properties in the map
    int         getPort() const;
    int         getClientBodySize() const;
    std::string getHost() const;
    std::string getRoot() const;
    std::string getServerName() const;
    std::string getErrorPage() const;

    bool isAllowedMethod(std::string method) const;

    int  server_listen();
    void server_polling();

    std::string getResponseFile(std::string route) const;
};

std::ostream &operator<<(std::ostream &out, Server const &server);

#endif // !SERVER_HPP
