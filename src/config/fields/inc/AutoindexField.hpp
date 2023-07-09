#ifndef AUTOINDEXFIELD_HPP
#define AUTOINDEXFIELD_HPP

#include <Base.hpp>
#include <Utils.hpp>
#include <unistd.h>

class AutoindexField : public BaseField<std::string>
{
  public:
    AutoindexField();
    AutoindexField(std::string val);
    virtual ~AutoindexField();
    AutoindexField(AutoindexField const &other);
    AutoindexField &operator=(AutoindexField const &other);

    bool validate();
    int  processValue(std::string value);
};

#endif // !AUTOINDEXFIELD_HPP
