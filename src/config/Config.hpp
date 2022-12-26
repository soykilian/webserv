#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

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

    // enum state
    // {
    //     GENERAL,
    //     SERVER_0,
    //     SERVER_1,
    //     LOCATION_0,
    //     LOCATION_1
    // };
    
    std::vector<std::string> tokens;

    bool nextToken(std::string &token);

    bool endOfProprty;

  public:
    // This is the only way to get an instance of the class
    static Config &instance();
};

#endif // DEBUG
