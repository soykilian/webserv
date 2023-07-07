#include "inc/LoadFolderField.hpp"

LoadFolderField::LoadFolderField(std::string val) : BaseField<std::string>(val)
{
    this->setIsPath(false);
}

LoadFolderField::LoadFolderField() : BaseField<std::string>()
{
    this->setIsPath(false);
}

LoadFolderField::~LoadFolderField() {}

LoadFolderField::LoadFolderField(LoadFolderField const &other)
{
    this->setValue(other.getValue());
}

LoadFolderField &LoadFolderField::operator=(const LoadFolderField &other)
{
    this->setValue(other.getValue());
    return *this;
}

int LoadFolderField::processValue(std::string value)
{
    this->setValue(value);
    return 1;
}
