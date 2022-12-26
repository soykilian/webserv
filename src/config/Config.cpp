#include "Config.hpp"
#include <cctype>

const std::string Config::DEFAULT_FILE_NAME = "webserv.conf";

bool Config::nextToken(std::string &token)
{
    token.clear();

    if (endOfProprty)
    {
        token += ';';
        endOfProprty = false;
        return true;
    }
    while (std::isspace(this->configFile.get()));
    this->configFile.unget();
    while(this->configFile.good())
    {
        char c = this->configFile.get();
        if (std::isspace(c))
            break;
        if (c == '#')
            while (this->configFile.get() != ' ');
        if (c == ';')
            break;
        token += c;
    }
    if (token.empty())
        return false;
    return (true);
}

void Config::loadConfig()
{
    std::string token;

    this->configFile.open(DEFAULT_FILE_NAME.c_str());

    if (!this->configFile.is_open())
    {
        std::cerr << "Error: Could not open config file: " << DEFAULT_FILE_NAME << std::endl;
        exit(1);
    }

    this->tokens = std::vector<std::string>();

    while (this->nextToken(token))
        this->tokens.push_back(token);

    while (this->tokens.size())
    {
        std::cout << this->tokens.front() << std::endl;
        this->tokens.erase(this->tokens.begin());
    }

    this->configFile.close();
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
