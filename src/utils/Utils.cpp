#include "Utils.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

bool ft::is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}