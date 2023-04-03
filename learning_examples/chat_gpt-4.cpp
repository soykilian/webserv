#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

const int PORT = 4242;

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return 1;
    }

    // Set server address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
        -1)
    {
        perror("bind");
        return 1;
    }

    // Listen for connections
    if (listen(server_fd, 10) == -1)
    {
        perror("listen");
        return 1;
    }

    // Initialize poll structure
    struct pollfd fds[2];
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    while (true)
    {
        int poll_count = poll(fds, 1, -1);

        if (poll_count == -1)
        {
            perror("poll");
            return 1;
        }

        if (fds[0].revents & POLLIN)
        {
            // Accept incoming connection
            if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                                    &client_addr_len)) == -1)
            {
                perror("accept");
                return 1;
            }

            // Receive message
            ssize_t bytes_received;
            while ((bytes_received =
                        recv(client_fd, buffer, sizeof(buffer), 0)) > 0)
            {
                // Send received message back
                if (send(client_fd, buffer, bytes_received, 0) == -1)
                {
                    perror("send");
                    return 1;
                }
            }

            if (bytes_received == -1)
            {
                perror("recv");
                return 1;
            }

            // Close client connection
            close(client_fd);
        }
    }

    // Close server socket
    close(server_fd);

    return 0;
}
