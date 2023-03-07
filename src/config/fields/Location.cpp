#include "fields/IndexField.hpp"
#include <Fields.hpp>
#include <unistd.h>

Location::Location() : BaseField<std::string>()
{
    this->fields["root"] = new RootField();
    this->fields["index"] = new IndexField();
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
    if (access(value.c_str(), F_OK) == -1)
    {
        std::cout << "Path does not exist" << std::endl;
        return 0;
    }
    this->setValue(value);
    return 1;
}

std::string Location::getPath() const { return this->getValue(); }

std::string Location::getRoot() const
{
    return ((RootField *)this->fields.at("root"))->getValue();
}

std::ostream &operator<<(std::ostream &out, Location const &location)
{
    out << "Location path: " << location.getPath()
        << "Location Root Path: " << location.getRoot();
    return out;
}
