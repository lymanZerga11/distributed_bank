#ifndef INCLUDED_udp_client_h
#define INCLUDED_udp_client_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>


class udp_client_server_runtime_error : public std::runtime_error
{
public:
    udp_client_server_runtime_error(const char *w) : std::runtime_error(w) {

    }
};


class UdpClient
{
	public:
		UdpClient(const std::string& addr, int port);
		~UdpClient();
	    int get_socket() const;
	    int get_port() const;
	    std::string get_addr() const;
	    int send(const char *msg, size_t size);
	    int recv(char *msg, size_t max_size);
      

	private:
	    int f_socket;
	    int f_port;
	    std::string f_addr;
	    struct addrinfo *f_addrinfo;
};

#endif
