#ifndef ALLOWD_METHODS_FIELD_HPP
#define ALLOWD_METHODS_FIELD_HPP

#include <Base.hpp>
#include <Utils.hpp>
#include <cstdlib>
#include <sstream>
#include <vector>

class AllowedMethodsField : public BaseField<std::string>
{
  public:
    AllowedMethodsField();
    AllowedMethodsField(std::string val);
    virtual ~AllowedMethodsField();
    AllowedMethodsField(AllowedMethodsField const &other);
    AllowedMethodsField &operator=(AllowedMethodsField const &other);

    bool validate(std::string value);

    int processValue(std::string value);

  private:
    std::vector<std::string> methods;
};

#endif // DEBUG
