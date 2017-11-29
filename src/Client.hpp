//
// Created by Yang Liu on 11/16/17.
//

#ifndef COMMON_CLIENT_H
#define COMMON_CLIENT_H


#include "Socket.hpp"
#include "src/proto/response.pb.h"
#include "src/proto/request.pb.h"

namespace NaviRice {
    namespace Networking {
        class Client {
            static const std::string COMMAND_NAMES[];
            static const std::string STATUS_NAMES[];
            NaviRice::Networking::Socket socket;
            std::function<void(navirice::proto::Response)> onReceiveDataCallback;
            std::function<void()> onConnectedCallback;

            void log(std::string message);
            void logRequest(navirice::proto::Request request);
            void logResponse(navirice::proto::Response response);
        public:
            Client();
            void onReceiveData(std::function<void(navirice::proto::Response)> onReceiveDataCallback);
            void onConnected(std::function<void()> onConnectedCallback);
            void connect(std::string ipAddress, int port);
            void send(navirice::proto::Request request);
        };
    }
}

#endif //COMMON_CLIENT_H
