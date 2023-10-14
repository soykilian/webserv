# Webserv: Your HTTP Server

## Introduction

"Webserv" is a project aimed at creating an HTTP server using C++98. The server is designed to manage HTTP, a pivotal protocol for data communication on the World Wide Web, and should be capable of handling various HTTP methods and status codes, interacting with web browsers, and managing client-server communication.
## Theory and Guidelines

### Understanding HTTP Servers

HTTP (Hypertext Transfer Protocol) servers are crucial in facilitating communication between clients and servers on the web. They handle requests from clients, typically web browsers, and send back responses, often in the form of web pages. Understanding the intricacies of HTTP, including its methods (GET, POST, DELETE, etc.) and status codes (200 OK, 404 Not Found, etc.), is vital for developing a robust HTTP server.

### NGINX as a Reference

NGINX is a renowned web server that can also be utilized as a reverse proxy, load balancer, and HTTP cache. It's known for its high performance, stability, rich feature set, simple configuration, and low resource consumption. NGINX’s behavior can be a benchmark for your project, especially in understanding how server_name works or how headers and answer behaviors should be structured.

### Sockets and Communication

Sockets provide an interface for programming networks at the transport layer of the internet protocol suite. They can be created using a variety of programming languages, including C++, and allow for the implementation of network communication between a client program and a server program. Understanding socket programming, including creating a socket, binding it to a specific address and port, and setting it to listen, is fundamental in managing client-server communication.

### Polling from Sockets

Polling is a mechanism that allows a program to manage multiple data streams, such as from sockets, simultaneously. It enables your server to handle multiple connections at once, without being blocked by slow connections. The `poll()` function, or its equivalent, checks the state of a socket in a non-blocking way and is crucial in ensuring that your server remains non-blocking and can manage multiple I/O operations between the client and the server.


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



