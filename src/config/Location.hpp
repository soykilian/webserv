#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <string>
#include <vector>

class Location
{
  private:
    typedef enum
    {
        GET,
        POST,
        PUT,
        DELETE,
        UNKNOWN
    } http_method;

    http_method method;

  public:
    Location();
    ~Location();
    Location(const Location &);
    Location &operator=(const Location &);
};

#endif // DEBUG
