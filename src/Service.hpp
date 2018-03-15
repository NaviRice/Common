//
// Created by Yang Liu on 11/15/17.
//

#ifndef SERVICE_H
#define SERVICE_H

#include <map>
#include "src/proto/service.pb.h"
#include "Server.hpp"

namespace NaviRice {
    namespace Networking {
        class Service {
            static const std::string SERVICE_NAMES[];
            static const std::string COMMAND_NAMES[];
            static const std::string STATUS_NAMES[];

            typedef std::function<void(
                    const char *body,
                    unsigned long bodyLength,
                    std::function<void(navirice::proto::Response)> respond
            )> RequestHandler;

            struct Route {
                navirice::proto::Request_Type type;
                RequestHandler handler;
            };

            NaviRice::Networking::Server *server;
            std::vector<Route> routes;
            std::string name;
            navirice::proto::Service serviceType;

            std::function<void()> onServiceStartedCallback;

            void log(std::string message);
            void logRequest(navirice::proto::Request request);
            void logResponse(navirice::proto::Request request, navirice::proto::Response response);

            virtual void setupRoutes() = 0;

        public:
            Service(std::string ipAddress, int port, std::string name, navirice::proto::Service serviceType);

            void onServiceStarted(std::function<void()> onServiceStartedCallback);

            void start();

            void addRoute(navirice::proto::Request_Type type, RequestHandler handler);

            void stop();
        };
    }
}


#endif //SERVICE_H
