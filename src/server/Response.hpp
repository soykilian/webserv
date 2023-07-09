#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <Request.hpp>
#include <Server.hpp>
#include <string>

// class Request;
class Response
{
  public:
    Response(Request *request);
    Response();
    ~Response();
    Response &operator=(const Response &other);
    Response(const Response &other);
    std::string                 getResponse();
    std::string                 fileEdition(int flag);
    std::vector<Server const *> getServersByHost() const;
    Server const               *getServerByLocation() const;
    void                        setLocationAndServer(std::string path);

    Server const   *getServer() const;
    Location const *getLocation() const;

  private:
    Request                    *request;
    const Server               &server;
    Server                      empty;
    std::vector<Location *>     locations;
    std::vector<Server const *> serversByHost;
    Server const               *currentServer;
    Location const             *currentLocation;
    std::string                 cgi_path;
    std::string                 php_path;
    std::string                 query_params;

    std::string getErrorPage(std::string code);
    std::string addDate();
    std::string getPath();
    std::string get_cgi();
    std::string processCgi();
    std::string checkErrors();
    char      **set_env();
};

#endif // !RESPONSE_HPP
