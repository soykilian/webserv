#include "inc/RootField.hpp"
#include "BaseField.hpp"

RootField::RootField() : BaseField<std::string>() {}

RootField::RootField(std::string value) : BaseField<std::string>(value) {}

RootField::~RootField() {}

RootField::RootField(RootField const &other)
{
    this->setValue(other.getValue());
}

RootField &RootField::operator=(const RootField &other)
{
    this->setValue(other.getValue());
    return *this;
}

int RootField::processValue(std::string value)
{
    // // check if the path is a valid path
    // if (access(value.c_str(), F_OK) == -1)
    // {
    //     std::cout << "Path does not exist" << std::endl;
    //     return 0;
    // }
    this->setValue(value);
    this->setRoot(value);
    return 1;
}
