#pragma once
#include <string>
#include <arpa/inet.h>

class Client {
public:
    Client();
    Client(int argc, char* argv[]);
    Client(const std::string& dataFileName, int serverPort, const std::string& serverIP);

    ~Client();

    void setDataFileName(const std::string& dataFileName);
    void sendData() const;

private:
    static const std::string DefaultServerInfo;

    std::string dataFileName;
    struct sockaddr_in serv_addr;
    int sock;

    void getDefaultInfo();
    void getDataFromFile(std::string& data) const;
    void createConnection(int serverPort, const std::string& serverIP);
};