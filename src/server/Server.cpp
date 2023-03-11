#include "Server.hpp"
#include "fields/ClientBodySizeField.hpp"
#include "fields/ErrorPageField.hpp"
#include "fields/IndexField.hpp"
#include "fields/ListenField.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <netinet/ip.h>

Server::Server()
{
    this->fields["listen"] = new ListenField();
    this->fields["location"] = new Location();
    this->fields["server_name"] = new ServerNameField();
    this->fields["root"] = new RootField();
    this->fields["error_page"] = new ErrorPageField();
    this->fields["client_body_size"] = new ClientBodySizeField();
    this->fields["index"] = new IndexField();
}

Server::~Server() {}

Server::Server(const Server &) {}

Server &Server::operator=(const Server &) { return (*this); }

void Server::setDefaultErrPage()
{
    ErrorPageField *defErrPage =
        dynamic_cast<ErrorPageField *>(this->fields["error_page"]);

    if (!defErrPage->isSet())
        defErrPage->setValue("./error_page/error.html");
}

bool Server::validate()
{
    if (!dynamic_cast<ListenField *>(this->fields["listen"])->isSet())
    {
        std::cout << "Listen field is not set." << std::endl;
        return false;
    }
    if (!dynamic_cast<ErrorPageField *>(this->fields["error_page"])->isSet())
        this->setDefaultErrPage();

    return true;
}

int Server::getPort() const
{
    return (dynamic_cast<ListenField *>(this->fields.at("listen"))->getPort());
}

std::string Server::getHost() const
{
    return (dynamic_cast<ListenField *>(this->fields.at("listen"))->getHost());
}

std::string Server::getServerName() const
{
    return (dynamic_cast<ServerNameField *>(this->fields.at("server_name"))
                ->getValue());
}

std::string Server::getRoot() const
{
    return (dynamic_cast<RootField *>(this->fields.at("root"))->getValue());
}

std::string Server::getErrorPage() const
{
    return (dynamic_cast<ErrorPageField *>(this->fields.at("error_page"))
                ->getValue());
}

int Server::getClientBodySize() const
{
    return (
        dynamic_cast<ClientBodySizeField *>(this->fields.at("client_body_size"))
            ->getValue());
}

std::ostream &operator<<(std::ostream &out, Server const &server)
{
    out << "Server: " << std::endl;
    out << "Port: " << server.getPort() << std::endl;
    out << " Host: " << server.getHost() << std::endl;
    out << " ServerName: " << server.getServerName() << std::endl;
    out << " Root: " << server.getRoot() << std::endl;
    out << " ErrorPage: " << server.getErrorPage() << std::endl;
    out << " ClientBodySize: " << server.getClientBodySize() << std::endl;
    out << std::endl;
    return (out);
}

int Server::server_listen()
{
    struct sockaddr_in address;
    int fd;
    int yes = 1;
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: "
                        "text/plain\nContent-Length: 12\n\nHello world!";
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Cannot create socket" << std::endl;
        return 1;
    }
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr =
        htonl(static_cast<uint32_t>(inet_addr(this->getHost().c_str())));
    address.sin_port = htons(this->getPort());
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        return 1;
    }
    if (listen(fd, 3) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << this->getPort() << std::endl;

    // while (1)
    // {
    // 	printf("\n+++++++ Waiting for new connection ++++++++\n\n");
    // 	if ((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t
    // *)&addrlen))< 0)
    // 	{
    // 		perror("In accept");
    // 		exit(EXIT_FAILURE);
    // 	}
    //
    //        char buffer[30000] = {0};
    //        read( new_socket , buffer, 30000);
    //        printf("%s\n",buffer );
    //        write(new_socket , hello.c_str(), strlen(hello.c_str()));
    //        printf("------------------Hello message
    //        sent-------------------\n");
    // 	close(new_socket);
    // }
    return (fd);
}
