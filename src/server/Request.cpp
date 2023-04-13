#include "Request.hpp"
#include <sys/socket.h>

#define BUFFER_SIZE 4096

Request::~Request() {}

Request::Request(int fd, Server const &server)
    : fd(fd), state(0), server(server)
{
}

Request::Request() : server(this->empty) {}

Request::Request(Request const &src) : server(src.server) { *this = src; }

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

std::string Request::getRoute() const { return this->route; }

std::string Request::getVersion() const { return this->version; }

const Server &Request::getServer() const { return this->server; }

bool Request::read()
{
    char        buffer[BUFFER_SIZE + 1];
    char        c;
    int         rc, i, j;
    std::string line, key, value;

    j = 0;

    rc = recv(this->fd, buffer, BUFFER_SIZE, 0);
    i  = 0;
    if (rc <= 0)
        return true;
    buffer[rc] = 0;
    this->bufferLeft += buffer;
    while ((c = this->bufferLeft[i]) != 0)
    {
        if (c == '\n')
        {
            if (j == i - 2)
                this->state = 2;
            line = this->bufferLeft.substr(j, i - j - 1);
            if (this->state == 0)
            {
                std::stringstream ss(line);
                ss >> this->method >> this->route >> this->version;
                this->state = 1;
            }
            else if (this->state == 1)
            {
                std::stringstream ss(line);
                ss >> key >> value;
                this->headers[key] = value;
            }
            j = i;
        }
        i++;
        if (this->state == 2)
            break;
    }
    if (this->state == 2)
    {
        this->body += this->bufferLeft.substr(j);
        this->bufferLeft.clear();
    }
    else
        this->bufferLeft = this->bufferLeft.substr(j);
    if (rc < BUFFER_SIZE)
        return true;
    return false;
}
