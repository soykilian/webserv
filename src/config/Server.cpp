#include "Server.hpp"
#include <arpa/inet.h>
#include <netinet/ip.h>
#include "fields/ClientBodySizeField.hpp"

Server::Server()
{
    this->fields["listen"] = new ListenField();
    this->fields["location"] = new Location();
    this->fields["server_name"] = new ServerNameField();
    this->fields["root"] = new RootField();
    this->fields["error_page"] = new ErrorPageField();
    this->fields["client_body_size"] = new ClientBodySizeField();
}

Server::~Server() {}

Server::Server(const Server &) {}

Server &Server::operator=(const Server &) { return (*this); }

void Server::validate()
{
    for (std::map<std::string, Base *>::iterator it = this->fields.begin();
         it != this->fields.end(); it++)
    {
    }
}

int Server::getPort() const
{
    return (dynamic_cast<ListenField *>(this->fields.at("listen"))->getPort());
}

std::string Server::getHost() const
{
    return (dynamic_cast<ListenField *>(this->fields.at("listen"))->getHost());
}

std::ostream &operator<<(std::ostream &out, Server const &server)
{
    out << "Server: ";
    out << "Port: " << server.getPort() << " Host: " << server.getHost();
    return (out);
}


int Server::server_listen()
{
    struct sockaddr_in address;
    int fd, new_socket;
	int yes = 1;
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    socklen_t addrlen;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Cannot create socket"<< std::endl;
		return 1;
	}
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(static_cast<uint32_t>(inet_addr(this->getHost().c_str())));
    address.sin_port=htons(this->getPort());
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&yes , sizeof(int));
	if (bind(fd, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		perror("In bind");
		return 1;
	}
	if (listen(fd,3) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t *)&addrlen))< 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}

        char buffer[30000] = {0};
        read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , hello.c_str(), strlen(hello.c_str()));
        printf("------------------Hello message sent-------------------\n");
		close(new_socket);
	}
}
