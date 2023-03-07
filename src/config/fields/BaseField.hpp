#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <string>

class Base
{
  public:
    Base()
    {
        this->setIsPath(false);
        this->m_isSet = false;
    }
    virtual ~Base(){};
    Base(const Base &);
    Base &operator=(const Base &);
    virtual int processValue(std::string value) = 0;

    bool isSet() const { return this->m_isSet; }

    bool isPath() const { return this->m_isPath; }

    void setIsPath(bool isPath) { this->m_isPath = isPath; }

    void setIsSet(bool isSet) { this->m_isSet = isSet; }

    std::string getRoot() { return this->root; }

    void setRoot(std::string str) { this->root = str; }

  private:
    bool m_isSet;
    bool m_isPath;
    std::string root;
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
        this->setIsSet(true);
    }

    virtual int processValue(std::string value)
    {
        std::cout << "Processing vlue: " << value << std::endl;
        return (0);
    };

  private:
    T m_data;
};

#endif // BASE_HPP
