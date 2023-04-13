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

    return 0;
}
