#ifndef LOCATIONMETHODSFIELD_HPP
#define LOCATIONMETHODSFIELD_HPP

#include "fields/BaseField.hpp"

class LocationMethodsField : public BaseField<std::string>
{
  public:
    LocationMethodsField() : BaseField<std::string>() {}
    LocationMethodsField(std::string val) : BaseField<std::string>(val) {}
    virtual ~LocationMethodsField();
    LocationMethodsField(const LocationMethodsField &other);
    LocationMethodsField &operator=(const LocationMethodsField &other);

    int processValue(std::string val)
    {
        this->setValue(val);
        return 1;
    }
};

#endif // !DEBUG
