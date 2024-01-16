#include "Client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

const int Client::MAX_BUFF_SIZE = 5e8 + 100;
const int Client::BUFF_STEP = 1e5;
const std::string Client::DefaultServerInfo = "inf/DefaultServerInfo.txt";

Client::Client() {
    getDefaultInfo();
}

Client::Client(int argc, char* argv[]) {
    if (argc <= 3) {
        getDefaultInfo();
        return;
    }
    
    dataFileName = argv[1];
    int serverPort = std::atoi(argv[2]);
    std::string serverIP = argv[3];

    createConnection(serverPort, serverIP);
}

Client::Client(const std::string& dataFileName, int serverPort, const std::string& serverIP) 
                            : dataFileName(dataFileName) {
    createConnection(serverPort, serverIP);
}

void Client::getDefaultInfo() {
    std::ifstream file(DefaultServerInfo);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open default file");
    }

    std::getline(file, dataFileName);

    std::string serverIP;
    std::getline(file, serverIP);

    std::string portStr; 
    std::getline(file, portStr);
    int serverPort = std::stoi(portStr);

    createConnection(serverPort, serverIP);
}

void Client::createConnection(int serverPort, const std::string& serverIP) {
    // Creating socket with IPv4 and TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock < 0) {
        throw std::runtime_error("Couldn't create socket");
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort);
    
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address");
    }
    
    // Connect to the server
    if (connect(sock, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        throw std::runtime_error("Connection failed");
    }
}

Client::~Client() {
    close(sock);
}

void Client::setDataFileName(const std::string& dataFileName) {
    this->dataFileName = dataFileName;
}

void Client::getDataFromFile(std::string& data) const {
    std::ifstream dataFile(dataFileName);

    if (!dataFile.is_open()) {
        throw std::runtime_error("Cannot open data file");
    }

    std::string line;
    while (getline(dataFile, line)) {
        data += line + '\n';
    }
    dataFile.close();
}

void Client::sendData() const {
    std::string data;    
    getDataFromFile(data);

    auto dataCstr = data.c_str();

    int totalBytes = 0;
    int newBytes = 0;
    do {
        newBytes = send(sock, dataCstr + totalBytes, std::min(BUFF_STEP, (int)strlen(dataCstr) - totalBytes), 0);
        totalBytes += newBytes;
    }
    while (totalBytes < strlen(dataCstr));

    std::cerr << "Data sent" << std::endl;

    // Buffer to hold the response
    char* buffer = new char[MAX_BUFF_SIZE];

    // Receive the response from the server
    totalBytes = 0;
    newBytes = 0;
    do {
        newBytes = recv(sock, buffer + totalBytes, BUFF_STEP, 0);
        totalBytes += newBytes;
    }
    while (newBytes > 0 && buffer[totalBytes - 1] != '\n');

    if (newBytes < 0) {
        std::perror("recv failed");
    } else {
        std::string response(buffer);
        std::cout << "Received from server: ";
        std::cout << response << std::endl;
    }

    delete[] buffer;
}
