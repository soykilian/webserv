#ifndef SERVERNAMEFIELD_HPP
#define SERVERNAMEFIELD_HPP

#include <Base.hpp>

class ServerNameField : public BaseField<std::string>
{
  public:
    ServerNameField();
    ServerNameField(std::string val);
    virtual ~ServerNameField();
    ServerNameField(ServerNameField const &other);
    ServerNameField &operator=(ServerNameField const &other);
    int              processValue(std::string value);
};

#endif // !SERVERNAMEFIELD_HPP
