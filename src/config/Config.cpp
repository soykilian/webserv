#include "Config.hpp"
#include <cctype>

const std::string Config::DEFAULT_FILE_NAME = CONFIG_FILE;
std::string Config::USER_SPECIFIED_FILE_NAME = "";

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
        {
            c = this->configFile.get();
            while (c != '\n')
            {
                std::cout << c;
                c = this->configFile.get();
            }
            c = this->configFile.get();
            std::cout << c;
            while (std::isspace(c))
                c = this->configFile.get();
        }
        if (c == ';')
        {
            this->endOfProprty = true;
            break;
        }
        // std::cout << c;
        this->token += c;
    }
    if (this->token.empty())
        return false;
    return (true);
}

bool Config::callProcessLocationProperty()
{
    // Store the current token containing the property name
    // and the next tokens containing the property value
    // until the ; is found
    std::string value;
    std::string key;

    key = this->token;
    while (this->nextToken() && this->token != ";")
        value += this->token;
    if (this->token != ";")
    {
        std::cout << "Error: expected ;" << std::endl;
        return false;
    }
    if (!this->currentLocation->fields[key]->processValue(value))
    {
        std::cout << "Error: invalid value for " << key << std::endl;
        return false;
    }
    return true;
}

/**
 * The location propertie is an object itself, so we need to parse it
 * the same way we parse the server object
 * The location has the following format:
 *
 * location /path/to/location {
 *     property1 value1;
 *     ...
 * }
 *
 */
bool Config::processLocation()
{
    this->currentLocation = new Location();
    if (!this->nextToken() || !this->currentLocation->processValue(this->token))
    {
        std::cout << "Invalid location route" << std::endl;
        return false;
    }
    if (!this->nextToken() || this->token != "{")
    {
        std::cout << "Invalid location route" << std::endl;
        return false;
    }
    while (this->nextToken() && this->token != "}")
    {
        if (this->currentLocation->fields.find(this->token) ==
            this->currentLocation->fields.end())
        {
            std::cout << "Error: unknown property " << this->token << std::endl;
            return false;
        }
        if (!this->callProcessLocationProperty())
            return false;
    }
    if (this->token != "}")
    {
        std::cout << "Error: expected }" << std::endl;
        return false;
    }
    // ------------------- PRINT LOCATION -------------------
    std::cout << *this->currentLocation << std::endl;
    this->currentServer->locations.push_back(this->currentLocation);
    return (true);
}

bool Config::callProcessServerProperty()
{
    // Store the current token containing the property name
    // and the next tokens containing the property value
    // until the ; is found
    std::string value;
    std::string key;
    Base *curr;

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
    curr = this->currentServer->fields[key];
    if (curr->isPath())
        this->pendingPaths[key] = value;
    if (curr->isSet())
    {
        std::cerr << key << " is already set." << std::endl;
        return false;
    }
    if (!curr->processValue(value))
        return false;
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
        if (this->currentServer->fields.find(this->token) ==
            this->currentServer->fields.end())
        {
            std::cout << "Error: unknown property " << this->token << std::endl;
            return false;
        }
        if (!this->callProcessServerProperty())
            return false;
    }
    if (this->token != "}")
    {
        std::cout << "Error: expected }" << std::endl;
        return false;
    }
    return true;
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
            if (!this->parseServer() || !this->currentServer->validate())
            {
                std::cerr << "Error: Invalid server block" << std::endl;
                exit(1);
            }
            // ------------------- PRINT SERVER -------------------
            std::cout << *this->currentServer << std::endl;
        }
    }
}

Config::Config()
{
    // Open the config file
    // USER_SPECIFIED_FILE_NAME is priorized
    // DEFAULT_CONFIG_FILE_NAME is used if the user didn't specify a config file
    this->configFile.open(!USER_SPECIFIED_FILE_NAME.empty()
                              ? USER_SPECIFIED_FILE_NAME.c_str()
                              : DEFAULT_FILE_NAME.c_str());
    if (!this->configFile.is_open())
    {
        std::cerr << "Error: Could not open config file: "
                  << (USER_SPECIFIED_FILE_NAME.empty()
                          ? DEFAULT_FILE_NAME
                          : USER_SPECIFIED_FILE_NAME)
                  << std::endl;
        exit(1);
    }
    this->endOfProprty = false;
    this->tokens = std::vector<std::string>();
    loadConfig();
    this->configFile.close();
}

Config &Config::instance()
{
    static Config config;
    return config;
}
void Config::setFileName(std::string &fileName)
{
    USER_SPECIFIED_FILE_NAME = fileName;
}
