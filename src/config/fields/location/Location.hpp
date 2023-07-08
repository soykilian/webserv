#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <Base.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <RedirectionField.hpp>

class Location : public BaseField<std::string>
{
  public:
    Location();
    ~Location();
    Location(const Location &);
    Location                     &operator=(const Location &);
    std::map<std::string, Base *> fields;
    int                           processValue(std::string value);
    int                           getClientBodySize() const;
    std::string                   getPath() const;
    std::string                   getRoot() const;
    std::string                   getIndex() const;
    std::string                   getErrorPage() const;
    std::string                   getFileEnd() const;
    RedirectionField*                   getRedirection() const;
    bool                          isAllowedMethod(std::string method) const;
};

std::ostream &operator<<(std::ostream &out, Location const &location);

#endif // DEBUG
