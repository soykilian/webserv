#ifndef CLIENT_BODY_SIZE_FIELD_HPP
#define CLIENT_BODY_SIZE_FIELD_HPP

#include <Base.hpp>
#include <Utils.hpp>
#include <cstdlib>

class ClientBodySizeField : public BaseField<size_t>
{
  public:
    ClientBodySizeField();
    ClientBodySizeField(size_t val);
    virtual ~ClientBodySizeField();
    ClientBodySizeField(ClientBodySizeField const &other);
    ClientBodySizeField &operator=(ClientBodySizeField const &other);

    int processValue(std::string value);
};

#endif // DEBUG
