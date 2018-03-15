//
// Created by Yang Liu on 11/17/17.
//

#include <iostream>
#include <arpa/inet.h>
#include "../Server.hpp"

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if(argc != 3) {
        std::cout << "Usage: service [ip address] [port]" << std::endl;
        return 1;
    }

    std::string ipAddress = argv[1];
    int port = std::stoi(argv[2]);

    NaviRice::Networking::Server server(ipAddress, port);
    server.onWaitingForConnection([&server](void) -> void {
        std::cout << "Waiting for connection" << std::endl;

    });
    server.onReceiveData([&server](int clientDescriptor, navirice::proto::Request request) -> void {
        std::cout << "Server received: " << request.DebugString() << std::endl;
        navirice::proto::Response response;
        response.set_status(navirice::proto::Response::Status::Response_Status_SUCCESS);
        server.send(clientDescriptor, response);
    });
    server.onAcceptConnection([&server](sockaddr_in clientAddress)->void {
        char ipAddressStr[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &clientAddress.sin_addr, ipAddressStr, INET_ADDRSTRLEN );
        std::cout << "Client connected: " << ipAddressStr << std::endl;
    });
    server.start();
}