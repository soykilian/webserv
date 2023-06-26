#include "inc/IndexField.hpp"

IndexField::IndexField() : BaseField<std::string>() {}

IndexField::IndexField(std::string val) : BaseField<std::string>(val) {}

int IndexField::processValue(std::string value)
{
    this->setValue(value);
    return (1);
}
