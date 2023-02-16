#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Request
{
  public:
    Request(std::string request);
    ~Request();

    std::string getMethod() const;
    std::string getUri() const;
    std::string getHost() const;
    std::string getContentLength() const;
    std::string getContentType() const;
    std::string getBody() const;

    void setMethod(std::string method);
    void setUri(std::string uri);
    void setHost(std::string host);
    void setContentLength(std::string contentLength);
    void setContentType(std::string contentType);
    void setBody(std::string body);

    std::map<std::string, std::string> headers;

  private:
    Request();
    Request(Request const &src);
    Request &operator=(Request const &rhs);

    std::string _method;
    std::string _uri;
    std::string _host;
    std::string _contentLength;
    std::string _contentType;
    std::string _body;
};

#endif // !REQUEST_HPP
