#ifndef ERRORPAGEFIELD_HPP
#define ERRORPAGEFIELD_HPP

#include "BaseField.hpp"

class ErrorPageField : public BaseField<std::string>
{
  public:
    ErrorPageField() : BaseField<std::string>() {}
    ErrorPageField(std::string val) : BaseField<std::string>(val) {}
    virtual ~ErrorPageField() {}
    ErrorPageField(ErrorPageField const &other);
    ErrorPageField &operator=(ErrorPageField const &other);

    int processValue(std::string value)
    {
        this->setValue(value);
        return 0;
    }
};

#endif // !ERRORPAGEFIELD_HPP
