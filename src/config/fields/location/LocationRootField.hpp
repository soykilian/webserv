#ifndef LOCATIONROOTFIELD_HPP
#define LOCATIONROOTFIELD_HPP

#include "../BaseField.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

class LocationRootField : public BaseField<std::string>
{
  public:
    LocationRootField() : BaseField<std::string>() {}
    ~LocationRootField() {}
    LocationRootField(const LocationRootField &);
    LocationRootField &operator=(const LocationRootField &);
    int processValue(std::string value)
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
};

#endif // !LOCATIONROOTFIELD_HPP
