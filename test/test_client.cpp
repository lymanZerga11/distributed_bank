#include "udp_client_server.h"

int main () {
  udp_client_server::udp_client client("127.0.0.1", 59205);
  client.send ("Hello", 6);
}
