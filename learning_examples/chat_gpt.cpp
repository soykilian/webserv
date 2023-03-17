#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 4242
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, client_fd, i, j, bytes_received, bytes_sent;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];
    int fd_array[MAX_CLIENTS];

    // create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // bind server socket to address
    if (bind(server_fd, (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // set all client file descriptors to 0
    memset(fd_array, 0, sizeof(fd_array));

    // add server socket to file descriptor array
    struct pollfd fds[MAX_CLIENTS + 1];
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    while (true)
    {
        int num_fds = 1;

        // add client file descriptors to file descriptor array
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            client_fd = fd_array[i];
            if (client_fd > 0)
            {
                fds[num_fds].fd = client_fd;
                fds[num_fds].events = POLLIN;
                num_fds++;
            }
        }

        // wait for activity on any file descriptor
        int result = poll(fds, num_fds, -1);
        if (result < 0)
        {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }

        // handle incoming connections
        if (fds[0].revents & POLLIN)
        {
            int client_address_len = sizeof(client_address);
            if ((client_fd =
                     accept(server_fd, (struct sockaddr *)&client_address,
                            (socklen_t *)&client_address_len)) < 0)
            {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }

            // add new client file descriptor to array
            for (i = 0; i < MAX_CLIENTS; i++)
            {
                if (fd_array[i] == 0)
                {
                    fd_array[i] = client_fd;
                    break;
                }
            }
        }

        // handle data from clients
        for (i = 0; i < num_fds; i++)
        {
            if (fds[i].revents & POLLIN)
            {
                int fd = fds[i].fd;
                bytes_received = recv(fd, buffer, BUFFER_SIZE, 0);
                if (bytes_received < 0)
                {
                    perror("recv failed");
                    exit(EXIT_FAILURE);
                }
                else if (bytes_received == 0)
                {
                    // client closed connection
                    close(fd);
                    fd_array[i - 1] = 0;
                }
                else
                {
                    // client sent data
                    buffer[bytes_received] = '\0';
                    std::cout << "Received message from client: " << buffer
                              << std::endl;

                    // send response
                    bytes_sent = 0;
                    while (bytes_sent < bytes_received)
                    {
                        int result = send(fd, buffer + bytes_sent,
                                          bytes_received - bytes_sent, 0);
                        if (result < 0)
                        {
                            perror("send failed");
                            exit(EXIT_FAILURE);
                        }
                        bytes_sent += result;
                    }
                }
            }
        }
    }

    return 0;
