#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include "KalmanFilter.hpp"

enum MessageType {
    MSG_START,
    TRUE_POSITION,
    SPEED,
    ACCELERATION,
    DIRECTION,
    MSG_END
};

struct Message {
    MessageType type;
    std::string timestamp;
    std::vector<long double> data;
};

struct InitStruct {
    std::vector<long double> position;
    long double speed;
    std::vector<long double> acceleration;
    std::vector<long double> direction;
};

Message parseMessage(const std::string& messageStr) {
    Message message;
    std::cout << "parseMessage " << messageStr << std::endl;
    if (messageStr == "MSG_START") {
        message.type = MSG_START;
    } else if (messageStr == "MSG_END") {
        message.type = MSG_END;
    } else {
        std::istringstream iss(messageStr);


        std::string first_line, messageType;
        iss >> first_line;

        if (first_line == "Trajectory") {
            message.type = MSG_START;
            return message;
        }
        message.timestamp = first_line.substr(0, 14);

        messageType = first_line.substr(14, first_line.length());

        if (messageType == "TRUE") {
            message.type = TRUE_POSITION;
            iss >> messageType;
        } else if (messageType == "SPEED") {
            message.type = SPEED;
        } else if (messageType == "ACCELERATION") {
            message.type = ACCELERATION;
        } else if (messageType == "DIRECTION") {
            message.type = DIRECTION;
        } else {
            message.type = MSG_START;
        }

        double value;
        while (iss >> value) {
            message.data.push_back(value);
        }
    }

    return message;
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4242);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with server IP

    // Send handshake signal
    const char* handshake = "READY";
    sendto(clientSocket, handshake, strlen(handshake), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Use a while loop to continuously receive and process data from the server
    int i = 0;
    InitStruct initStruct;
    char buffer[1024 * 256];
    socklen_t addrLen = sizeof(serverAddr);
    while (i < 8) {
        ssize_t numBytes = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &addrLen);
        buffer[numBytes] = '\0';
        Message message = parseMessage(buffer);
        switch (message.type) {
            case MSG_START:
                break;
            case TRUE_POSITION:
                initStruct.position = message.data;
                break;
            case SPEED:
                initStruct.speed = message.data[0];
                break;
            case ACCELERATION:
                initStruct.acceleration = message.data;
                break;
            case DIRECTION:
                initStruct.direction = message.data;
                break;
            case MSG_END:
                break;
        }
        i++;
    }
    printf("init Struct\n");
    printf("pos %Lf %Lf %Lf\n", initStruct.position[0], initStruct.position[1], initStruct.position[2]);
    printf("spe %Lf\n", initStruct.speed);
    printf("acc %Lf %Lf %Lf\n", initStruct.acceleration[0], initStruct.acceleration[1], initStruct.acceleration[2]);
    printf("dir %Lf %Lf %Lf\n", initStruct.direction[0], initStruct.direction[1], initStruct.direction[2]);
    KalmanFilter kf;
    while (true) {
        ssize_t numBytes = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &addrLen);
        if (numBytes <= 0) {
            return 1;
        }
        buffer[numBytes] = '\0';
        std::cout << "new message : " << buffer << std::endl;    
        Message message = parseMessage(buffer);

    }

    close(clientSocket);

    return 0;
}
