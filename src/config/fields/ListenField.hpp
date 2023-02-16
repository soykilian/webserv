#ifndef LISTENFIELD_HPP
#define LISTENFIELD_HPP

#include "BaseField.hpp"
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
    int m_port;
    std::string m_host;

  public:
    ListenField() : BaseField<std::string>() {}
    ListenField(std::string val) : BaseField<std::string>(val) {}
    virtual ~ListenField() {}
    ListenField(ListenField const &other);
    ListenField &operator=(ListenField const &other);

    int getPort() const { return this->m_port; }
    std::string getHost() const { return this->m_host; }

    int processValue(std::string value)
    {
        if (value.find(":") != std::string::npos)
        {
            char *token = strtok((char *)value.c_str(), ":");
            if (inet_addr(token) == INADDR_NONE)
            {
                std::cout << "Invalid host" << std::endl;
                return 0;
            }
            this->m_host = token;
            token = strtok(NULL, ":");
            if (token != NULL && !ft::is_number(token))
            {
                std::cout << "Value is not a number" << std::endl;
                delete token;
                return 0;
            }
            this->m_port = atoi(token);
        }
        else if (ft::is_number(value))
            this->m_port = atoi(value.c_str());
        else
        {
            std::cout << "Invalid value" << std::endl;
            return 0;
        }
        if (this->m_port <= 0 || this->m_port >= 65536)
            return 0;
        this->setValue(value);
        return 1;
    }
};

#endif // LISTENFIELD_HPP
