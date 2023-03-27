#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Request
{
  public:
    Request(int fd);
    Request();
    ~Request();
    Request(Request const &src);
    Request &operator=(Request const &rhs);

    std::string getHeader(std::string key) const;
    std::string getBody() const;
    std::string getMethod() const;

  private:
    std::string                        bufferLeft;
    std::string                        body;
    std::map<std::string, std::string> headers;
    std::string                        method;
    std::string                        route;
    std::string                        version;
};

#endif // !REQUEST_HPP
