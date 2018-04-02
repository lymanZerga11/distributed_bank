#include "udp_client.h"
#include <string.h>
#include <unistd.h>

// https://linux.m2osw.com/c-implementation-udp-clientserver

UdpClient::UdpClient(const std::string& addr, int port) : f_port(port), f_addr(addr)
{
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r = getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo);
    if(r != 0 || f_addrinfo == NULL)
    {
        throw udp_client_server_runtime_error(("Invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(f_socket == -1)
    {
        freeaddrinfo(f_addrinfo);
        throw udp_client_server_runtime_error(("Could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
}

UdpClient::~UdpClient()
{
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

int UdpClient::get_socket() const
{
    return f_socket;
}

int UdpClient::get_port() const
{
    return f_port;
}

std::string UdpClient::get_addr() const
{
    return f_addr;
}

int UdpClient::send(const char *msg, size_t size)
{
    return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
}

int UdpClient::recv(char *msg, size_t max_size)
{
    return recvfrom(f_socket, msg, max_size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
}

int UdpClient::recv_loop(MessageBuffer &inbox)
{
    while(true){
    	char raw_msg[max_size];
    	if(recv(raw_msg, max_size) == max_size){
    	    Message msg(raw_msg);
    		inbox.insert(msg);
    	}
    }
}

int udp_server::timed_recv(char *msg, size_t max_size, int max_wait_ms)
{
    fd_set s;
    FD_ZERO(&s);
    FD_SET(f_socket, &s);
    struct timeval timeout;
    timeout.tv_sec = max_wait_ms / 1000;
    timeout.tv_usec = (max_wait_ms % 1000) * 1000;
    int retval = select(f_socket + 1, &s, &s, &s, &timeout);
    if(retval == -1)
    {
        // select() set errno accordingly
        return -1;
    }
    if(retval > 0)
    {
        // our socket has data
        return udp_server::recv(f_socket, msg, max_size, 0);
    }
    errno = EAGAIN;
    return -1;
}
