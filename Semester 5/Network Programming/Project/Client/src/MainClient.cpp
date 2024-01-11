#include "Client.h"
#include <iostream>

int main(int argc, char *argv[]) {
    // Create and use the client
    Client client(argc, argv);
    client.sendData();
    return 0;
}