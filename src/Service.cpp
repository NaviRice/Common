//
// Created by Yang Liu on 11/15/17.
//

#include <iostream>
#include <iomanip>
#include "Service.hpp"

static const char *SERVICE_NAMES[] = {
    "COMPUTER_VISION",
    "MACHINE_LEARNING",
    "GEOMETRY_PROCESSING",
    "RENDERING",
    "HEAD_TRACKING",
    "IMAGE_PROVIDING"
};

NaviRice::Networking::Service::Service(std::string ipAddress, int port, std::string name,
                                       navirice::proto::Service serviceType) {
    server = new NaviRice::Networking::Server(ipAddress, port);
    this->name = name;
    this->serviceType = serviceType;
}

void NaviRice::Networking::Service::start() {
    server->onReceiveData([](int clientDescriptor, navirice::proto::Request request) {
        std::cout << "Command: " << request.command() << std::endl;
        std::cout << "Resource: " << request.resource() << std::endl;
        std::cout << "Options: " << request.options() << std::endl;
    });
    server->onAcceptConnection([](sockaddr_in clientIp) {

    });
    server->onWaitingForConnection([this]() {
        this->log("Service started.");
    });
    server->start();
}

void NaviRice::Networking::Service::addRoute(navirice::proto::Request_Command command, std::string path,
                                             std::function<void(std::map<int, std::string> params,
                                                                std::map<int, std::string> options,
                                                                void *body)> handler) {
    Route route;
    route.command = command;
    route.path = path;
    route.handler = handler;

    routes.push_back(route);
}

void NaviRice::Networking::Service::stop() {
    server->stop();
}

void NaviRice::Networking::Service::log(std::string message) {
    std::time_t t = std::time(nullptr);
    const char *format = "%T";
    std::cout << std::put_time(std::localtime(&t), format) << " [" << SERVICE_NAMES[serviceType] << "][" << name << "] "
              << message << std::endl;
}
