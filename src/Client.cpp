//
// Created by Yang Liu on 11/16/17.
//

#include <iostream>
#include <iomanip>
#include <tiff.h>
#include "Client.hpp"

const std::string NaviRice::Networking::Client::SERVICE_NAMES[] = {
        "COMPUTER_VISION",
        "MACHINE_LEARNING",
        "GEOMETRY_PROCESSING",
        "RENDERING",
        "HEAD_TRACKING",
        "IMAGE_PROVIDING"
};

const std::string NaviRice::Networking::Client::COMMAND_NAMES[] = {
        "CURRENT_STEP",
        "CURRENT_LOCATION",
};

const std::string NaviRice::Networking::Client::STATUS_NAMES[] = {
        "SUCCESS",
        "BAD_REQUEST",
        "NOT_IMPLEMENTED"
};

NaviRice::Networking::Client::Client(std::string name, navirice::proto::Service serviceType) {
    this->name = name;
    this->serviceType = serviceType;
}

void NaviRice::Networking::Client::connect(std::string ipAddress, int port) {
    Client *client = this;
    socket.onReceiveData([client](int descriptor, NaviRice::Networking::Buffer buffer) -> void {
        navirice::proto::Response response;
        response.ParseFromArray(buffer.data, buffer.length);
        client->logResponse(response);
        client->onReceiveDataCallback(response);
    });
    socket.onConnected([client]() -> void {
        client->onConnectedCallback();
    });
    socket.connect(ipAddress, port);
}

void NaviRice::Networking::Client::onReceiveData(
        std::function<void(navirice::proto::Response)> onReceiveDataCallback) {
    this->onReceiveDataCallback = onReceiveDataCallback;
}

void NaviRice::Networking::Client::onConnected(std::function<void()> onConnectedCallback) {
    this->onConnectedCallback = onConnectedCallback;
}

void NaviRice::Networking::Client::send(navirice::proto::Request request) {
    int length = request.ByteSizeLong();
    char buffer[BUFFER_SIZE];
    request.SerializeToArray(&buffer, length);
    socket.send(buffer, length);
    logRequest(request);
}

void NaviRice::Networking::Client::log(std::string message) {
    std::time_t t = std::time(nullptr);
    const char *format = "%T";
    std::cout << std::put_time(std::localtime(&t), format) << " [" << SERVICE_NAMES[serviceType] << "][" << name << "] "
              << message << std::endl;
}

void NaviRice::Networking::Client::logRequest(navirice::proto::Request request) {
    log(COMMAND_NAMES[request.type()]);
}

void NaviRice::Networking::Client::logResponse(navirice::proto::Response response) {
    log(STATUS_NAMES[response.status()]);
}