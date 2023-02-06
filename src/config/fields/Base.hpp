#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <string>

template <typename T> class Base
{
  public:
    Base();
    virtual ~Base();
    Base(Base const &other);
    Base &operator=(Base const &other);
    T getValue() const;
    void setValue(T value);
    virtual void processValue(std::string value) = 0;

  private:
    T m_data;
};

#endif // BASE_HPP
