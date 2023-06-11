#ifndef BASE_FIELD_HPP
#define BASE_FIELD_HPP

#include "BaseClass.hpp"
#include <iostream>
#include <string>

template <typename T> class BaseField : public Base
{
  public:
    BaseField() : Base() {}
    BaseField(T val) : Base() { this->setValue(val); }
    BaseField(bool val) : Base() { this->setIsPath(val); }
    BaseField(T val, bool isPath) : Base()
    {
        this->setValue(val);
        this->setIsPath(isPath);
    }
    virtual ~BaseField() {}
    BaseField(BaseField<T> const &other) { this->setValue(other.getValue()); }

    BaseField<T> &operator=(BaseField<T> const &other)
    {
        this->setValue(other.getValue());
        return *this;
    }

    T            getValue() const { return this->m_data; }
    virtual void setValue(T value)
    {
        this->m_data = value;
        this->setIsSet(true);
    }

    virtual int processValue(std::string value)
    {
        std::cout << "Processing vlue: " << value << std::endl;
        return (0);
    }

  private:
    T m_data;
};

#endif // BASE_FIELD_HPP
