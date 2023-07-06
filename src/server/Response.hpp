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
    std::string fileEdition(int flag);

  private:
    Request                *request;
    const Server           &server;
    Server                  empty;
    std::vector<Location *> locations;
    std::string             cgi_path;
    std::string             php_path;
    std::string             query_params;

    std::string getErrorPage(std::string code);
    std::string addDate(std::string message);
    std::string getPath();
    std::string get_cgi();
    std::string processCgi();
    char      **set_env();
};

#endif // !RESPONSE_HPP
