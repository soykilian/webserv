#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "BaseField.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Location : public BaseField<std::string>
{
  public:
    Location();
    ~Location();
    Location(const Location &);
    Location &operator=(const Location &);
    std::map<std::string, Base *> fields;
    int processValue(std::string value);
    std::string getPath() const;
    std::string getRoot() const;
};

std::ostream &operator<<(std::ostream &out, Location const &location);

#endif // DEBUG
