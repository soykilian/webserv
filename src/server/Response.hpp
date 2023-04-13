#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <Request.hpp>
#include <string>

class Response
{
  public:
    Response(Request *request);
    Response();
    ~Response();
    Response &operator=(const Response &other);
    Response(const Response &other);
    std::string getResponse();

  private:
    Request      *request;
    const Server &server;
    Server        empty;

    std::string getErrorPage();
};

#endif // !RESPONSE_HPP
