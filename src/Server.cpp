//
// Created by Yang Liu on 11/16/17.
//

#include <iostream>
#include <arpa/inet.h>
#include "Server.h"

NaviRice::Networking::Server::Server(std::string ipAddress, int port) {
    this->ipAddress = ipAddress;
    this->port = port;
}

void NaviRice::Networking::Server::start() {
    Server *server = this;
    socket.onWaitingForConnection([&server](void) -> void {
        server->onWaitingForConnectionCallback();
    });
    socket.onReceiveData([&server](int clientDescriptor, NaviRice::Networking::Buffer buffer) -> void {
        navirice::proto::Request request;
        request.ParseFromArray(buffer.data, buffer.length);
        std::cout << buffer.length << std::endl;
        server->onReceiveDataCallback(clientDescriptor, request);
    });
    socket.onAcceptConnection([&server](sockaddr_in clientAddress) -> void {
        char ipAddressStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddress.sin_addr, ipAddressStr, INET_ADDRSTRLEN);
        server->onAcceptConnectionCallback(clientAddress);
    });
    socket.listen(this->ipAddress, this->port);
}

void NaviRice::Networking::Server::send(int clientDescriptor, navirice::proto::Response response) {
    int length = response.ByteSizeLong();
    char buffer[BUFFER_SIZE];
    response.SerializeToArray(&buffer, length);
    socket.send(clientDescriptor, buffer, length);
}

void NaviRice::Networking::Server::stop() {
    socket.close();
}

void NaviRice::Networking::Server::onReceiveData(std::function<void(int, navirice::proto::Request)> onReceiveDataCallback) {
    this->onReceiveDataCallback = onReceiveDataCallback;
}

void NaviRice::Networking::Server::onWaitingForConnection(std::function<void()> onWaitingForConnectionCallback) {
    this->onWaitingForConnectionCallback = onWaitingForConnectionCallback;
}

void NaviRice::Networking::Server::onAcceptConnection(std::function<void(sockaddr_in)> onAcceptConnectionCallback) {
    this->onAcceptConnectionCallback = onAcceptConnectionCallback;
}

int main(int argc, char* argv[]) {
    NaviRice::Networking::Server server("0.0.0.0", 8080);
    server.onWaitingForConnection([&server](void) -> void {
        std::cout << "Waiting for connection +++++++" << std::endl;

    });
    server.onReceiveData([&server](int clientDescriptor, navirice::proto::Request request) -> void {
        std::cout << "Server received: +++++++" << request.DebugString() << std::endl;
        navirice::proto::Response response;
        response.set_status(navirice::proto::Response::Status::Response_Status_FORBIDDEN);
        server.send(clientDescriptor, response);
    });
    server.onAcceptConnection([&server](sockaddr_in clientAddress)->void {
        char ipAddressStr[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &clientAddress.sin_addr, ipAddressStr, INET_ADDRSTRLEN );
        std::cout << "Client connected: +++++++" << ipAddressStr << std::endl;
    });
    server.start();
}