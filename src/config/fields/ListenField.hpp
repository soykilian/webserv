#ifndef LISTENFIELD_HPP
#define LISTENFIELD_HPP

#include "BaseField.hpp"
#include <cstdlib>
#include <sstream>
#include <string>

#include <Utils.hpp>

class ListenField : public BaseField<int>
{
  public:
    ListenField() : BaseField<int>() {}
    ListenField(int val) : BaseField<int>(val) {}
    virtual ~ListenField() {}
    ListenField(ListenField const &other);
    ListenField &operator=(ListenField const &other);

    int processValue(std::string value)
    {
        std::cout << "Processing value: " << value << std::endl;

        if (!ft::is_number(value))
        {
            std::cout << "Value is not a number" << std::endl;
            return 0;
        }

        int i;
        std::istringstream(value) >> i;
        if (i > 0 && i < 65536)
            return i;
        else
            return 0;
        this->setValue(i);
        return 1;
    }
};

#endif // LISTENFIELD_HPP
