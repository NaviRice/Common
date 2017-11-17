//
// Created by Yang Liu on 11/15/17.
//

#ifndef SOCKET_H
#define SOCKET_H

#include <string>

#define MAX_CONNECTION 10
#define BUFFER_SIZE 1024

namespace NaviRice {
    namespace Networking {
        struct Buffer {
            char data[BUFFER_SIZE];
            ssize_t length;
        };

        class Socket {
            int descriptor;

            struct sockaddr_in toSockAddress(std::string, int);

            struct ReceiveParams {
                int descriptor;
                Socket *socket;
            };

            std::function<void()> onWaitingForConnectionCallback;
            std::function<void()> onConnectedCallback;

            std::function<void(sockaddr_in)> onAcceptConnectionCallback;

            std::function<void(int, Buffer)> onReceiveDataCallback;


            static void *receive(void *);

        public:

            Socket();

            void listen(std::string, int);

            void connect(std::string, int);

            int send(void *buffer, size_t length);

            int send(int clientDescriptor, void *buffer, size_t length);

            void close();

            void onWaitingForConnection(std::function<void()> onWaitingForConnectionCallback);
            void onConnected(std::function<void()> onConnectedCallback);
            void onAcceptConnection(std::function<void(sockaddr_in)> onAcceptConnectionCallback);
            void onReceiveData(std::function<void(int, Buffer)> onReceiveDataCallback);
        };
    }
}


#endif //SOCKET_H
