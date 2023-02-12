#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Server.hpp"
#include <Fields.hpp>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

// This class is a singleton that holds the configuration for the server.
// This example is not thread safe, but it could be made thread safe by
// using a mutex to lock the instance() method.

class Config
{
  private:
    static const std::string DEFAULT_FILE_NAME;

    std::ifstream configFile;

    void loadConfig();

    // No need for the constructor to be public
    Config();
    Config(const Config &);            // Don't implement
    Config &operator=(const Config &); // Don't implement

    // This is the vector that holds the tokens from the config file
    std::vector<std::string> tokens;

    // This is a helper function that returns the next token from the config
    // file
    bool nextToken();

    // This variable keeps track of whether or not we are at the end of a
    // property
    bool endOfProprty;

    // Linked list of servers
    public:
      std::vector<Server *> servers;

    typedef enum
    {
        SERVER,
        LOCATION,
        UNKNOWN
    } ParseState;

    bool callProcessLocationProperty();
    bool processLocation();
    bool callProcessServerProperty();
    bool parseServer();

    // Variable to hold the current server been parsed
    Server *currentServer;

    // Variable to hold the current server been parsed
    Location *currentLocation;

    // Variable to hold the current token been parsed
    std::string token;

  public:
    // This is the only way to get an instance of the class
    static Config &instance();
};

#endif // DEBUG
