#include "Request.hpp"

Request::~Request() {}

Request::Request(std::string request)
{
    std::cout << "Request: " << std::endl;
    // read the request line by line
    std::string line;
    std::istringstream ss(request);
    while (std::getline(ss, line))
    {
        std::cout << line << std::endl;
    }
}
