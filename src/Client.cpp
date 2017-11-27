//
// Created by Yang Liu on 11/16/17.
//

#include <iostream>
#include "Client.h"

NaviRice::Networking::Client::Client() {
}

void NaviRice::Networking::Client::connect(std::string ipAddress, int port) {
    Client *client = this;
    socket.onReceiveData([client](int descriptor, NaviRice::Networking::Buffer buffer) -> void {
        navirice::proto::Response response;
        response.ParseFromArray(buffer.data, buffer.length);
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
}