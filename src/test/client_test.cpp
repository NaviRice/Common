//
// Created by Yang Liu on 11/17/17.
//

#include <iostream>
#include "../Client.hpp"
#include <src/proto/step.pb.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        std::cout << "Usage: client [ip address] [port]" << std::endl;
        return 1;
    }

    std::string ipAddress = argv[1];
    int port = std::stoi(argv[2]);

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    NaviRice::Networking::Client client("NaviRices 1", navirice::proto::HEAD_TRACKING);
    client.onReceiveData([&client](navirice::proto::Response response) -> void {
        std::cout << "\nPlease enter command(0-7) path(string) options(string) body(string): " << std::endl;
    });
    client.onConnected([&client]() -> void {
        std::cout << "Connect established." << std::endl;
    });

    client.connect(ipAddress, port);

    std::cout << "Please enter latitude(double) longitude(double) description(string) icon(string):" << std::endl;

    navirice::proto::Request_Type type = navirice::proto::Request_Type::Request_Type_CURRENT_STEP;
    double latitude;
    double longitude;
    std::string description;
    std::string icon;

    while (true) {
        std::cin >> latitude;
        std::cin >> longitude;
        std::cin >> description;
        std::cin >> icon;

        navirice::proto::Step step;
        step.set_latitude(latitude);
        step.set_longitude(longitude);
        step.set_description(description);
        step.set_icon(icon);

        std::cout <<step.DebugString()<<std::endl;

        int length = step.ByteSizeLong();
        char body[BUFFER_SIZE];

        step.SerializeToArray(&body, length);

        navirice::proto::Request request;
        request.set_type(type);
        request.set_body(body);
        client.send(request);
    }
}