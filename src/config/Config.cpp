#include "Config.hpp"
#include <cctype>

const std::string Config::DEFAULT_FILE_NAME = "webserv.conf";

/**
 * The location propertie is an object itself, so we need to parse it
 * the same way we parse the server object
 */
bool Config::processLocation()
{
    if (this->nextToken() && this->token != "{")
        return false;
    return (true);
}

bool Config::callProcessServerProperty()
{
    // Store the current token containing the property name
    // and the next tokens containing the property value
    // until the ; is found
    std::string value;
    std::string key;

    key = this->token;
    if (key == "location")
        return (this->processLocation());
    while (this->nextToken() && this->token != ";")
        value += this->token;
    if (this->token != ";")
    {
        std::cout << "Error: expected ;" << std::endl;
        return false;
    }
    if ((this->currentServer->*currentServer->propertyMap[key])(value))
    {
        std::cout << "Error: invalid value for " << key << std::endl;
        return false;
    }
    return true;
}

bool Config::parseServer()
{

    if (nextToken() && this->token != "{")
    {
        std::cout << "Error: expected {" << std::endl;
        return false;
    }
    while (this->nextToken() && this->token != "}")
    {
        if (this->currentServer->propertyMap.find(this->token) !=
            this->currentServer->propertyMap.end())
            return this->callProcessServerProperty();
        else
        {
            std::cout << "Error: unknown property " << this->token << std::endl;
            return false;
        }
    }
    if (this->token != "}")
    {
        std::cout << "Error: expected }" << std::endl;
        return false;
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
    return (true);
}

/*
 * This functions is the entry point for the parsing
 * of the config file.  It will call the nextToken
 * to get each token and then call the parseServer
 * function to parse the server object.
 */
void Config::loadConfig()
{
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
            this->servers.push_back(new Server());
            this->currentServer = this->servers.back();
            this->parseServer();
        }
    }
    this->configFile.close();
}

Config::Config()
{
    this->endOfProprty = false;
    loadConfig();
}

Config &Config::instance()
{
    static Config config;
    return config;
}
