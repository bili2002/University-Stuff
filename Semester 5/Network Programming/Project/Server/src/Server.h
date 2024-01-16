#pragma once
#include <string>
#include <arpa/inet.h>
#include <mutex>
#include <set>

class Server {
public:
    explicit Server(const int port);
    void stop();

    virtual void start() = 0;

protected:
    static const int MAX_BUFF_SIZE;
    static const int BUFF_STEP;

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
    static const int MAX_CLIENTS;
    int current_threads;

    fd_set current_sockets;
    int fd_max;

    std::mutex mtx;
    std::set<int> current_calc_fd; 
    
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


