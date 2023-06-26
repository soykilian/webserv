#include "inc/LoadFolderField.hpp"

LoadFolderField::LoadFolderField(std::string val)
    : BaseField<std::string>(val)
{
    this->setIsPath(true);
}

LoadFolderField::LoadFolderField() : BaseField<std::string>() {
    this->setIsPath(true);
}

LoadFolderField::~LoadFolderField() {}

LoadFolderField::LoadFolderField(LoadFolderField const &other)
{
    this->setValue(other.getValue());
}

LoadFolderField &
LoadFolderField::operator=(const LoadFolderField &other)
{
    this->setValue(other.getValue());
    return *this;
}

int LoadFolderField::processValue(std::string value)
{
    if (value.find("/") == 0)
        value = value.substr(1);
    std::string folderPath = this->getRoot() +"/"+ value;
    std::cout << "Folder path: " << folderPath << std::endl;
    if (access(folderPath.c_str(), F_OK) == -1)
    {
        std::cout << "Folder does not exist" << std::endl;
        return 0;
    }
    return 1;
}
