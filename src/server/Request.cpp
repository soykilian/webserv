#include "Request.hpp"
#include <sys/socket.h>
#include <vector>

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

int Request::getState() const { return this->state; }

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

std::string Request::getHost() const { return this->host; }

void Request::limitBody()
{
    std::map<std::string, std::string>::iterator iterator =
        this->headers.find("Content-Length");

    if (iterator != this->headers.end())
    {
        int maxLength = std::stoi(iterator->second);
        if (static_cast<int>(this->body.length()) > maxLength)
            this->body = this->body.substr(0, maxLength + 1);
    }
}

int Request::read()
{
    char        buffer[BUFFER_SIZE + 1];
    char        c;
    int         rc, i, j;
    std::string line, key, value;

    j = 0;

    rc = recv(this->fd, buffer, BUFFER_SIZE, 0);
    i  = 0;
    if (rc == -1)
        return 2;
    if (rc == 0)
        return 1;
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
                key.erase(key.end() - 1);
                this->headers[key] = value;
                if (key == "Host")
                    this->host = value;
            }
            j = i;
        }
        i++;
        if (this->state == 2)
            break;
    }
    if (this->state == 2)
    {
        if (this->body.empty())
            j++;
        this->body += this->bufferLeft.substr(j);
        limitBody();
        this->bufferLeft.clear();
    }
    else
        this->bufferLeft = this->bufferLeft.substr(j);
    if (rc < BUFFER_SIZE)
        return 1;
    return 0;
}

std::ostream &operator<<(std::ostream &out, Request const &req)
{
    out << "Request: " << std::endl;
    out << "\t Version: " << req.getVersion() << std::endl;
    out << "\t Method: " << req.getMethod() << std::endl;
    out << "\t Route: " << req.getRoute() << std::endl;

    return out;
}
