#ifndef SERVER_HPP
#define SERVER_HPP

#include <BaseField.hpp>
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
    std::vector<Location *> locations;
};

#endif // !SERVER_HPP
