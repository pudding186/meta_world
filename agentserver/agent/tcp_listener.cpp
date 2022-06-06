#include "tcp_listener.hpp"
#include "agent.hpp"
#include "log_system.hpp"

ISession* TcpListener::OnOpen(HSESSION session)
{
    FUNC_PERFORMANCE_CHECK();

    CAgent* agent = S_NEW(CAgent, 1, session, this);

    LogWRN("agent={} NEW", (void*)agent);

    return &agent->GetFrontSession();
}

void TcpListener::OnClose(ISession* s)
{
    (void)s;
    //CFrontSession* front_session = static_cast<CFrontSession*>(s);
    //front_session->GetAgent().ActiveTerminateCheck();
}
