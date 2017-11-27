//
// Created by Yang Liu on 11/17/17.
//

#include "../Service.hpp"

int main(int argc, char *argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    NaviRice::Networking::Service service("0.0.0.0", 8080, "Rendering Service 1", navirice::proto::RENDERING);
    service.addRoute(navirice::proto::Request_Command::Request_Command_CREATE, "/",
                     [](std::map<int, std::string> params,
                        std::map<int, std::string> options,
                        void *body) {

                     });
    service.start();
}