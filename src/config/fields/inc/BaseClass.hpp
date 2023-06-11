#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <string>

class Base
{
  public:
    Base();
    virtual ~Base();
    Base(const Base &);
    Base       &operator=(const Base &);
    virtual int processValue(std::string value) = 0;

    bool isSet() const;

    bool isPath() const;

    void setIsPath(bool isPath);

    void setIsSet(bool isSet);

    std::string getRoot();

    void setRoot(std::string str);

  private:
    bool        m_isSet;
    bool        m_isPath;
    std::string root;
};

#endif // BASE_HPP
