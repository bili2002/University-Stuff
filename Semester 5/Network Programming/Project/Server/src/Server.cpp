#include "Server.h"
#include "MatrixProcessor.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <thread>

const int Server::MAXBUF = 5e5;


Server::Server(int serverPort) {
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw std::runtime_error("Socket failed");
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(serverPort);
}

void Server::setupServer() {
    running = true;

    // Binding socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        throw std::runtime_error("Listen failed");
    }
}

void Server::stop() {
    running = false;
}


void ServerParallel::start() {
    setupServer();

    while (true) {
        socklen_t addrLen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrLen);
        if (new_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
        }
        
        std::cerr << "Connected" << std::endl;
        
        // Create a new thread for this client and detach it so it can operate independently
        std::thread clientThread([this, new_socket]() { this->clientHandler(new_socket); });
        clientThread.detach();
    }
}

void ServerParallel::clientHandler(const int new_socket) {
    char buffer[MAXBUF] = {0};
    
    // Read the incoming matrix
    ssize_t bytesRead = recv(new_socket, buffer, MAXBUF, 0);

    if (bytesRead <= 0) {        
        throw std::runtime_error("Read failed");
    }

    auto strMatrix = std::string(buffer);

    // Count the zones using MatrixProcessor
    MatrixProcessor mp(strMatrix);
    auto count = mp.countZones();
    
    // Send the count back to the client
    auto strCount = std::to_string(count);
    if (send(new_socket, strCount.c_str(), strCount.size(), 0) == -1) {
        std::cerr << "Couldn't send the message" << std::endl;
    }

    close(new_socket);
}



void ServerSelector::handleSelectFD(int fd, fd_set& ready_fds) {
    if (FD_ISSET(fd, &ready_fds)) {
        if (fd == server_fd) {
            // New connection
            std::cerr << "Connected" << std::endl;

            int client_fd = accept(server_fd, NULL, NULL);
            if (client_fd < 0) {
                std::cerr << "Accept failed" << std::endl;
            }
            
            fd_max = std::max(fd_max, client_fd);                    
            FD_SET(client_fd, &current_sockets);
        } else {
            // Data to read
            std::thread clientThread = std::thread(&ServerSelector::clientHandler, this,  fd);
            clientThread.detach();
        }
    }
}

void ServerSelector::start() {
    setupServer();

    // Setuping socket sets
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    fd_max = server_fd;

    while (true) {
        fd_set ready_fds = current_sockets;

        if (select(fd_max + 1, &ready_fds, NULL, NULL, NULL) < 0) {
            throw std::runtime_error("Select failed.");
        }
        
        for (int fd = 0; fd <= fd_max; fd++) {
            handleSelectFD(fd, ready_fds);
        }
    }
}

void ServerSelector::clientHandler(const int new_socket) {
    char buffer[MAXBUF] = {0};
    
    // Read the incoming matrix
    ssize_t bytesRead = recv(new_socket, buffer, MAXBUF, 0);

    if (bytesRead <= 0) {
        shutdown(new_socket, 2);
        FD_CLR(new_socket, &current_sockets);
        return;
    }

    auto strMatrix = std::string(buffer);

    // Count the zones using MatrixProcessor
    MatrixProcessor mp(strMatrix);
    auto count = mp.countZones();
    
    // Send the count back to the client
    auto strCount = std::to_string(count);
    if (send(new_socket, strCount.c_str(), strCount.size(), 0) == -1) {
        std::cerr << "Couldn't send the message" << std::endl;
    }
}
