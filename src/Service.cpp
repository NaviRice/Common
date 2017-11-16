//
// Created by Yang Liu on 11/15/17.
//

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Service.hpp"

NaviRice::Networking::Service::Service(std::string name, std::string ipAddress, int port) {
    this->name = name;
    this->ipAddress = ipAddress;
    this->port = port;
}

void NaviRice::Networking::Service::start() {
    Service *service = this;
    socket.onWaitingForConnection([&service](void) -> void {
        std::cout << "Waiting for connection" << std::endl;
        service->onWaitingForConnection();
    });
    socket.onReceiveData([&service](NaviRice::Networking::Buffer buffer) -> void {
        std::cout << "Server received: " << buffer.data << std::endl;
        Message message;
        service->onReceiveData(message);
    });
    socket.onAcceptConnection([&service](sockaddr_in clientAddress)->void {
        char ipAddressStr[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &clientAddress.sin_addr, ipAddressStr, INET_ADDRSTRLEN );
        std::cout << "Client connected: " << ipAddressStr << std::endl;
        service->onAcceptConnection(clientAddress);
    });
    socket.listen(this->ipAddress, this->port);
}

void NaviRice::Networking::Service::stop() {
    socket.close();
}
