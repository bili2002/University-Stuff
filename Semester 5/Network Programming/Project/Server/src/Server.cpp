#include "Server.h"
#include "MatrixProcessor.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <thread>

Server::Server(int serverPort) {
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw std::runtime_error("Socket failed");
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(serverPort);
}

const int MAXBUF = 5e5;
void Server::clientHandler(const int new_socket) const {
    char buffer[MAXBUF] = {0};
    
    // Read the incoming matrix
    ssize_t bytesRead = read(new_socket, buffer, MAXBUF);
    if (bytesRead < 0) {
        throw std::runtime_error("Read failed");
        return;
    }

    auto strMatrix = std::string(buffer);

    // std::cerr << "Data recieved:\n" << strMatrix << std::endl;
    
    // Count the zones using MatrixProcessor
    MatrixProcessor mp(strMatrix);
    auto count = mp.countZones();
    
    // Send the count back to the client
    auto strCount = std::to_string(count);
    if (send(new_socket, strCount.c_str(), strCount.size(), 0) == -1) {
        std::cerr << "Couldn't send the message" << std::endl;
    }

    // std::cerr << "Data sent:\n" << strCount << std::endl;
    
    close(new_socket);
}

void Server::start() {
    // Binding socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        throw std::runtime_error("Listen failed");
    }

    while (true) {
        std::cout << "Waiting for a connection..." << std::endl;
        
        socklen_t addrLen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrLen);
        if (new_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
        }
        
        std::cout << "Connected" << std::endl;
        
        // Create a new thread for this client and detach it so it can operate independently
        std::thread clientThread([this, new_socket]() { this->clientHandler(new_socket); });
        clientThread.detach();
    }
}
