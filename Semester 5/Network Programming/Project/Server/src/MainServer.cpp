#include "Server.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./server <port>\n";
        return 1;
    }

    int port = std::atoi(argv[1]);
  
    ServerSelector server(port);
    server.start();

    return 0;
}

