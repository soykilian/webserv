This system call are in the same exact (approximately) order that they need to be called in.


# `getaddrinfo()`

This structure as mentioned in the [[Structs#addrinfo |addrinfo]] section will help set up the `struct`s needed later.

``` c++
int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
                const char *service,  // e.g. "http" or port number
                const struct addrinfo *hints,
                struct addrinfo **res);
```

`const char *node` is the host name you want to connect to, or an IP address

`const char *service` is the port number or the name of the specific service (found in [The IANA Port List](https://www.iana.org/assignments/port-numbers)[20](https://beej.us/guide/bgnet/html/split/footnotes.html#fn20) or the `/etc/services` file on your Unix machine) like “http” or “ftp” or “telnet” or “smtp” or whatever.

`const struct addrinfo *hints`  is a pointer to a structure you need to fill with some info


## Examples

### Server that listens on your host IP address in port 3490

``` c++
int status;
struct addrinfo hints;
struct addrinfo *servinfo;  // will point to the results

memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
//hints.ai_family = AF_INET;     // Only IPv4
//hints.ai_family = AF_INET6;     // Only IPv6
hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
}

// servinfo now points to a linked list of 1 or more struct addrinfos

// ... do everything until you don't need servinfo anymore ....

freeaddrinfo(servinfo); // free the linked-list
```

### Client that connect to "www.example.com" port 3490

``` c++
int status;
struct addrinfo hints;
struct addrinfo *servinfo;  // will point to the results

memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

// get ready to connect
status = getaddrinfo("www.example.net", "3490", &hints, &servinfo);

// servinfo now points to a linked list of 1 or more struct addrinfos

// etc.
```


Let’s write a quick demo program to show off this information. [[System_calls_example#`getaddrinfo()` examples:|This short program]] will print the IP addresses for whatever host you specify on the command line: