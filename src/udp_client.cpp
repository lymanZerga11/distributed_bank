
#include "../include/udp_client.h"

#include <string.h>
#include <unistd.h>
#include <iostream>

udp_client::udp_client(const std::string& addr, int port)
    : f_port(port), f_addr(addr) {
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
    if(r != 0 || f_addrinfo == NULL) {
        throw udp_client_server_runtime_error(("invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(f_socket == -1) {
        freeaddrinfo(f_addrinfo);
        throw udp_client_server_runtime_error(("could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    
    struct sockaddr_in  my_addr;
    memset((char *)&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(0);
    r = bind(f_socket, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if(r != 0)
    {
        freeaddrinfo(f_addrinfo);
        close(f_socket);
        throw udp_client_server_runtime_error(("could not bind UDP socket with: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
}

udp_client::~udp_client() {
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

int udp_client::get_socket() const {
    return f_socket;
}

int udp_client::get_port() const {
    return f_port;
}

std::string udp_client::get_addr() const {
    return f_addr;
}

int udp_client::send(const char *msg, size_t size) {
    if (SIMULATE_REQUEST_LOSS) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine engine(seed);
        std::uniform_int_distribution<uint32_t> distr(0, 100);
        int val = distr(engine);
        if (val > 75) { 
            return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
        }
    }
    else {
        return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
    }
    return 0;
}

int udp_client::recv(char *msg, size_t max_size)
{
    return ::recv(f_socket, msg, max_size, 0);
}

int udp_client::timed_recv(char *msg, size_t max_size, int max_wait_ms) {
    fd_set s;
    FD_ZERO(&s);
    FD_SET(f_socket, &s);
    struct timeval timeout;
    timeout.tv_sec = max_wait_ms / 1000;
    timeout.tv_usec = (max_wait_ms % 1000) * 1000;
    int retval = select(FD_SETSIZE, &s, NULL, NULL, &timeout);
    if(retval == -1) {
        // select() set errno accordingly
        return -1;
    }
    if(retval > 0) {
        // our socket has data
        return udp_client::recv(msg, max_size);
    }
    errno = EAGAIN;
    return -1;
}

int udp_client::get_bound_port () const {
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(get_socket(), (struct sockaddr *)&sin, &len) == -1)
      perror("getsockname");
    return ntohs(sin.sin_port);
}
