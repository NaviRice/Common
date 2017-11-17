//
// Created by Yang Liu on 11/16/17.
//

#ifndef COMMON_SERVER_H
#define COMMON_SERVER_H


#include <netinet/in.h>
#include "Socket.hpp"
#include "src/proto/request.pb.h"
#include "src/proto/response.pb.h"

namespace NaviRice {
    namespace Networking {
        class Server {
        protected:
            NaviRice::Networking::Socket socket;
            std::string ipAddress;
            int port;

            std::function<void()> onWaitingForConnectionCallback;

            std::function<void(sockaddr_in)> onAcceptConnectionCallback;

            std::function<void(int, navirice::proto::Request)> onReceiveDataCallback;

        public:
            void onWaitingForConnection(std::function<void()> onWaitingForConnectionCallback);
            void onAcceptConnection(std::function<void(sockaddr_in)> onAcceptConnectionCallback);
            void onReceiveData(std::function<void(int, navirice::proto::Request)> onReceiveDataCallback);

            void send(int clientDescriptor, navirice::proto::Response response);

            Server(std::string, int);

            void start();

            void stop();
        };
    }
}


#endif //COMMON_SERVER_H
