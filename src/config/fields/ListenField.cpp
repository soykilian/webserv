#include "inc/ListenField.hpp"

ListenField::ListenField() : BaseField<std::string>() {}

ListenField::ListenField(std::string val) : BaseField<std::string>(val) {}

ListenField::~ListenField() {}

ListenField::ListenField(ListenField const &other)
{
    this->setValue(other.getValue());
}

ListenField &ListenField::operator=(const ListenField &other)
{
    this->setValue(other.getValue());
    return *this;
}

int         ListenField::getPort() const { return this->m_port; }
std::string ListenField::getHost() const { return this->m_host; }

int ListenField::processValue(std::string value)
{
    if (value.find(":") != std::string::npos)
    {
        char *token = strtok((char *)value.c_str(), ":");
        if (inet_addr(token) == INADDR_NONE)
        {
            std::cout << "Invalid host" << std::endl;
            return 0;
        }
        this->m_host = token;
        token        = strtok(NULL, ":");
        if (token != NULL && !ft::is_number(token))
        {
            std::cout << "Value is not a number" << std::endl;
            delete token;
            return 0;
        }
        this->m_port = atoi(token);
    }
    else if (ft::is_number(value))
        this->m_port = atoi(value.c_str());
    else
    {
        std::cout << "Invalid value" << std::endl;
        return 0;
    }
    if (this->m_port <= 0 || this->m_port >= 65536)
        return 0;
    this->setValue(value);
    return 1;
}
