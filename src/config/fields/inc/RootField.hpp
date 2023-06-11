#ifndef ROOTFIELD_HPP
#define ROOTFIELD_HPP

#include <Base.hpp>
#include <string>
#include <unistd.h>

class RootField : public BaseField<std::string>
{
  public:
    RootField();
    RootField(std::string val);
    virtual ~RootField();
    RootField(RootField const &other);
    RootField &operator=(RootField const &other);

    int processValue(std::string value);
};

#endif // !ROOTFIELD_HPP
