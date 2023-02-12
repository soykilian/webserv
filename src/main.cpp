#include <Config.hpp>
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *pfds =(pollfd*) realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}

void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    pfds[i] = pfds[*fd_count-1];
    (*fd_count)--;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    Config &globalConfig = Config::instance();
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: "
                        "text/plain\nContent-Length: 12\n\nHello world!";
    int fd = globalConfig.servers[0]->server_listen();
    int fd_count;
    int fd_size = 5;
    int new_socket;
    struct sockaddr_storage address;
    socklen_t addrlen;
    struct pollfd *pfds = (pollfd *)malloc(sizeof *pfds * fd_size);
    pfds[0].fd = fd;
    pfds[0].events = POLLIN;
    fd_count = 1;
    while (1)
    {
        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }
        for (int i = 0; i < fd_count; i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                if (pfds[i].fd == fd)
                {
                    if ((new_socket = accept(fd, (struct sockaddr *)&address,
                                             (socklen_t *)&addrlen)) < 0)
                    {
                        perror("In accept");
                        exit(EXIT_FAILURE);
                    }
                    add_to_pfds(&pfds, new_socket, &fd_count, &fd_size);
                }
                else
                {
                    char buffer[30000] = {0};
                    read(pfds[i].fd, buffer, 30000);
                    printf("%s\n", buffer);
                    pfds[i].events=POLLOUT;
                }
            }
            else if (pfds[i].revents & POLLOUT)
            {
                write(pfds[i].fd, hello.c_str(), strlen(hello.c_str()));
                printf("------------------Hello message sent-------------------\n");
                close(pfds[i].fd); // Bye!
                del_from_pfds(pfds, i, &fd_count);
            }
        }
    }
    return 0;
}
