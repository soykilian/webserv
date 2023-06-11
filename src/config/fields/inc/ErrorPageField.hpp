#ifndef ERRORPAGEFIELD_HPP
#define ERRORPAGEFIELD_HPP

#include <Base.hpp>
#include <Utils.hpp>
#include <unistd.h>

class ErrorPageField : public BaseField<std::string>
{
  public:
    ErrorPageField();
    ErrorPageField(std::string val);
    virtual ~ErrorPageField();
    ErrorPageField(ErrorPageField const &other);
    ErrorPageField &operator=(ErrorPageField const &other);

    int processValue(std::string value);
};

#endif // !ERRORPAGEFIELD_HPP
