#ifndef INDEXFIELD_HPP
#define INDEXFIELD_HPP

#include <Base.hpp>
#include <iostream>

class IndexField : public BaseField<std::string>
{
  public:
    IndexField();
    IndexField(std::string val);
    virtual ~IndexField() {}
    IndexField(IndexField const &other);
    IndexField &operator=(IndexField const &other);

    int processValue(std::string value);
};

#endif // !INDEXFIELD_HPP
