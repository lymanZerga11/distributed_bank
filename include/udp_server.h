#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#define SIMULATE_REPLY_LOSS 1

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>

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

    int                 recv(int f_socket, char request_packet[], int packet_size, int flag, struct sockaddr_in &client_address, int &client_address_length);
    int                 send(int f_socket, char response_packet[], int packet_size, int flag, struct sockaddr_in &client_address, int &client_address_length);

private:
    int                 f_socket;
    int                 f_port;
    std::string         f_addr;
    struct addrinfo *   f_addrinfo;
};

#endif
