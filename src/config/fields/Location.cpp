#include "inc/IndexField.hpp"
#include <Fields.hpp>
#include <unistd.h>

Location::Location() : BaseField<std::string>()
{
    this->fields["root"]             = new RootField();
    this->fields["index"]            = new IndexField();
    this->fields["methods"]          = new AllowedMethodsField();
    this->fields["post_folder"]      = new LoadFolderField();
    this->fields["client_body_size"] = new ClientBodySizeField();
    this->fields["error_page"]       = new ErrorPageField();
}

Location::~Location() {}

Location::Location(const Location &other) : BaseField<std::string>()
{
    this->fields = other.fields;
}

Location &Location::operator=(const Location &rhs)
{
    if (this == &rhs)
        return *this;
    return *this;
}

int Location::processValue(std::string value)
{
    // check if the path is a valid path
    // if (access(value.c_str(), F_OK) == -1)
    // {
    //     std::cout << "Path does not exist" << std::endl;
    //     return 0;
    // }
    this->setValue(value);
    return 1;
}

std::string Location::getPath() const { return this->getValue(); }

std::string Location::getRoot() const
{
    return ((RootField *)this->fields.at("root"))->getValue();
}

std::string Location::getIndex() const
{
    return ((RootField *)this->fields.at("index"))->getValue();
}

int Location::getClientBodySize() const
{
    return (
        dynamic_cast<ClientBodySizeField *>(this->fields.at("client_body_size"))
            ->getValue());
}

std::string Location::getErrorPage() const
{
    return (dynamic_cast<ErrorPageField *>(this->fields.at("error_page"))
                ->getValue());
}

bool Location::isAllowedMethod(std::string method) const
{
    return (dynamic_cast<AllowedMethodsField *>(this->fields.at("methods"))
                ->validate(method));
}

std::ostream &operator<<(std::ostream &out, Location const &location)
{
    out << "Location path: " << location.getPath()
        << "Location Root Path: " << location.getRoot();
    return out;
}
