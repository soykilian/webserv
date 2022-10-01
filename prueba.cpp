#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/ip.h>

#define PORT 8081

int main(void)
{
	int fd, new_socket;
	//char *hello = "HOLA CLIENTE!";
	char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	socklen_t	addrlen;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Cannot create socket"<< std::endl;
		return 1;
	}
	struct sockaddr_in address;
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
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
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
		close(new_socket);
	}

}
