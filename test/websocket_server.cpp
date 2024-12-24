#include "websockets.h"
#define _WIN32_WINNT 0x601
#pragma comment(lib, "Ws2_32.lib")
// #include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2spi.h>
// #include <sys/types.h>


// #include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void WebsocketServer::WSA_Start() {
	static bool isDone = false;

	if (!isDone)
	{
		WSADATA info;
		if (WSAStartup(MAKEWORD(2, 0), &info)) {
			throw "Could not start WSA";
		}

	}
}



WebsocketServer::WebsocketServer(uint16_t port)
	:port(port)
{
	WSA_Start();

	int server_fd, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	const char* hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw("socket failed");
	}

	// Forcefully attaching socket to the port 8080
	//if (setsockopt(server_fd, SOL_SOCKET,
	//	SO_REUSEADDR, (const char*)&opt,
	//	sizeof(opt))) {
	//	throw("setsockopt");
	//}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);


	if (bind(server_fd, (struct sockaddr*)&address,
		sizeof(address))
		< 0) {
		throw("bind failed");
	}

	if (listen(server_fd, 3) < 0) {
		throw("listen");
	}

	while (true)
	{
		int new_socket;
		if ((new_socket
			= accept(server_fd, (struct sockaddr*)&address,
				(socklen_t*)&addrlen))
			< 0) {
			throw("accept");
		}

		WebSocketConn* conn = new WebSocketConn(new_socket);
	}

}

WebsocketServer::~WebsocketServer()
{
}

WebSocketConn::WebSocketConn(int socket)
	:socket(socket)
{
	char buffer[512];
	int valread = recv(this->socket, buffer, 512, 0);


	if ((memcmp(buffer, "GET ", 3) != 0) || (memcmp(buffer, "get ", 3) != 0))
	{
		return;
	}

	char *tok = strtok(buffer, "\r\n");

	char* c = buffer;
	while (*c != '\0')
	{
		c++;
	}
}

WebSocketConn::~WebSocketConn()
{
	// closing the connected socket
	closesocket(this->socket);
}