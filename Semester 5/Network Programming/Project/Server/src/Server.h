#pragma once
#include <string>
#include <arpa/inet.h>

class Server {
public:
    explicit Server(const int port);
    void start();
    void stop();

private:
    bool running;
    int server_fd;
    struct sockaddr_in address;
    fd_set current_sockets;
    int fd_max;
    
    void clientHandler(const int new_socket) const;
};