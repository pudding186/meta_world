#pragma once
#include "net_tcp.h"
#include "meyers_singleton.hpp"
#include <string>

class CFuncPerformanceInfo;

class NetSystem
{
public:
    NetSystem();

    ~NetSystem();

public:
    bool Initialize(
        uint32_t max_socket_num,
        uint32_t max_io_thread_num,
        uint32_t max_accept_ex_num,
        uint32_t max_event_num);

    void Uninitialized(void);

    bool OnUpdate(CFuncPerformanceInfo** info);

public:

    HSESSION Connect(const std::string& ip,
        uint16_t port, uint32_t recvbuf, uint32_t sendbuf);

    HLISTENER Listen(const std::string& ip,
        uint16_t port, uint32_t recvbuf, uint32_t sendbuf);

public:
    static std::string GetPeerIP(HSESSION s);

    static int32_t GetPeerPort(HSESSION s);

    static std::string GetLocalIP(HSESSION s);

    static int32_t GetLocalPort(HSESSION s);

    static uint32_t GetPeerIPInt(HSESSION s);

public:
    static uint32_t ParsePacket(HSESSION session,
        const char* pData, const uint32_t len);

    static void OnEstablish(HLISTENER handle, HSESSION session);

    static void OnTerminate(HSESSION session);

    static void OnError(HSESSION session, net_tcp_error moduleerror,
        int32_t systemerror);

    static void OnRecv(HSESSION session, const char *data,
        uint32_t len);

private:
    HTCPMANAGER m_netComponent;
};

//#define sNetSystem (*NetSystem::Instance())
#define sNetSystem MeyersSingleton<NetSystem>::Instance()
