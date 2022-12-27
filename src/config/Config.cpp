#include "Config.hpp"
#include <cctype>

const std::string Config::DEFAULT_FILE_NAME = "webserv.conf";

bool Config::parseServer()
{
    std::string value;
    std::string key;

    if (nextToken() && this->token != "{")
    {
        std::cout << "Error: expected {" << std::endl;
        return false;
    }
    while (this->nextToken() && this->token != "}")
    {
        if (this->server->propertyMap.find(this->token) !=
            this->server->propertyMap.end())
        {
            value.clear();
            key = this->token;
            while (this->nextToken() && this->token != ";")
                value += this->token;
            if (this->token != ";")
            {
                std::cout << "Error: expected ;" << std::endl;
                return false;
            }
            if ((this->server->*server->propertyMap[key])(value))
            {
                std::cout << "Error: invalid value for " << key << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "Error: unknown property " << this->token << std::endl;
            return false;
        }
    }
    return true;
}

/*
 * This function recives a reference to a string
 * and returns true if there is a next token in the
 * config file.  If there is a next token, the string
 * is set to the next token.
 */
bool Config::nextToken()
{
    char c;
    this->token.clear();

    if (endOfProprty)
    {
        this->token += ';';
        this->endOfProprty = false;
        return true;
    }
    while (std::isspace(this->configFile.get()))
        ;
    this->configFile.unget();
    while (this->configFile.good())
    {
        c = this->configFile.get();
        if (std::isspace(c))
            break;
        if (c == '#')
            while (this->configFile.get() != '\n')
                ;
        if (c == ';')
        {
            this->endOfProprty = true;
            break;
        }
        this->token += c;
    }
    if (this->token.empty())
        return false;
    std::cout << this->token << std::endl;
    return (true);
}

void Config::loadConfig()
{
    this->currentServer = this->server;
    ParseState state = UNKNOWN;

    this->configFile.open(DEFAULT_FILE_NAME.c_str());
    if (!this->configFile.is_open())
    {
        std::cerr << "Error: Could not open config file: " << DEFAULT_FILE_NAME
                  << std::endl;
        exit(1);
    }

    this->tokens = std::vector<std::string>();
    while (this->nextToken())
    {
        if (state == UNKNOWN && this->token != "server")
        {
            std::cerr << "Error: Config file must start with a server block"
                      << std::endl;
            exit(1);
        }
        else if (state == UNKNOWN && this->token == "server")
        {
            state = SERVER;
            // Set the current server to the first server in the list
            if (currentServer == NULL)
            {
                this->server = new Server();
                this->currentServer = this->server;
            }
            else
            {
                this->currentServer->next = new Server();
                this->currentServer = this->currentServer->next;
            }
            this->parseServer();
        }
    }

    // while (this->nextToken(token))
    //     this->tokens.push_back(token);

    // for (size_t i = 0; i < this->tokens.size(); i++)
    // {
    //     if (state == UNKNOWN && this->tokens[0] != "server")
    //     {
    //         std::cerr << "Error: Config file must start with a server block"
    //                   << std::endl;
    //         exit(1);
    //     }
    // }

    // while (this->tokens.size())
    // {
    //     std::cout << this->tokens.front() << std::endl;
    //     this->tokens.erase(this->tokens.begin());
    // }

    this->configFile.close();
}

Config::Config()
{
    this->endOfProprty = false;
    this->server = NULL;
    loadConfig();
}

Config &Config::instance()
{
    static Config config;
    return config;
}
