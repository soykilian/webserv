#ifdef CLIENT_BODY_SIZE_FIELD_HPP
#define CLIENT_BODY_SIZE_FIELD_HPP

#include "BaseField.hpp"

class ClientBodySizeField : public BaseField<std::string>
{
  public:
    ClientBodySizeField() : BaseField<std::string>() {}
    ClientBodySizeField(std::string val) : BaseField<std::string>(val) {}
    virtual ~ClientBodySizeField() {}
    ClientBodySizeField(ClientBodySizeField const &other);
    ClientBodySizeField &operator=(ClientBodySizeField const &other);

    int processValue(std::string value)
    {
        if (value == "off")
        {
            this->setValue(value);
            return 1;
        }
        if (!ft::is_number(value))
        {
            std::cout << "Value is not a number" << std::endl;
            return 0;
        }
        this->setValue(value);
        return 1;
    }
};

#endif // DEBUG
