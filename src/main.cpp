#include "server/Cluster.hpp"
#include "server/Request.hpp"
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

// void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int
// *fd_size)
// {
//     // If we don't have room, add more space in the pfds array
//     if (*fd_count == *fd_size)
//     {
//         *fd_size *= 2; // Double it
//
//         *pfds = (pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
//     }
//
//     (*pfds)[*fd_count].fd = newfd;
//     (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read
//
//     (*fd_count)++;
// }
//
// void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
// {
//     pfds[i] = pfds[*fd_count - 1];
//     (*fd_count)--;
// }

int main(int argc, char **argv)
{
    // Check if file name is provided
    // If it is use it if not use default defined in Makefile
    if (argc == 2)
    {
        std::cout << "Using config file: " << argv[1] << std::endl;
        std::string config_file = argv[1];
        Config::setFileName(config_file);
    }
    Config &globalConfig = Config::instance();

    Cluster *cluster = new Cluster(globalConfig.servers);

    cluster->start();

    //
    // std::string hello = "HTTP/1.1 200 OK\nContent-Type: "
    //                     "text/plain\nContent-Length: 12\n\nHello world!";
    // int fd = globalConfig.servers[0]->server_listen();
    // int fd_count = 1;
    // int fd_size = 5;
    // int new_socket, rc;
    // struct sockaddr_storage address;
    // socklen_t addrlen;
    // struct pollfd *pfds = (pollfd *)malloc(sizeof *pfds * fd_size);
    // pfds[0].fd = fd;
    // pfds[0].events = POLLIN;
    // while (1)
    // {
    //     int poll_count = poll(pfds, fd_count, -1);
    //
    //     if (poll_count == -1)
    //     {
    //         perror("poll");
    //         exit(1);
    //     }
    //     for (int i = 0; i < fd_count; i++)
    //     {
    //         if (pfds[i].revents & POLLIN)
    //         {
    //             if (pfds[i].fd == fd)
    //             {
    //                 if ((new_socket = accept(fd, (struct sockaddr *)&address,
    //                                          (socklen_t *)&addrlen)) < 0)
    //                 {
    //                     perror("In accept");
    //                     exit(EXIT_FAILURE);
    //                 }
    //                 add_to_pfds(&pfds, new_socket, &fd_count, &fd_size);
    //             }
    //             else
    //             {
    //                 printf("Reading from socket %d \n", pfds[i].fd);
    //                 char buffer[30000] = {0};
    //                 rc = recv(pfds[i].fd, buffer, 30000, 0);
    //                 printf("Read %d bytes:\n", rc);
    //
    //                 if (rc <= 0)
    //                 {
    //                     close(pfds[i].fd);
    //                     del_from_pfds(pfds, i, &fd_count);
    //                 }
    //                 else
    //                 {
    //                     Request request(buffer);
    //                     send(pfds[i].fd, hello.c_str(), hello.length(), 0);
    //                     close(pfds[i].fd);
    //                     del_from_pfds(pfds, i, &fd_count);
    //                 }
    //             }
    //         }
    // else if (pfds[i].revents & POLLOUT)
    // {
    //     write(pfds[i].fd, hello.c_str(), strlen(hello.c_str()));
    //     printf("------------------Hello message    "
    //            "sent-------------------\n");
    //     close(pfds[i].fd); // Bye!
    //     del_from_pfds(pfds, i, &fd_count);
    // }
    return 0;
}
