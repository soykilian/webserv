#include <Config.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    Config &globalConfig = Config::instance();
    globalConfig.servers[0]->server_listen();
    return 0;
}
