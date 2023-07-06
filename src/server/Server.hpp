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
    std::string getIndex() const;
    std::string getServerName() const;
    std::string getErrorPage() const;
    std::string getFileEnd() const;

    bool isAllowedMethod(std::string method) const;
    bool isAllowedMethodByPath(std::string method, std::string path) const;

    int  server_listen();
    void server_polling();

    std::vector<Location *> findLocationsByPath(std::string path) const;
    std::string directoryListing(std::string responseFile, std::string route) const;
    Location               *findLongestLocationByPath(std::string path) const;

    const Server &getServerByHost(std::string server_name);

    std::string getResponseFile(std::string route, short *flag) const;
};

std::ostream &operator<<(std::ostream &out, Server const &server);

#endif // !SERVER_HPP
