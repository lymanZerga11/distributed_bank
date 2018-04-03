#include "../include/server.h"

int main () {
    Server server ("127.0.0.1", 52222);
    server.process_messages();
    return 0;
}