#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <string>

class udp_client_server_runtime_error : public std::runtime_error
{
public:
    udp_client_server_runtime_error(const char *w) : std::runtime_error(w) {}
};

class udp_server
{
public:
                        udp_server(const std::string& addr, int port);
                        ~udp_server();

    int                 get_socket() const;
    int                 get_port() const;
    std::string         get_addr() const;

    int                 recvfrom(char *msg, size_t max_size);
    int                 sendto(char *msg, size_t max_size);

private:
    int                 f_socket;
    int                 f_port;
    std::string         f_addr;
    struct addrinfo *   f_addrinfo;
};

#endif
