#pragma once
#include <string>
#include <arpa/inet.h>

class Server final {
public:
    explicit Server(const int port);
    void start();

private:
    int server_fd;
    struct sockaddr_in address;
    
    void clientHandler(const int new_socket) const;
};