#pragma once
#include <stdint.h>

class WebsocketServer
{
private:
    uint16_t port;

    static void WSA_Start();

public:
    WebsocketServer(uint16_t port);
    ~WebsocketServer();
};

class WebSocketConn
{
public:
	WebSocketConn(int socket);
	~WebSocketConn();

    int Recv(void** buff);
    int Send(void* buff, unsigned len);

private:
    int socket;
};

