#ifndef LISTENFIELD_HPP
#define LISTENFIELD_HPP

#include "Base.hpp"
#include <string>

class ListenField : public Base<int>
{
  public:
    ListenField() : Base<int>() {}
    virtual ~ListenField();
    ListenField(ListenField const &other);
    ListenField &operator=(ListenField const &other);

    void processValue(std::string value) { this->setValue(std::stoi(value)); }
};

#endif // LISTENFIELD_HPP
