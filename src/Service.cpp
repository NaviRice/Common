//
// Created by Yang Liu on 11/15/17.
//

#include <iostream>
#include <iomanip>
#include "Service.hpp"

const std::string NaviRice::Networking::Service::SERVICE_NAMES[] = {
        "COMPUTER_VISION",
        "MACHINE_LEARNING",
        "GEOMETRY_PROCESSING",
        "RENDERING",
        "HEAD_TRACKING",
        "IMAGE_PROVIDING"
};

const std::string NaviRice::Networking::Service::COMMAND_NAMES[] = {
        "CURRENT_STEP",
        "CURRENT_LOCATION",
};

const std::string NaviRice::Networking::Service::STATUS_NAMES[] = {
        "SUCCESS",
        "BAD_REQUEST",
        "NOT_IMPLEMENTED"
};

NaviRice::Networking::Service::Service(std::string ipAddress, int port, std::string name,
                                       navirice::proto::Service serviceType) {
    server = new NaviRice::Networking::Server(ipAddress, port);
    this->name = name;
    this->serviceType = serviceType;
}

void NaviRice::Networking::Service::start() {
    Service *service = this;

    server->onReceiveData([service](int clientDescriptor, navirice::proto::Request request) {
        navirice::proto::Response response;
        response.set_status(navirice::proto::Response_Status_SUCCESS);
        const char *body = request.body().c_str();
        unsigned long bodyLength = request.body().length();

        for (Route route : service->routes) {
            if (route.type == request.type()) {
                route.handler(body,
                              bodyLength,
                              [service, clientDescriptor, request](navirice::proto::Response response) {
                                  service->server->send(clientDescriptor, response);
                                  service->logResponse(request, response);
                              });
                return;
            }
        }
    });
    server->onAcceptConnection([this](sockaddr_in clientIp) {
        this->log("Accept connection");
    });
    server->onWaitingForConnection([this]() {
        this->log("Service started.");
        if(onServiceStartedCallback != nullptr)
        onServiceStartedCallback();
    });
    setupRoutes();
    server->start();
}

void NaviRice::Networking::Service::addRoute(navirice::proto::Request_Type type,
                                             NaviRice::Networking::Service::RequestHandler handler) {
    Route route;
    route.type = type;
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

void NaviRice::Networking::Service::logRequest(navirice::proto::Request request) {
    log(COMMAND_NAMES[request.type()]);
}

void NaviRice::Networking::Service::logResponse(navirice::proto::Request request, navirice::proto::Response response) {
    log(COMMAND_NAMES[request.type()]);
}

void NaviRice::Networking::Service::onServiceStarted(std::function<void()> onServiceStartedCallback) {
    this->onServiceStartedCallback = onServiceStartedCallback;
}
