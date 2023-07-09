#include "inc/ClientBodySizeField.hpp"

ClientBodySizeField::ClientBodySizeField() : BaseField<size_t>() {}

ClientBodySizeField::ClientBodySizeField(size_t val) : BaseField<size_t>(val) {}

ClientBodySizeField::~ClientBodySizeField() {}

ClientBodySizeField::ClientBodySizeField(ClientBodySizeField const &other)
{
    this->setValue(other.getValue());
}

ClientBodySizeField &
ClientBodySizeField::operator=(const ClientBodySizeField &other)
{
    this->setValue(other.getValue());
    return *this;
}

int ClientBodySizeField::processValue(std::string value)
{
    if (!ft::is_number(value))
    {
        std::cout << "Value:" << value << " is not a number" << std::endl;
        return 0;
    }
    this->setValue(std::stoul(value.c_str()));
    return 1;
}
