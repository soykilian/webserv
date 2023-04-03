#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <Request.hpp>
#include <Server.hpp>
#include <vector>

class Cluster
{
  public:
    Cluster() : servers(this->empty_server) {}
    Cluster(std::vector<Server *> &servers) : servers(servers){};
    Cluster(Cluster &other);
    Cluster(const Cluster &);
    Cluster &operator=(Cluster &other);
    Cluster &operator=(const Cluster &other);
    ~Cluster();

    bool start();

  private:
    std::vector<Server *> &servers;
    std::vector<Server *> empty_server;
};

#endif // !CLUSTER_HPP
