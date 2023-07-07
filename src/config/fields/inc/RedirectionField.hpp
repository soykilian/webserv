#ifndef REDIRECTION_FIELD_HPP
#define REDIRECTION_FIELD_HPP

#include <Base.hpp>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <Utils.hpp>
#include <sstream>

class RedirectionField : public BaseField<std::string>
{
  public:
    RedirectionField();
    RedirectionField(std::string val);
    virtual ~RedirectionField();
    RedirectionField(RedirectionField const &other);
    RedirectionField &operator=(RedirectionField const &other);
    std::string getUri() const;
    std::string getCode() const;
    std::string getRedirectionMessage() const;
    void setCode(std::string code);
    void setUri(std::string uri);

    int processValue(std::string value);
  private:
    std::string code;
    std::string uri;
    std::map<std::string, std::string> redirections_messages;

};

#endif // DEBUG
