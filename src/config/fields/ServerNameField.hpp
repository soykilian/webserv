#ifndef SERVERNAMEFIELD_HPP
#define SERVERNAMEFIELD_HPP

#include "BaseField.hpp"

class ServerNameField : public BaseField<std::string>
{
  public:
    ServerNameField() : BaseField<std::string>() {}
    ServerNameField(std::string val) : BaseField<std::string>(val) {}
    virtual ~ServerNameField() {}
    ServerNameField(ServerNameField const &other);
    ServerNameField &operator=(ServerNameField const &other);

    int processValue(std::string value)
    {
        // TODO: check if the server name is valid
        this->setValue(value);
        return 1;
    }
};

#endif // !SERVERNAMEFIELD_HPP
