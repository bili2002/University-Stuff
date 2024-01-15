#pragma once
#include <string>
#include <arpa/inet.h>

class Server {
public:
    explicit Server(const int port);
    void stop();

    virtual void start() = 0;

protected:
    static const int MAXBUF;

    bool running;
    int server_fd;
    
    struct sockaddr_in address;
    
    void setupServer();

    virtual void clientHandler(const int new_socket) = 0;
};


class ServerSelector : Server {
    using Server::Server;

public:
    void start() override;

protected:
    fd_set current_sockets;
    int fd_max;
    
    void handleSelectFD(int fd, fd_set& ready_fds);
    void clientHandler(const int new_socket) override;
};


class ServerParallel : Server {
    using Server::Server;
public:
    void start() override;

protected:
    
    void clientHandler(const int new_socket) override;
};


