#include "inc/ServerNameField.hpp"

ServerNameField::ServerNameField() : BaseField<std::string>() {}

ServerNameField::~ServerNameField() {}

ServerNameField::ServerNameField(std::string value)
    : BaseField<std::string>(value)
{
}

ServerNameField::ServerNameField(ServerNameField const &other)
{
    this->setValue(other.getValue());
}

ServerNameField &ServerNameField::operator=(const ServerNameField &other)
{
    this->setValue(other.getValue());
    return *this;
}

int ServerNameField::processValue(std::string value)
{
    // TODO: check if the server name is valid
    this->setValue(value);
    return 1;
}
