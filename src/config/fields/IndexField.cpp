#include "inc/IndexField.hpp"

IndexField::IndexField() : BaseField<std::string>() { this->setIsPath(true); }

IndexField::IndexField(std::string val) : BaseField<std::string>(val)
{
    this->setIsPath(true);
}

int IndexField::processValue(std::string value)
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
