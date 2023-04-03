#include <Cluster.hpp>
#include <Request.hpp>
#include <Response.hpp>
#include <cstddef>
#include <poll.h>
#include <vector>

Cluster::~Cluster() {}

bool Cluster::start()
{
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: "
                        "text/plain\nContent-Length: 12\n\nHello world!";

    std::vector<int> fds;
    // std::map<int, >

#ifdef DEBUG
    std::cout << "----------------------------" << std::endl;
    std::cout << "+++++ Cluster starting +++++" << std::endl;

    std::cout << "++++ Starting " << this->servers.size() << " servers ++++"
              << std::endl;
    std::cout << "----------------------------" << std::endl << std::endl;
#endif

    for (size_t i = 0; i < this->servers.size(); i++)
        fds.push_back(this->servers[i]->server_listen());

    size_t fd_listeners = fds.size();
    int    new_socket;
    // int                    rc;
    struct sockaddr_storage  address;
    socklen_t                addrlen;
    std::vector<pollfd>      pfds(fds.size());
    std::map<int, Request *> requests;

    for (size_t i = 0; i < fds.size(); ++i)
    {
        pfds[i].fd     = fds[i];
        pfds[i].events = POLLIN;
    }
    while (1)
    {
        int poll_count = poll(&pfds[0], pfds.size(), -1);

        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }
        for (size_t i = 0; i < pfds.size(); i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                if (i < fd_listeners)
                {
                    if ((new_socket =
                             accept(pfds[i].fd, (struct sockaddr *)&address,
                                    (socklen_t *)&addrlen)) < 0)
                    {
                        perror("In accept");
                        exit(EXIT_FAILURE);
                    }
                    pollfd pfd;
                    pfd.fd     = new_socket;
                    pfd.events = POLLIN;
                    pfds.push_back(pfd);
                }
                else
                {
                    // printf("Reading from socket %d \n", pfds[i].fd);
                    // char buffer[30000] = {0};
                    // rc                 = recv(pfds[i].fd, buffer, 30000, 0);
                    // printf("Read %d bytes:\n", rc);
                    std::cout << "New request" << std::endl;
                    Request *newRequest = new Request(pfds[i].fd);

                    requests[pfds[i].fd] = newRequest;

                    std::cout << &newRequest << std::endl;

                    // if (rc <= 0)
                    // {
                    //     close(pfds[i].fd);
                    //     pfds.erase(pfds.begin() + i);
                    // }
                    // else
                    // {
                    // Request request(buffer);
                    // send(pfds[i].fd, hello.c_str(), hello.length(), 0);
                    // close(pfds[i].fd);
                    // pfds.erase(pfds.begin() + i);
                    pfds[i].events = POLLOUT;
                    // }
                }
            }
            else if (pfds[i].revents & POLLOUT)
            {
                // write(pfds[i].fd, hello.c_str(), strlen(hello.c_str()));
                // std::cout << requests[pfds[i].fd].getBody() << std::endl;
                std::cout << &requests[pfds[i].fd] << std::endl;
                std::string message = "HTTP/1.1 200 OK\n";
                message += "Content-Type: text/plain\n";
                message += "Content-Length: ";
                message +=
                    std::to_string(requests[pfds[i].fd]->getBody().length());
                message += "\n\n";
                message += requests[pfds[i].fd]->getBody();
                std::cout << "Message to send: " << message << std::endl;
                // write(pfds[i].fd, hello.c_str(), hello.length());
                write(pfds[i].fd, message.c_str(), message.length());
#ifdef DEBUG
                printf("------------------Hello message "
                       "sent-------------------\n");
#endif
                close(pfds[i].fd); // Bye!
                pfds.erase(pfds.begin() + i);
            }
        }
    }
    return true;
}
