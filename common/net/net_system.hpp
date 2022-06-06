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

    inline HTCPMANAGER GetNetComponent(void) const { return m_netComponent; }

private:
    HTCPMANAGER m_netComponent;
};

#define sNetSystem MeyersSingleton<NetSystem>::Instance()
