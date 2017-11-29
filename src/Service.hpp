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
            static const std::string SERVICE_NAMES[];
            static const std::string COMMAND_NAMES[];
            static const std::string STATUS_NAMES[];

            struct Route {
                navirice::proto::Request_Command command;
                std::string path;
                std::function<void(
                        std::map<std::string, std::string> params,
                        std::map<std::string, std::string> options,
                        const char *body,
                        std::function<void(navirice::proto::Response)> respond
                )> handler;
            };

            NaviRice::Networking::Server *server;
            std::vector<Route> routes;
            std::string name;
            navirice::proto::Service serviceType;

            void log(std::string message);
            void logRequest(navirice::proto::Request request);
            void logResponse(navirice::proto::Request request, navirice::proto::Response response);

            virtual void setupRoutes() = 0;

        public:
            Service(std::string ipAddress, int port, std::string name, navirice::proto::Service serviceType);

            void start();

            void addRoute(navirice::proto::Request_Command command,
                          std::string path,
                          std::function<void(
                                  std::map<std::string, std::string> params,
                                  std::map<std::string, std::string> options,
                                  const char *body,
                                  std::function<void(navirice::proto::Response)> respond
                          )> handler);

            void stop();
        };
    }
}


#endif //SERVICE_H
