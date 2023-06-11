#ifndef LISTENFIELD_HPP
#define LISTENFIELD_HPP

#include <Base.hpp>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>

#include <Utils.hpp>

class ListenField : public BaseField<std::string>
{
  private:
    int         m_port;
    std::string m_host;

  public:
    ListenField();
    ListenField(std::string val);
    virtual ~ListenField();
    ListenField(ListenField const &other);
    ListenField &operator=(ListenField const &other);

    int         getPort() const;
    std::string getHost() const;

    int processValue(std::string value);
};

#endif // LISTENFIELD_HPP
