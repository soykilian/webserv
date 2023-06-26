#ifndef LOAD_FOLDER_FIELD_HPP
#define LOAD_FOLDER_FIELD_HPP

#include <Base.hpp>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <Utils.hpp>
#include <sstream>

class LoadFolderField : public BaseField<std::string>
{
  public:
    LoadFolderField();
    LoadFolderField(std::string val);
    virtual ~LoadFolderField();
    LoadFolderField(LoadFolderField const &other);
    LoadFolderField &operator=(LoadFolderField const &other);

    int processValue(std::string value);
};

#endif // DEBUG
