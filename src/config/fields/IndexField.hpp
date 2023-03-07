#ifndef INDEXFIELD_HPP
#define INDEXFIELD_HPP

#include "BaseField.hpp"
#include <iostream>

class IndexField : public BaseField<std::string>
{
  public:
    IndexField() : BaseField<std::string>() { this->setIsPath(true); }
    IndexField(std::string val) : BaseField<std::string>(val) {}
    virtual ~IndexField() {}
    IndexField(IndexField const &other);
    IndexField &operator=(IndexField const &other);

    int processValue(std::string value)
    {
        (void)value;
        if (this->getRoot().empty())
        {
            std::cerr << "Index is a path field. Root must be provided"
                      << std::endl;
            return (0);
        }
        return (1);
    }
};

#endif // !INDEXFIELD_HPP
