#include "inc/AutoindexField.hpp"

AutoindexField::AutoindexField(std::string val) : BaseField<std::string>(val) {}

AutoindexField::AutoindexField() : BaseField<std::string>() {}

AutoindexField::~AutoindexField() {}

AutoindexField::AutoindexField(AutoindexField const &other)
{
    this->setValue(other.getValue());
}

AutoindexField &AutoindexField::operator=(const AutoindexField &other)
{
    this->setValue(other.getValue());
    return *this;
}

bool AutoindexField::validate()
{
    if (this->getValue() == "on")
        return true;
    return false;
}

int AutoindexField::processValue(std::string value)
{
    if (value == "on" || value == "off")
    {
        this->setValue(value);
        return 0;
    }
    return 1;
}
