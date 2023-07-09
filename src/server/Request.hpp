#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Server.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Server;
class Request
{
  public:
    Request(int fd, const Server &server);
    Request();
    ~Request();
    Request(Request const &src);
    Request &operator=(Request const &rhs);

    std::string getHeader(std::string key) const;
    std::string getBody() const;
    std::string getMethod() const;
    std::string getRoute() const;
    std::string getVersion() const;
    std::string getHost() const;

    int getState() const;

    const Server &getServer() const;

    int read();

  private:
    std::string                        bufferLeft;
    std::string                        body;
    std::map<std::string, std::string> headers;
    std::string                        method;
    std::string                        route;
    std::string                        version;
    std::string                        host;
    int                                fd;
    int                                state;
    const Server                      &server;
    Server                             empty;
    void                               limitBody();
};

std::ostream &operator<<(std::ostream &out, Request const &req);

#endif // !REQUEST_HPP
