#include "Config.hpp"

const std::string Config::DEFAULT_FILE_NAME = "webserv.conf";

void Config::loadConfig()
{
    std::ifstream configFile(this->DEFAULT_FILE_NAME);

    if (!configFile.is_open())
    {
        std::cerr << "Error: Could not open config file: " << DEFAULT_FILE_NAME << std::endl;
        exit(1);
    }
    std::string line;
    state currentState = GENERAL;
    while (std::getline(configFile, line))
    {
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;
        if (line[0] == '}')
        {
            currentState = GENERAL;
            continue;
        }
        if (line[0] == '{')
        {
            currentState = SERVER;
            continue;
        }
        if (currentState == GENERAL)
        {
            std::cout << "GENERAL: " << line << std::endl;
        }
        else if (currentState == SERVER)
        {
            std::cout << "SERVER: " << line << std::endl;
        }
        else if (currentState == LOCATION)
        {
            std::cout << "LOCATION: " << line << std::endl;
        }
    }
}

Config::Config()
{
    loadConfig();
}

Config &Config::instance()
{
    static Config config;
    return config;
}
