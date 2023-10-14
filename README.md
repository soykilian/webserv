# Webserv: Your Own HTTP Server

## Introduction

"Webserv" is a project aimed at creating an HTTP server using C++98. The server is designed to manage HTTP, a pivotal protocol for data communication on the World Wide Web, and should be capable of handling various HTTP methods and status codes, interacting with web browsers, and managing client-server communication.

## Project Guidelines

### Key Features

- **Non-blocking Server**: Utilize only one poll() (or equivalent) for all I/O operations and ensure requests never hang.
- **HTTP Methods**: Support at least GET, POST, and DELETE methods.
- **Configuration File Management**: Manage settings like choosing the port and host, setting up server names, defining default error pages, and more.
- **Resilience**: Ensure the server remains available under stress tests and various conditions.
  
### Requirements

- The server must not crash or quit unexpectedly.
- Compile code with c++ and the flags `-Wall -Wextra -Werror`.
- Adhere to the C++98 standard.
- No use of external or Boost libraries.
- Ensure accurate HTTP response status codes.
- Provide default error pages if none are provided.
- Ensure compatibility with the chosen web browser.
- [More requirements...](#)

### For MacOS Users

- You are allowed to use `fcntl()` as follows: `fcntl(fd, F_SETFL, O_NONBLOCK);` to implement non-blocking file descriptors.

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

## Bonus Features

- Support cookies and session management.
- Handle multiple CGI.

**Note**: Bonus features will only be assessed if the mandatory part is perfect.

## Submission and Peer-evaluation

Ensure to turn in your assignment in your Git repository as usual. Only the work inside your repository will be evaluated during the defense.

## Additional Notes

- Ensure to read the RFC and perform tests with telnet and NGINX before starting this project.
- If you have any questions about behavior, compare your program behavior with NGINX’s.
- Stress-test your server to ensure availability.
- [More notes...](#)

## Contributing

*Guidelines for contributing to the project will be added here.*

## License

*Information about the project's license will be added here.*


