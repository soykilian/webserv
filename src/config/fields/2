#ifndef CGIFIELD_HPP
#define CGIFIELD_HPP

#include <Base.hpp>
#include <Utils.hpp>
#include <unistd.h>

class CGIField : public BaseField<std::string>
{
  public:
    CGIField();
    CGIField(std::string val);
    virtual ~CGIField();
    CGIField(CGIField const &other);
    CGIField &operator=(CGIField const &other);

    bool validate();
    int  processValue(std::string value);
};

#endif // !AUTOINDEXFIELD_HPP
