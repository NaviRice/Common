//
// Created by Yang Liu on 11/15/17.
//

#ifndef SERVICE_H
#define SERVICE_H

#include "Socket.hpp"
#include "src/proto/header_message.pb.h"

namespace NaviRice {
	namespace Networking {
		class Service {
			struct Message {
				navirice::proto::Request header;
				char data[BUFFER_SIZE];
			};
			protected:
			NaviRice::Networking::Socket socket;
			std::string name;
			std::string ipAddress;
			int port;

			public:
			virtual void onWaitingForConnection() {};

			virtual void onAcceptConnection(sockaddr_in clientAddress) {};

			virtual void onReceiveData(Message) {};

			Service(std::string, std::string, int);

			void start();

			void stop();
		};
	}
}


#endif //SERVICE_H
