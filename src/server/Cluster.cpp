#include <Cluster.hpp>
#include <Request.hpp>
#include <Response.hpp>
#include <cstddef>
#include <poll.h>
#include <vector>

Cluster::~Cluster() {}

bool Cluster::start()
{
    // Create a vector of file descriptors
    std::vector<int> fds;

    for (size_t i = 0; i < this->servers.size(); i++)
        fds.push_back(this->servers[i]->server_listen());

    // Variables to store all the pollfd
    size_t              fd_listeners = fds.size();
    std::vector<pollfd> pfds(fds.size());
    // Variables to accept new connections
    int                     new_socket;
    struct sockaddr_storage address;
    socklen_t               addrlen;
    // Variables to store all the data received from the clients
    std::map<int, Request *> requests;

    // Store all the file descriptors in the pollfd vector
    for (size_t i = 0; i < fds.size(); ++i)
    {
        pfds[i].fd     = fds[i];
        pfds[i].events = POLLIN;
    }

    // Polling loop
    while (1)
    {
        int poll_count = poll(&pfds[0], pfds.size(), -1);

        // std::cout << "Poll_count" << poll_count << std::endl;
        // std::cout << "pfds size" << pfds.size() << std::endl;

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
                    Request *newRequest =
                        new Request(new_socket, *this->servers[i]);

                    requests[new_socket] = newRequest;
                }
                else
                {
                    int res;
                    if ((res = requests[pfds[i].fd]->read()) == 1)
                        pfds[i].events = POLLOUT;
                    else if (res == 2)
                    {
                        close(pfds[i].fd);
                        pfds.erase(pfds.begin() + i);
                    }
                }
            }
            else if (pfds[i].revents & POLLOUT)
            {
                if (requests[pfds[i].fd]->getState() == 0)
                {
                    close(pfds[i].fd);
                    pfds.erase(pfds.begin() + i);
                }
                else
                {
                    Response   *response = new Response(requests[pfds[i].fd]);
                    std::string message  = response->getResponse();
                    write(pfds[i].fd, message.c_str(), message.length());
                    close(pfds[i].fd);
                    pfds.erase(pfds.begin() + i);
                }
            }
        }
    }
    return true;
}
