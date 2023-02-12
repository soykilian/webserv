#ifndef CLIENT_BODY_SIZE_FIELD_HPP
#define CLIENT_BODY_SIZE_FIELD_HPP

#include "BaseField.hpp"
#include <Utils.hpp>
#include <cstdlib>

class ClientBodySizeField : public BaseField<int>
{
  public:
    ClientBodySizeField() : BaseField<int>() {}
    ClientBodySizeField(int val) : BaseField<int>(val) {}
    virtual ~ClientBodySizeField() {}
    ClientBodySizeField(ClientBodySizeField const &other);
    ClientBodySizeField &operator=(ClientBodySizeField const &other);

    int processValue(std::string value)
    {
        if (!ft::is_number(value))
        {
            std::cout << "Value is not a number" << std::endl;
            return 0;
        }
        this->setValue(atoi(value.c_str()));
        return 1;
    }
};

#endif // DEBUG
