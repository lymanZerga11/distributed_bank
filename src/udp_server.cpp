
#include "../include/udp_server.h"

#include <string.h>
#include <unistd.h>
#include <iostream>

udp_server::udp_server(const std::string& addr, int port)
    : f_port(port)
    , f_addr(addr)
{
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
    if(r != 0 || f_addrinfo == NULL)
    {
        throw udp_client_server_runtime_error(("invalid address or port for UDP socket: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(f_socket == -1)
    {
        freeaddrinfo(f_addrinfo);
        throw udp_client_server_runtime_error(("could not create UDP socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    r = bind(f_socket, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
    if(r != 0)
    {
        freeaddrinfo(f_addrinfo);
        close(f_socket);
        throw udp_client_server_runtime_error(("could not bind UDP socket with: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
}

udp_server::~udp_server()
{
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

int udp_server::get_socket() const
{
    return f_socket;
}

int udp_server::get_port() const
{
    return f_port;
}

std::string udp_server::get_addr() const
{
    return f_addr;
}

int udp_server::recv(int f_socket, char request_packet[], int packet_size, int flag, struct sockaddr_in &client_address, int &client_address_length) {
    recvfrom(f_socket, request_packet, packet_size, flag,
        (struct sockaddr *) &client_address, (socklen_t *) &client_address_length);
}

int udp_server::send(int f_socket, char response_packet[], int packet_size, int flag, struct sockaddr_in &client_address, int &client_address_length) {
    
    if (SIMULATE_REPLY_LOSS) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine engine(seed);
        std::uniform_int_distribution<uint32_t> distr(0, 100);
        int val = distr(engine);
        if (val > 75) { 
            sendto(f_socket, response_packet, packet_size, flag,
                 (struct sockaddr *) &client_address, client_address_length);
        }
    }
    else {
        sendto(f_socket, response_packet, packet_size, flag,
                 (struct sockaddr *) &client_address, client_address_length);
    }
}

