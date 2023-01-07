#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

Location::Location(const Location &) {}

Location &Location::operator=(const Location &rhs)
{
    if (this == &rhs)
        return *this;
    return *this;
}
