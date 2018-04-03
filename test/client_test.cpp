#include "../include/client.h"

int main () {
    Client client ("127.0.0.1", 52222);
    client.take_loan("Jacob", 1325389290, "1234567");
    return 0;
}