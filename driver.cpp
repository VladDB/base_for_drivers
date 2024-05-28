#include "driver.h"

Driver::~Driver()
{
    close_socket(this->sock);
}

//-------------------------------------------------------------------------------------------

void Driver::startTask()
{
    doLog("Start task");
}

//-------------------------------------------------------------------------------------------

bool Driver::initSocket()
{
    if (this->sock)
        return true;

    char log[128];

#ifdef __WIN32__
    // запрашиваем интерфейс Winsock
    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData))
    {
        doLog("Winsock WSAStartup() error !");
        return false;
    }
#endif

    struct sockaddr_in addr;
    this->sock = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sock == socket_res_err)
    {
        sprintf(log, "Error create socket");
        doLog(log);
        return false;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_settings.port);
    addr.sin_addr.s_addr = inet_addr(m_settings.ip.c_str());
    if (connect(this->sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        sprintf(log, "Error connect socket");
        doLog(log);
        perror("init socket");
        return false;
    }
    sprintf(log, "Connected to %s:%d", inet_ntoa(addr.sin_addr), m_settings.port);
    doLog(log);
    return true;
}

//-------------------------------------------------------------------------------------------

bool Driver::initLink()
{
    if (!this->initSocket())
        return false;

    return true;
}

//-------------------------------------------------------------------------------------------

int Driver::sendDataByTCP(uint8_t *data, int len, uint8_t *respBuff, int *respSize)
{
    char log[128];

    if (!this->sock)
    {
        doLog("Data does not send, socket is not init");
        if (!this->initLink())
        {
            doLog("Socket can not been init");
            return -1;
        }
    }

    // Отправка
    int total = 0;
    int respLen;
    while (total < len)
    {
#ifdef __linux__
        respLen = send(this->sock, data, len - total, 0);
#else
        respLen = send(this->sock, (const char *)(data), len - total, 0);
#endif                    //__linux__
        if (respLen <= 0) //-1
        {
            doLog("Error while sending data by TCP");
            // perror("socket send data");
            return -1;
        }
        total += respLen;
    }

    // Получение
    int res{0}, MaxChr{0};
    fd_set ListenSockets;
    struct timeval time_wait;
    time_wait.tv_sec = m_settings.timeoutMs / 1000;
    time_wait.tv_usec = 0;

    if (*respSize)
    {
        MaxChr = *respSize;
        *respSize = 0;
    }

    time_t startTime = time(0);
    if (useDeviceSystemTimeout)
    {
        while ((time(0) - startTime) < (this->m_settings.timeoutMs / 1000))
        {
            FD_ZERO(&ListenSockets);
            FD_SET(this->sock, &ListenSockets);

            res = select(nfds(this->sock), &ListenSockets, NULL, NULL, &time_wait);
            // res = select(NULL, &ListenSockets, NULL, NULL, &time_wait);
            if (res < 0)
            {
                // close(this->sock);
                return -1;
            }

            if (!res || !FD_ISSET(this->sock, &ListenSockets)) // ничего не получили
                return 0;
            else
            {
#ifdef __linux__
                res = recv(this->sock, respBuff + *respSize, MaxChr, 0);
#else
                res = recv(this->sock, reinterpret_cast<char *>(respBuff) + *respSize, MaxChr, 0);
                // res = recv(this->sock, (const char*) (respBuff), MaxChr, 0);
#endif //__linux__
                if (res > 0)
                {
                    *respSize += res;
                    MaxChr -= res;
                    // return 0;
                }
                else
                {
                    sprintf(log, "Error while reading response data by TCP. Recv size = %i", res);
                    doLog(log);
                    return -1;
                }
            }
        }
    }
    else
    {
        FD_ZERO(&ListenSockets);
        FD_SET(this->sock, &ListenSockets);

        res = select(nfds(this->sock), &ListenSockets, NULL, NULL, &time_wait);
        // res = select(NULL, &ListenSockets, NULL, NULL, &time_wait);
        if (res < 0)
        {
            // close(this->sock);
            return -1;
        }

        if (!res || !FD_ISSET(this->sock, &ListenSockets)) // ничего не получили
        {
            // perror("Get Data");
            // doLog("select() - no answer, timeout");
            return 0;
        }
        else
        {
#ifdef __linux__
            res = recv(this->sock, respBuff + *respSize, MaxChr, 0);
#else
            res = recv(this->sock, reinterpret_cast<char *>(respBuff) + *respSize, MaxChr, 0);
            // res = recv(this->sock, (const char*) (respBuff), MaxChr, 0);
#endif //__linux__
            if (res > 0)
            {
                *respSize += res;
                MaxChr -= res;
                // return 0;
            }
            else
            {
                sprintf(log, "Error while reading response data by TCP. Recv size = %i", res);
                doLog(log);
                return -1;
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------

int Driver::sendData(uint8_t *data, int len, uint8_t *response, int *respSize)
{
    *respSize = RecvBufSize;

    return this->sendDataByTCP(data, len, response, respSize);

    return -1;
}

//-------------------------------------------------------------------------------------------
