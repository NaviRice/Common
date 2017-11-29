//
// Created by Yang Liu on 11/17/17.
//

#include <iostream>
#include "../Client.hpp"
#include "src/proto/response.pb.h"

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    NaviRice::Networking::Client client;
    client.onReceiveData([&client](navirice::proto::Response response) -> void {
        std::cout << "Client received: " << response.DebugString() << std::endl;
    });
    client.onConnected([&client]() -> void {
        std::cout << "Connect established." << std::endl;
    });
    client.connect("127.0.0.1", 8080);

    int command;
    std::string path;
    std::string options;

    while(true) {
        std::cin >> command;
        std::cin >> path;
        std::cin >> options;
        navirice::proto::Request request;
        request.set_command((navirice::proto::Request_Command)command);
        request.set_resource(path);
        request.set_options(options);
        client.send(request);
    }
}