//
// Created by Yang Liu on 11/17/17.
//

#include <iostream>
#include "../Service.hpp"
#include "src/proto/response.pb.h"

class TestingService : public NaviRice::Networking::Service {
    void setupRoutes() override {
        addRoute(navirice::proto::Request_Type::Request_Type_CURRENT_STEP,
                 [](const char *body,
                    unsigned long bodyLength,
                    std::function<void(navirice::proto::Response)> respond) {
                     navirice::proto::Response response;
                     response.set_status(navirice::proto::Response_Status_SUCCESS);
                     respond(response);
                 });
    }

public:
    TestingService(std::string ipAddress, int port, std::string name) :
            Service(ipAddress, port, name, navirice::proto::RENDERING) {
    }
};

int main(int argc, char *argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if(argc != 3) {
        std::cout << "Usage: service [ip address] [port]" << std::endl;
        return 1;
    }

    std::string ipAddress = argv[1];
    int port = std::stoi(argv[2]);

    NaviRice::Networking::Service *testingService = new TestingService(ipAddress, port, "1");
    testingService->start();
}