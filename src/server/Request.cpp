#include "Request.hpp"
#include <sys/socket.h>

#define BUFFER_SIZE 4096

Request::~Request() {}

Request::Request(int fd)
{
    char        buffer[BUFFER_SIZE];
    std::string line;
    int         i, j;
    int         rc;
    std::string key;
    std::string value;
    int         state = 0;
    char        c;

    std::cout << "Request constructor" << std::endl;

    this->bufferLeft = "";

    j = 0;

    while (1)
    {
        rc = recv(fd, buffer, BUFFER_SIZE, 0);
        i  = 0;
        if (rc <= 0)
            break;
        this->bufferLeft += buffer;
        while ((c = this->bufferLeft[i]) != 0)
        {
            if (c == '\n')
            {
                line = this->bufferLeft.substr(j, i - j - 1);
                if (state == 0)
                {
                    std::stringstream ss(line);
                    ss >> this->method >> this->route >> this->version;
                    state = 1;
                }
                else if (state == 1)
                {
                    std::stringstream ss(line);
                    ss >> key >> value;
                    this->headers[key] = value;
                }
                j    = i;
                line = "";
            }
            i++;
        }
        this->bufferLeft = this->bufferLeft.substr(j, this->bufferLeft.size());
        if (rc <= BUFFER_SIZE)
            break;
    }
    // while (1)
    // {
    //     rc = recv(fd, buffer, BUFFER_SIZE, 0);
    //     i  = 0;
    //     if (rc <= 0)
    //         break;
    //     // for (int i = 0; i < rc || buffer[i] != '\n' || buffer[i] != '\r';
    //     // i++)
    //     while (i < rc && buffer[i] != '\n' && buffer[i] != '\r')
    //         line += buffer[i];
    // }
    this->body = this->bufferLeft.substr(1);
    std::cout << "Body: " << this->body << std::endl;
}

Request::Request() {}

Request::Request(Request const &src) { *this = src; }

Request &Request::operator=(Request const &rhs)
{
    if (this != &rhs)
    {
        this->headers = rhs.headers;
    }
    return *this;
}

std::string Request::getHeader(std::string key) const
{
    std::map<std::string, std::string>::const_iterator it;

    it = this->headers.find(key);
    if (it != this->headers.end())
        return it->second;
    return "";
}

std::string Request::getBody() const { return this->body; }

std::string Request::getMethod() const { return this->method; }
