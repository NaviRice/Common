//
// Created by Yang Liu on 11/16/17.
//

#include <iostream>
#include <iomanip>
#include "Client.hpp"

const std::string NaviRice::Networking::Client::COMMAND_NAMES[] = {
        "INDEX",
        "CREATE",
        "SHOW",
        "UPDATE",
        "DESTROY",
        "SUBSCRIBE",
        "UNSUBSCRIBE",
        "REMOTE_PROCEDURE_CALL"
};

const std::string NaviRice::Networking::Client::STATUS_NAMES[] = {
        "SUCCESS",
        "BAD_REQUEST",
        "FORBIDDEN",
        "NOT_FOUND",
        "SERVER_INTERNAL_ERROR",
        "NOT_IMPLEMENTED"
};

NaviRice::Networking::Client::Client() {
}

void NaviRice::Networking::Client::connect(std::string ipAddress, int port) {
    Client *client = this;
    socket.onReceiveData([client](int descriptor, NaviRice::Networking::Buffer buffer) -> void {
        std::cout << "++++" << std::endl;
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
    std::cout << std::put_time(std::localtime(&t), format) << " " << message << std::endl;
}

void NaviRice::Networking::Client::logRequest(navirice::proto::Request request) {
    log(COMMAND_NAMES[request.command()] + " " + request.resource() + " " + request.options());
}

void NaviRice::Networking::Client::logResponse(navirice::proto::Response response) {
    log(STATUS_NAMES[response.status()]);
}