#include "Server.h"
#include "ArrayProcessor.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <thread>

const int Server::MAX_BUFF_SIZE = 5e8 + 100;
const int Server::BUFF_STEP = 1e5;

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


/* ServerParallel */
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
        std::thread clientThread = std::thread(&ServerParallel::clientHandler, this,  new_socket);
        clientThread.detach();
    }
}

void ServerParallel::clientHandler(const int new_socket) {
    char* buffer = new char[MAX_BUFF_SIZE];
    
    // Read the incoming array
    int totalBytes = 0;
    int newBytes = 0;
    do {
        newBytes = recv(new_socket, buffer + totalBytes, BUFF_STEP - totalBytes, 0);
        totalBytes += newBytes;
    }
    while (newBytes > 0 && buffer[totalBytes - 1] != '\n');

    if (newBytes < 0) {   
        delete[] buffer;
        throw std::runtime_error("Read failed");
    }
    
    auto arrayString = std::string(buffer);

    // Sort the array using ArrayProcessor
    ArrayProcessor arrayProcessor(arrayString);
    arrayProcessor.mergeSortZones();
    
    // Send the count back to the client
    arrayString = arrayProcessor.getString();
    auto arrayCstr = arrayString.c_str();

    totalBytes = 0;
    newBytes = 0;
    do {
        newBytes = send(new_socket, arrayCstr + totalBytes, std::min(BUFF_STEP, (int)strlen(arrayCstr) - totalBytes), 0);
        totalBytes += newBytes;
    }
    while (newBytes > 0 && totalBytes < strlen(arrayCstr));

    if (newBytes < 0) {
        throw std::runtime_error("Send failed.");
    }

    delete[] buffer;
    close(new_socket);
}


/* ServerSelector */
const int ServerSelector::MAX_CLIENTS = 4;

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

            // if we want one client at a time
            // clientHandler(fd);

            mtx.lock();
            if (current_threads < MAX_CLIENTS && current_calc_fd.find(fd) == current_calc_fd.end()) {
                current_calc_fd.insert(fd);
                current_threads++;

                std::thread clientThread = std::thread(&ServerSelector::clientHandler, this,  fd);
                clientThread.detach();
            }
            mtx.unlock();
        }
    }
}

void ServerSelector::start() {
    setupServer();

    current_threads = 0;
    mtx.unlock();

    // Setuping socket sets
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    fd_max = server_fd;

    while (running) {
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
    char* buffer = new char[MAX_BUFF_SIZE];
    
    // Read the incoming array
    int totalBytes = 0;
    int newBytes = 0;
    do {
        newBytes = recv(new_socket, buffer + totalBytes, BUFF_STEP, 0);
        totalBytes += newBytes;
    }
    while (newBytes > 0 && buffer[totalBytes - 1] != '\n');

    if (totalBytes <= 0) {
        delete[] buffer;
        shutdown(new_socket, 2);
        FD_CLR(new_socket, &current_sockets);
        return;
    }

    auto arrayString = std::string(buffer);

    // Sort the array using ArrayProcessor
    ArrayProcessor arrayProcessor(arrayString);
    arrayProcessor.mergeSortZones();
    
    // Send the count back to the client
    arrayString = arrayProcessor.getString();
    auto arrayCstr = arrayString.c_str();

    totalBytes = 0;
    newBytes = 0;
    do {
        newBytes = send(new_socket, arrayCstr + totalBytes, std::min(BUFF_STEP, (int)strlen(arrayCstr) - totalBytes), 0);
        totalBytes += newBytes;
    }
    while (newBytes > 0 && totalBytes < strlen(arrayCstr));

    if (newBytes < 0) {
        throw std::runtime_error("Send failed.");
    }

    delete[] buffer;

    mtx.lock(); 
    current_calc_fd.erase(new_socket);
    current_threads--;
    mtx.unlock();
}
