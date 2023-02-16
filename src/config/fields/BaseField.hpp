#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <string>

class Base
{
  public:
    Base() {}
    virtual ~Base(){};
    Base(const Base &);
    Base &operator=(const Base &);
    virtual int processValue(std::string value) = 0;
};

template <typename T> class BaseField : public Base
{
  public:
    BaseField() : Base() {}
    BaseField(T val) { this->setValue(val); }
    virtual ~BaseField(){};
    BaseField(BaseField const &other) { this->setValue(other.getValue()); }
    BaseField &operator=(BaseField const &other)
    {
        this->setValue(other.getValue());
        return *this;
    }
    T getValue() const { return this->m_data; }
    void setValue(T value)
    {
        this->m_data = value;
        this->m_isSet = true;
    }

    bool isSet() const { return this->m_isSet; }

    virtual int processValue(std::string value)
    {
        std::cout << "Processing vlue: " << value << std::endl;
        return (0);
    };

  private:
    T m_data;
    bool m_isSet;
};

#endif // BASE_HPP
