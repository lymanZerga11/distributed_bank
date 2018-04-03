#include "../include/server.h"

int main () {
    Server server ("127.0.0.1", 52225);
    server.process_messages();
    return 0;
}