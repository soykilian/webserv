#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Server
{
  private:
    int port;
    std::string host;
    std::string serverName;
    std::string root;
    std::string index;
    std::string errorPage;
    std::string clientMaxBodySize;

  public:
    Server();
    ~Server();
    Server(const Server &);
    Server &operator=(const Server &);

    typedef int (Server::*processProperty)(std::string);

    std::map<std::string, processProperty> propertyMap;

    std::vector<std::string> mandatoryProperties;

    int processListen(std::string value);
    int processServerName(std::string value);
    int processRoot(std::string value);
    int processIndex(std::string value);
    int processErrorPage(std::string value);
    int processClientMaxBodySize(std::string value);
    int processLocation(std::string value);

    Server *next;
};

#endif // !SERVER_HPP
