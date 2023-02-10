#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <string>

template <typename T> class BaseProperty
{

  public:
    BaseProperty();
    virtual ~BaseProperty();
    BaseProperty(BaseProperty const &other);
    BaseProperty &operator=(BaseProperty const &other);
    T getValue() const { return m_data; }
    void setValue(T value) { m_data = value; }
    virtual void processValue(std::string value) = 0;

  private:
    T m_data;
};

#endif // BASE_HPP
