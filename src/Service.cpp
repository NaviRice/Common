//
// Created by Yang Liu on 11/15/17.
//

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Service.hpp"

//NaviRice::Networking::Service::Service(std::string name, std::string ipAddress, int port) {
//    this->name = name;
//    this->ipAddress = ipAddress;
//    this->port = port;
//}
//
//void NaviRice::Networking::Service::start() {
//    Service *service = this;
//    server.onWaitingForConnection([&service](void) -> void {
//        std::cout << "Waiting for connection" << std::endl;
//        service->onWaitingForConnection();
//    });
//    server.onReceiveData([&service](NaviRice::Networking::Buffer buffer) -> void {
//        navirice::proto::Response response;
//        response.ParseFromString(buffer.data);
//        std::cout << "Server received: " << response.DebugString() << std::endl;
//        service->onReceiveData(response);
//    });
//    server.onAcceptConnection([&service](sockaddr_in clientAddress)->void {
//        char ipAddressStr[INET_ADDRSTRLEN];
//        inet_ntop( AF_INET, &clientAddress.sin_addr, ipAddressStr, INET_ADDRSTRLEN );
//        std::cout << "Client connected: " << ipAddressStr << std::endl;
//        service->onAcceptConnection(clientAddress);
//    });
//    server.listen(this->ipAddress, this->port);
//}
//
//void NaviRice::Networking::Service::stop() {
//    server.close();
//}
