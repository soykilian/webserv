#include <Cluster.hpp>
#include <cstddef>
#include <poll.h>
#include <vector>

Cluster::~Cluster() {}

bool Cluster::start()
{
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: "
                        "text/plain\nContent-Length: 12\n\nHello world!";

    std::vector<int> fds;

    std::cout << "----------------------------" << std::endl;
    std::cout << "+++++ Cluster starting +++++" << std::endl;

    std::cout << "++++ Starting " << this->servers.size() << " servers ++++"
              << std::endl;
    std::cout << "----------------------------" << std::endl << std::endl;

    for (size_t i = 0; i < this->servers.size(); i++)
        fds.push_back(this->servers[i]->server_listen());

    size_t fd_listeners = fds.size();
    int new_socket, rc;
    struct sockaddr_storage address;
    socklen_t addrlen;
    std::vector<pollfd> pfds(fds.size());

    for (size_t i = 0; i < fds.size(); ++i)
    {
        pfds[i].fd = fds[i];
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
                    pfd.fd = new_socket;
                    pfd.events = POLLIN;
                    pfds.push_back(pfd);
                }
                else
                {
                    printf("Reading from socket %d \n", pfds[i].fd);
                    char buffer[30000] = {0};
                    rc = recv(pfds[i].fd, buffer, 30000, 0);
                    printf("Read %d bytes:\n", rc);

                    if (rc <= 0)
                    {
                        close(pfds[i].fd);
                        pfds.erase(pfds.begin() + i);
                    }
                    else
                    {
                        Request request(buffer);
                        send(pfds[i].fd, hello.c_str(), hello.length(), 0);
                        close(pfds[i].fd);
                        pfds.erase(pfds.begin() + i);
                    }
                }
            }
            // else if (pfds[i].revents & POLLOUT)
            // {
            //     write(pfds[i].fd, hello.c_str(), strlen(hello.c_str()));
            //     printf("------------------Hello message    "
            //            "sent-------------------\n");
            //     close(pfds[i].fd); // Bye!
            //     del_from_pfds(pfds, i, &fd_count);
            // }
        }
    }
    return true;
}
