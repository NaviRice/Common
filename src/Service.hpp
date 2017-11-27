//
// Created by Yang Liu on 11/15/17.
//

#ifndef SERVICE_H
#define SERVICE_H

#include <map>
#include "Server.hpp"
#include "../build/src/proto/service.pb.h"

namespace NaviRice {
    namespace Networking {
        class Service {
            struct Route {
                navirice::proto::Request_Command command;
                std::string path;
                std::function<void(std::map<int, std::string> params, std::map<int, std::string> options,
                                   void *body)> handler;
            };

            NaviRice::Networking::Server *server;
            std::vector<Route> routes;
            std::string name;
            navirice::proto::Service serviceType;

            void log(std::string message);

        public:
            Service(std::string ipAddress, int port, std::string name, navirice::proto::Service serviceType);

            void start();

            void addRoute(navirice::proto::Request_Command command,
                          std::string path,
                          std::function<void(std::map<int, std::string> params,
                                             std::map<int, std::string> options,
                                             void *body)> handler);

            void stop();
        };
    }
}


#endif //SERVICE_H
