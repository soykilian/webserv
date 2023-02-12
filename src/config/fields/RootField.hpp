#ifndef ROOTFIELD_HPP
#define ROOTFIELD_HPP

#include "BaseField.hpp"
#include <string>
#include <unistd.h>

class RootField : public BaseField<std::string>
{
  public:
    RootField() : BaseField<std::string>() {}
    RootField(std::string val) : BaseField<std::string>(val) {}
    virtual ~RootField() {}
    RootField(RootField const &other);
    RootField &operator=(RootField const &other);

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

#endif // !ROOTFIELD_HPP
