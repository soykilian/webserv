#include "inc/RedirectionField.hpp"
#include <vector>

RedirectionField::RedirectionField(std::string val)
    : BaseField<std::string>(val)
{
    this->setIsPath(false);
}

RedirectionField::RedirectionField() : BaseField<std::string>() {
    this->setIsPath(false);
}

RedirectionField::~RedirectionField() {}

RedirectionField::RedirectionField(RedirectionField const &other)
{
    this->setValue(other.getValue());
}

RedirectionField &
RedirectionField::operator=(const RedirectionField &other)
{
    this->setValue(other.getValue());
    return *this;
}
std::string RedirectionField::getUri() const
{
    return this->uri;
}

std::string RedirectionField::getCode() const
{
    return this->code;
}
void RedirectionField::setCode(std::string code)
{
    this->code = code;
}
void RedirectionField::setUri(std::string uri)
{
    this->uri = uri;
}

std::string RedirectionField::getRedirectionMessage() const
{
    std::cout << "code: " << this->getCode() << std::endl;
    return this->redirections_messages.at(this->code);
}

int RedirectionField::processValue(std::string value)
{
    std::vector<std::string> vals;
    std::istringstream     iss(value);
    std::string             sub;
    while (iss >> sub)
        vals.push_back(sub);
    if (vals.size() != 2 || !ft::is_number(vals[0]))
    {
        std::cout << "Invalid redirection field" << std::endl;
        return 0;
    }
    this->redirections_messages["300"] = "Multiple Choices";
    this->redirections_messages["301"] = "Moved Permanently";
    this->redirections_messages["302"] = "Found";
    this->redirections_messages["303"] = "See Other";
    this->redirections_messages["304"] = "Not Modified";
    this->redirections_messages["307"] = "Temporary Redirect";
    this->redirections_messages["308"] = "Permanent Redirect";
    std::map<std::string, std::string>::iterator it = this->redirections_messages.find(vals[0]);
    if (it == this->redirections_messages.end())
    {
        std::cout << "Invalid redirection code" << std::endl;
        return 0;
    }
    this->setValue(value);
    this->setCode(vals[0]);
    this->setUri(vals[1]);
    std::cout << "Redirection code: " << this->getCode() << std::endl;
    return 1;
}
