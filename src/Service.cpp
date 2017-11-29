//
// Created by Yang Liu on 11/15/17.
//

#include <iostream>
#include <iomanip>
#include "Service.hpp"
#include "../build/src/proto/response.pb.h"

const std::string NaviRice::Networking::Service::SERVICE_NAMES[] = {
        "COMPUTER_VISION",
        "MACHINE_LEARNING",
        "GEOMETRY_PROCESSING",
        "RENDERING",
        "HEAD_TRACKING",
        "IMAGE_PROVIDING"
};

const std::string NaviRice::Networking::Service::COMMAND_NAMES[] = {
        "INDEX",
        "CREATE",
        "SHOW",
        "UPDATE",
        "DESTROY",
        "SUBSCRIBE",
        "UNSUBSCRIBE",
        "REMOTE_PROCEDURE_CALL"
};

const std::string NaviRice::Networking::Service::STATUS_NAMES[] = {
        "SUCCESS",
        "BAD_REQUEST",
        "FORBIDDEN",
        "NOT_FOUND",
        "SERVER_INTERNAL_ERROR",
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
        service->server->send(clientDescriptor, response);
        service->logRequest(request);

        std::map<std::string, std::string> params;
        std::map<std::string, std::string> options;
        const char *body = request.body().c_str();

        for (Route route : service->routes) {
            if (route.command == request.command() && request.resource() == route.path) {
                route.handler(params, options, body,
                              [service, clientDescriptor, request](navirice::proto::Response response) {
                                  service->server->send(clientDescriptor, response);
                                  service->logResponse(request, response);
                              });
                return;
            }
        }
    });
    server->onAcceptConnection([](sockaddr_in clientIp) {

    });
    server->onWaitingForConnection([this]() {
        this->log("Service started.");
    });
    setupRoutes();
    server->start();
}

void NaviRice::Networking::Service::addRoute(navirice::proto::Request_Command command, std::string path,
                                             std::function<void(
                                                     std::map<std::string, std::string> params,
                                                     std::map<std::string, std::string> options,
                                                     const char *body,
                                                     std::function<void(navirice::proto::Response)> response
                                             )> handler) {
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

void NaviRice::Networking::Service::logRequest(navirice::proto::Request request) {
    log(COMMAND_NAMES[request.command()] + " " + request.resource() + " " + request.options());
}

void NaviRice::Networking::Service::logResponse(navirice::proto::Request request, navirice::proto::Response response) {
    log(COMMAND_NAMES[request.command()] + " " + request.resource() + " " + request.options() +
        " -> " + STATUS_NAMES[response.status()]);
}
