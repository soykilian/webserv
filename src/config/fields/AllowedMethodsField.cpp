#include "inc/AllowedMethodsField.hpp"

AllowedMethodsField::AllowedMethodsField(std::string val)
    : BaseField<std::string>(val)
{
}

AllowedMethodsField::AllowedMethodsField() : BaseField<std::string>()
{
    this->setValue("GET POST DELETE");
    this->methods.push_back("GET");
    this->methods.push_back("POST");
    this->methods.push_back("DELETE");
    this->setIsSet(false);
}

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
    for (size_t i = 0; i < methods.size(); i++)
    {
        if (!methods[i].empty() && methods[i] == value)
            return true;
    }
    return false;
}

int AllowedMethodsField::processValue(std::string value)
{
    size_t      start          = 0;
    size_t      end            = 0;
    std::string allowedMethods = "GET POST DELETE";
    this->methods.clear();

    while ((end = value.find(" ", start)) != std::string::npos)
    {
        methods.push_back(value.substr(start, end - start));
        start = end + 1;
    }
    methods.push_back(value.substr(start, end - start));
    for (size_t i = 0; i < methods.size(); i++)
    {
        if (!methods[i].empty() &&
            allowedMethods.find(methods[i]) == std::string::npos)
            return 0;
    }
    this->setValue(value);
    return 1;
}
