#ifndef ERRORPAGEFIELD_HPP
#define ERRORPAGEFIELD_HPP

#include "BaseField.hpp"
#include <Utils.hpp>
#include <unistd.h>

class ErrorPageField : public BaseField<std::string>
{
  public:
    ErrorPageField() : BaseField<std::string>(true) {}
    ErrorPageField(std::string val) : BaseField<std::string>(val, true) {}
    virtual ~ErrorPageField() {}
    ErrorPageField(ErrorPageField const &other);
    ErrorPageField &operator=(ErrorPageField const &other);

    int processValue(std::string value)
    {
        std::string fullpath = ft::concatPath(this->getRoot(), value);

        if (access(fullpath.c_str(), F_OK) == -1)
        {
            std::cout << "Path :" << value << "does not exist" << std::endl;
            return 0;
        }
        std::cout << "Path exists" << std::endl;
        this->setValue(value);
        return 1;
    }
};

#endif // !ERRORPAGEFIELD_HPP
