# Webserv: Your Own HTTP Server

## Introduction

"Webserv" is a project aimed at creating an HTTP server using C++98. The server is designed to manage HTTP, a pivotal protocol for data communication on the World Wide Web, and should be capable of handling various HTTP methods and status codes, interacting with web browsers, and managing client-server communication.

## Project Guidelines

### Key Features

- **Non-blocking Server**: Utilize only one poll() (or equivalent) for all I/O operations and ensure requests never hang.
- **HTTP Methods**: Support at least GET, POST, and DELETE methods.
- **Configuration File Management**: Manage settings like choosing the port and host, setting up server names, defining default error pages, and more.
- **Resilience**: Ensure the server remains available under stress tests and various conditions.
  
### Configuration File

In the configuration file, you should be able to:
- Choose the port and host of each 'server'.
- Setup the server_names.
- Setup default error pages.
- Limit client body size.
- Setup routes with specific rules/configuration.
- [More details...](#)

## How to Use

*Steps on how to use the project will be added here.*



