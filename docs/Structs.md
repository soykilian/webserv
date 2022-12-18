# Sockets

The main types of sockets i need to know are 

## SOCKET_STREAM

Reliable 2 way connection. If u send packages 1 then 2 packages will arrive and will arrive in order.

## SOCKET_DGRAM

Doesn't need a 2 way connection and packages may be lost or out of order.



# Big and little eldian

## Big eldian or Network Byte Order
- The bytes are stored the right way.
- If you are sending something over the internet you use Big Eldian
- ex: b34f will be stored as b3   4f

## Little eldian
- The bytes are stored the oposite way.
- ex: b34f will be stored as 4f   b3

|Function |Description            |
|-        |-                      |
|htons()  | host to network short |
|htonl()  | host to network long  |
|ntohs()  | network to host short |
|ntohl()  | network to host long  |


# structs

## Socket descriptor
```c++
int
```

## addrinfo

- Used to prep the socket address `structs` for subsequent use 
- It’s also used in host name lookups, and service name lookups

```c++
struct addrinfo 
{
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // use 0 for "any"
    size_t           ai_addrlen;   // size of ai_addr in bytes
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    char            *ai_canonname; // full canonical hostname
    
    struct addrinfo *ai_next;      // linked list, next node
};
```

You'll load this struct up a bit, and then call `getaddrinfo()`

## sockaddr

Holds socket address information for many types of sockets

```c++
struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
}; 
```

`sa_family` can be a variety of things, but it'll be `AF_INET` (IPv4) or `AF_INET6` (IPv6) for everything here.

`sa_data` contains a destination address and port number for this socket

To deal with this `struct` programers created a parallel struct to be used with IPv4:

## socketaddr_in (IPv4 specific)

### IMPORTANT NOTES
- A pointer to `struct sockaddr_in` can be casted to `struct sockaddr`. 
- So event though `connect()` wants a `struct sockaddr*` you can still use `struct sockaddr_in`

```c++
// (IPv4 only--see struct sockaddr_in6 for IPv6)

struct sockaddr_in {
    short int          sin_family;  // Address family, AF_INET
    unsigned short int sin_port;    // Port number
    struct in_addr     sin_addr;    // Internet address
    unsigned char      sin_zero[8]; // Same size as struct sockaddr
};
```

This struct makes it easy to reference elements of the socket address.

`sin_zero` should be set to all zeros with `memset()`

`sin_family` should be set to `AF_INET`

`sin_port` must be in Network Bite Order (use `htons()`)

`sin_addr` is a `struct in_addr`:

## in_addr

```c++
// (IPv4 only--see struct in6_addr for IPv6)

// Internet address (a structure for historical reasons)
struct in_addr {
    uint32_t s_addr; // that's a 32-bit int (4 bytes)
};
```

*This used to be a union (If your system uses it you can still reference it as if it was a struct thanks to macros)*

`s_addr` is the 4-byte IP address in Network Byte Order

Similar structs exists for IPv6

## socketaddr_in6 (IPv6 specific)

```c++
// (IPv6 only--see struct sockaddr_in and struct in_addr for IPv4)

struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port number, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};

struct in6_addr {
    unsigned char   s6_addr[16];   // IPv6 address
};
```


## sockaddr_storage

```c++
struct sockaddr_storage {
    sa_family_t  ss_family;     // address family

    // all this is padding, implementation specific, ignore it:
    char      __ss_pad1[_SS_PAD1SIZE];
    int64_t   __ss_align;
    char      __ss_pad2[_SS_PAD2SIZE];
};
```

Is designed to be large enough to hold both IPv4 and IPv6 `struct`s.

Sometimes you don't know in advance if it's going to fill out your `struct sockaddr` with an IPv4 or IPv6 address.

So you pass in this parallel structure, very similar to `struct sockaddr` except larger, and then cast it to the type you need.

What’s important is that you can see the address family in the `ss_family` field—check this to see if it’s `AF_INET` or `AF_INET6`

Then you can cast it to a `struct sockaddr_in` or `struct sockaddr_in6` if you wanna.

