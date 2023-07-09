#include "inc/CGIField.hpp"

CGIField::CGIField(std::string val) : BaseField<std::string>(val)
{
    this->processValue(val);
}

CGIField::CGIField() : BaseField<std::string>()
{
    this->setValue("off");
}

CGIField::~CGIField() {}

CGIField::CGIField(CGIField const &other)
{
    this->setValue(other.getValue());
}

CGIField &CGIField::operator=(const CGIField &other)
{
    this->setValue(other.getValue());
    return *this;
}

bool CGIField::validate()
{
    if (this->getValue() == "on")
        return true;
    return false;
}

int CGIField::processValue(std::string value)
{
    std::cout << "CGIField::processValue: -" << value << "-" << std::endl;
    if (value == "on" || value == "off")
    {
        this->setValue(value);
        return 1;
    }
    return 0;
}
