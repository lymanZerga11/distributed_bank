#include "udp_client_server.h"
#include <iostream>

int main () {
  udp_client_server::udp_server server("127.0.0.1", 59205);
  char msg[100];
  if (server.timed_recv(msg, 100, 10000) > 0) {
    std::cout << "Received: " << msg << std::endl;
  }
  else {
    std::cout << "Timeout" << std::endl;
  }
  struct sockaddr_in si_other;
 int slen = sizeof(si_other);

 udp_client_server::udp_server server("127.0.0.1", 59207);
 char msg[1000];
 if ((recvfrom(server.get_socket(), msg, 512, 0, (struct sockaddr *) &si_other, (socklen_t *) &slen)) == -1)
 {
   ;
 }
 //print details of the client/peer and the data received
 printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
 printf("Data: %s\n" , msg);
}
