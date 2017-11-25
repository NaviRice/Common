//
// Created by Yang Liu on 11/15/17.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "Socket.hpp"
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

NaviRice::Networking::Socket::Socket() {
	// Create socket file descriptor
	if ((this->descriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		std::perror("Fail to initialize socket");
		exit(EXIT_FAILURE);
	}

	// Promote reuse of ip address
	int opt = 1;
	if (setsockopt(this->descriptor, SOL_SOCKET, SO_REUSEADDR, (char *) &opt,
				sizeof(opt)) < 0) {
		perror("Fail to enable socket options");
		exit(EXIT_FAILURE);
	}
}

void NaviRice::Networking::Socket::listen_async_stop(){
	this->async_listen_online = false;
}

void NaviRice::Networking::Socket::listen_async(std::string ipAddress, int port){
	if(this->async_listen_online) return;
	this->async_listen_online = true;
	std::thread listnerThread(&Socket::listen, this, ipAddress, port);
	listnerThread.detach();
}

void NaviRice::Networking::Socket::listen(std::string ipAddress, int port) {
	struct sockaddr_in address = toSockAddress(ipAddress, port);

	// Bind socket to ipAddress and port
	if (bind(this->descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
		std::string error = "Fail to listen at " + ipAddress + ":" + std::to_string(port);
		std::perror(error.c_str());
		exit(EXIT_FAILURE);
	}

	if (::listen(this->descriptor, MAX_CONNECTION) < 0) {
		perror("Fail to listen to connection");
		exit(EXIT_FAILURE);
	}

	this->onWaitingForConnectionCallback();

	if(!this->async_listen_online) this->async_listen_online = true;
	while (this->async_listen_online) {
		int clientDescriptor;
		struct sockaddr_in clientAddress;
		int clientAddressLen = sizeof(clientAddress);
		if ((clientDescriptor = accept(this->descriptor, (struct sockaddr *) &address,
						(socklen_t *) &clientAddressLen)) > 0) {
			this->onAcceptConnectionCallback(clientAddress);
			struct ReceiveParams* receiveParams = new ReceiveParams();
			receiveParams->socket = this;
			receiveParams->descriptor = clientDescriptor;
			std::thread receiveThread(&Socket::receive, this, receiveParams);
			receiveThread.detach();
		}
	}

	close();
}

void NaviRice::Networking::Socket::receive(void *receiveParams) {
	struct Buffer buffer;
	struct ReceiveParams params = *((struct ReceiveParams *) receiveParams);
	delete (struct ReceiveParams*)receiveParams;

	while (true) {
		if ((buffer.length = read(params.descriptor, buffer.data, BUFFER_SIZE)) > 0)
			params.socket->onReceiveDataCallback(params.descriptor, buffer);
	}
}

int NaviRice::Networking::Socket::send(int clientDescriptor, void *buffer, size_t length) {
	return ::send(clientDescriptor, buffer, length, 0);
}

int NaviRice::Networking::Socket::send(void *buffer, size_t length) {
	return ::send(this->descriptor, buffer, length, 0);
}

void NaviRice::Networking::Socket::connect(std::string ipAddress, int port) {
	struct sockaddr_in address = toSockAddress(ipAddress, port);
	if (::connect(this->descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
		std::string error = "Fail to connect to server at " + ipAddress + ":" + std::to_string(port);
		std::perror(error.c_str());
		return;
	}
	this->onConnectedCallback();
	
	struct ReceiveParams* receiveParams = new ReceiveParams();
	receiveParams->socket = this;
	receiveParams->descriptor = this->descriptor;

	std::thread receiveThread(&Socket::receive, this, receiveParams);
	receiveThread.detach();
}


struct sockaddr_in NaviRice::Networking::Socket::toSockAddress(std::string ipAddress, int port) {
	struct sockaddr_in address;
	memset(&address, '0', sizeof(address));

	address.sin_family = AF_INET;
	inet_pton(AF_INET, ipAddress.c_str(), &address.sin_addr);
	address.sin_port = htons(port);
	return address;
}

void NaviRice::Networking::Socket::close() {
	::close(this->descriptor);
}

void NaviRice::Networking::Socket::onReceiveData(std::function<void(int, Buffer)> onReceiveDataCallback) {
	this->onReceiveDataCallback = onReceiveDataCallback;
}

void NaviRice::Networking::Socket::onWaitingForConnection(std::function<void()> onWaitingForConnectionCallback) {
	this->onWaitingForConnectionCallback = onWaitingForConnectionCallback;
}

void  NaviRice::Networking::Socket::onAcceptConnection(std::function<void(sockaddr_in)> onAcceptConnectionCallback) {
	this->onAcceptConnectionCallback = onAcceptConnectionCallback;
}

void NaviRice::Networking::Socket::onConnected(std::function<void()> onConnectedCallback) {
	this->onConnectedCallback = onConnectedCallback;
}
