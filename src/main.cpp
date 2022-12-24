#include <Config.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    Config &globalConfig = Config::instance();
    (void)globalConfig;
    return 0;
}
