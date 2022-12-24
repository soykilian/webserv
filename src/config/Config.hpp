#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <iostream>
#include <string>

// This class is a singleton that holds the configuration for the server.
// This example is not thread safe, but it could be made thread safe by
// using a mutex to lock the instance() method.

class Config
{
  private:
    static const std::string DEFAULT_FILE_NAME;

    void loadConfig();

    // No need for the constructor to be public
    Config();
    Config(const Config &);            // Don't implement
    Config &operator=(const Config &); // Don't implement

    enum state
    {
        GENERAL,
        SERVER,
        LOCATION
    };

  public:
    // This is the only way to get an instance of the class
    static Config &instance();
};

#endif // DEBUG
