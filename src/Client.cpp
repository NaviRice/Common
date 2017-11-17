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

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    NaviRice::Networking::Client client;
    client.onReceiveData([&client](navirice::proto::Response response) -> void {
        std::cout << "Client received: +++++++" << response.DebugString() << std::endl;
    });
    client.onConnected([&client]() -> void {
            std::cout << "Connect established." << std::endl;
    });
    client.connect("127.0.0.1", 8080);

    int command;
    while(true) {
        std::cin >> command;
        navirice::proto::Request request;
        request.set_command((navirice::proto::Request_Command)command);
        client.send(request);
    }
}