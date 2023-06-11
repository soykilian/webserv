#include "inc/AllowedMethodsField.hpp"

AllowedMethodsField::AllowedMethodsField(std::string val)
    : BaseField<std::string>(val)
{
}

AllowedMethodsField::AllowedMethodsField() : BaseField<std::string>() {}

AllowedMethodsField::~AllowedMethodsField() {}

AllowedMethodsField::AllowedMethodsField(AllowedMethodsField const &other)
{
    this->setValue(other.getValue());
}

AllowedMethodsField &
AllowedMethodsField::operator=(const AllowedMethodsField &other)
{
    this->setValue(other.getValue());
    return *this;
}

bool AllowedMethodsField::validate(std::string value)
{
    std::string       allowedMethods = "GET POST DELETE";
    std::stringstream ss(allowedMethods);
    std::string       substr;

    while (ss >> substr)
    {
        if (substr == value)
            return true;
    }
    return false;
}

int AllowedMethodsField::processValue(std::string value)
{
    size_t      start          = 0;
    size_t      end            = 0;
    std::string allowedMethods = "GET POST DELETE";

    while ((end = value.find(" ", start)) != std::string::npos)
    {
        methods.push_back(value.substr(start, end - start));
        start = end + 1;
    }
    methods.push_back(value.substr(start, end - start));
    for (size_t i = 0; i < methods.size(); i++)
    {
        if (!validate(methods[i]))
            return 0;
    }
    return 1;
}
