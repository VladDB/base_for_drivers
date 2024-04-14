
#pragma once

#include <cstring>
#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#else
#include <winsock2.h>
#include <Ws2tcpip.h>
#endif // __linux__

#include "global.h"

#define SendBufSize 512
#define RecvBufSize 52428

using namespace std;

#ifdef __linux__
// socket
#define socket_type int
#define nfds(_client) (_client + 1)
#define close_socket close
#define socket_res_err (-1)
// #define socket_protocol     IPPROTO_UDP
// #define SOCKET_ERROR        (-1)
#else
// socket
#define socket_type SOCKET
#define nfds(_client) 0
#define close_socket closesocket
#define socket_res_err INVALID_SOCKET
// #define socket_protocol     IPPROTO_IP
#endif

struct TSettings
{
    string ip;
    int port;
    int timeoutMs;
};

class Driver
{
private:
    socket_type sock = 0;
    TSettings m_settings;

    bool initSocket();
    int sendDataByTCP(uint8_t *data, int len, uint8_t *respBuff, int *respSize);

public:
    uint8_t Offset;
    uint8_t RecvBuf[RecvBufSize];
    TSettings settings;
    bool useDeviceSystemTimeout = false;

    Driver(TSettings settings) : m_settings(settings) {};
    virtual ~Driver();
    bool initLink();
    int sendData(uint8_t *data, int len, uint8_t *respBuff, int *respSize);
    void startTask();

    virtual void getInfo() = 0;
    virtual void getCurrentDt() = 0;
};
