#include "inc/ErrorPageField.hpp"

ErrorPageField::ErrorPageField() : BaseField<std::string>() {}

ErrorPageField::~ErrorPageField() {}

ErrorPageField::ErrorPageField(std::string value)
    : BaseField<std::string>(value)
{
}

ErrorPageField::ErrorPageField(ErrorPageField const &other)
{
    this->setValue(other.getValue());
}

ErrorPageField &ErrorPageField::operator=(const ErrorPageField &other)
{
    this->setValue(other.getValue());
    return *this;
}

int ErrorPageField::processValue(std::string value)
{
    std::string fullpath = ft::concatPath(this->getRoot(), value);

    // if (access(fullpath.c_str(), F_OK) == -1)
    // {
    //     std::cout << "Path :" << value << "does not exist" << std::endl;
    //     return 0;
    // }
    // std::cout << "Path exists" << std::endl;
    this->setValue(value);
    return 1;
}
