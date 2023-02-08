#ifndef LISTENFIELD_HPP
#define LISTENFIELD_HPP

#include "BaseField.hpp"
#include <sstream>
#include <string>

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
        int i;
        std::istringstream(value) >> i;
        this->setValue(i);
        return 1;
    }
};

#endif // LISTENFIELD_HPP
