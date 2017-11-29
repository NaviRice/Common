//
// Created by Yang Liu on 11/17/17.
//

#include "../Service.hpp"
#include "../../build/src/proto/response.pb.h"

class TestingService : public NaviRice::Networking::Service {
    void setupRoutes() override {
        addRoute(navirice::proto::Request_Command::Request_Command_CREATE, "/",
                 [](std::map<std::string, std::string> params,
                    std::map<std::string, std::string> options,
                    const char *body,
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

    NaviRice::Networking::Service *testingService = new TestingService("0.0.0.0", 8080, "NaviRices 1");
    testingService->start();
}