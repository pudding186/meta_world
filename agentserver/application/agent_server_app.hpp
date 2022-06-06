#pragma once
#include "svr_app.hpp"
#include "version.h"
#include "meyers_singleton.hpp"
#include "agent/agent.hpp"
#include "agent/tcp_listener.hpp"

class CAgentServerApp:public SvrApp<CAgentServerApp>
{
public:

    std::string OnInitialize(void);
    bool OnUpdate(CFuncPerformanceInfo** info);
    void OnUninitialized(void);

    AsyncRet OnAsyncInit(void);
    AsyncRet OnAsyncUninit(void);

    static inline std::string GetName() { return "AgentServer"; }
    static inline std::string GetVersion() { return STRPRODUCTVER; }
protected:
private:
    TcpListener m_tcp_listener;
};

#define sAgentServerApp MeyersSingleton<CAgentServerApp>::Instance()