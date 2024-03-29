#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <Base.hpp>
#include <RedirectionField.hpp>
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
    Location                     &operator=(const Location &);
    std::map<std::string, Base *> fields;
    int                           processValue(std::string value);
    size_t                        getClientBodySize() const;
    bool                          isAutoindexOn() const;
    bool                          isCGIOn() const;
    std::string                   getPath() const;
    std::string                   getRoot() const;
    std::string                   getIndex() const;
    std::string                   getErrorPage() const;
    std::string                   getFileEnd() const;
    RedirectionField             *getRedirection() const;
    bool                          isAllowedMethod(std::string method) const;
    bool                          isClientBodySizeSet() const;
    bool                          isRootSet() const;
};

std::ostream &operator<<(std::ostream &out, Location const &location);

#endif // DEBUG
