#include "utility.hpp"
#include "net_system.hpp"
#include "session.hpp"

NetSystem::NetSystem()
    :
    m_netComponent(nullptr)
{

}

NetSystem::~NetSystem()
{

}

bool NetSystem::Initialize(uint32_t max_socket_num, uint32_t max_io_thread_num, uint32_t max_accept_ex_num, uint32_t max_event_num)
{
    FUNC_PERFORMANCE_CHECK();

    m_netComponent = create_net_tcp
    (
        OnEstablish,
        OnTerminate,
        OnError,
        OnRecv,
        NetSystem::ParsePacket,
        max_socket_num,
        max_io_thread_num,
        max_accept_ex_num,
        max_event_num
    );
    if (!m_netComponent)
    {
        return false;
    }

    return true;
}

void NetSystem::Uninitialized(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_netComponent)
    {
        destroy_net_tcp(m_netComponent);
        m_netComponent = nullptr;
    }
}

bool NetSystem::OnUpdate(CFuncPerformanceInfo** info)
{
    FUNC_PERFORMANCE_CHECK();
    *info = &s_func_perf_info;
    return net_tcp_run(m_netComponent, 0);
}

HSESSION NetSystem::Connect(const std::string& ip, uint16_t port, uint32_t recvbuf, uint32_t sendbuf)
{
    return net_tcp_connect(
        m_netComponent,
        ip.c_str(),
        port,
        recvbuf,
        sendbuf,
        false,
        0,
        0,
        OnEstablish,
        OnTerminate,
        OnError,
        OnRecv,
        NetSystem::ParsePacket
    );
}

HLISTENER NetSystem::Listen(const std::string& ip, uint16_t port, uint32_t recvbuf, uint32_t sendbuf)
{
    return net_tcp_listen(
        m_netComponent,
        ip.c_str(),
        port,
        recvbuf,
        sendbuf,
        false,
        OnEstablish,
        OnTerminate,
        OnError,
        OnRecv,
        NetSystem::ParsePacket
    );
}

std::string NetSystem::GetPeerIP(HSESSION s)
{
    FUNC_PERFORMANCE_CHECK();

    ip_info ipinfo;
    if (!net_tcp_get_peer_ip_port(s, &ipinfo))
    {
        return u8"";
    }

    return ipinfo.ip_str;
}

int32_t NetSystem::GetPeerPort(HSESSION s)
{
    FUNC_PERFORMANCE_CHECK();

    ip_info ipinfo;
    if (!net_tcp_get_peer_ip_port(s, &ipinfo))
    {
        return 0;
    }

    return ipinfo.port;
}

std::string NetSystem::GetLocalIP(HSESSION s)
{
    FUNC_PERFORMANCE_CHECK();

    ip_info ipinfo;
    if (!net_tcp_get_local_ip_port(s, &ipinfo))
    {
        return u8"";
    }

    return ipinfo.ip_str;
}

int32_t NetSystem::GetLocalPort(HSESSION s)
{
    FUNC_PERFORMANCE_CHECK();

    ip_info ipinfo;
    if (!net_tcp_get_local_ip_port(s, &ipinfo))
    {
        return 0;
    }

    return ipinfo.port;
}

uint32_t NetSystem::GetPeerIPInt(HSESSION s)
{
    FUNC_PERFORMANCE_CHECK();

    addr_info info;
    if (!net_tcp_get_peer_sock_addr(s, &info))
    {
        return 0;
    }

    return (uint32_t)info.sock_addr_ptr.v4->sin_addr.s_addr;
}

uint32_t NetSystem::ParsePacket(HSESSION session, const char* pData, const uint32_t len)
{
    FUNC_PERFORMANCE_CHECK();

    ISession* svr = (ISession*)net_tcp_get_session_data(session);
    if (svr)
    {
        return svr->OnParsePacket(pData, len);
    }
    return (uint32_t)-1;
}

void NetSystem::OnEstablish(HLISTENER handle, HSESSION session)
{
    FUNC_PERFORMANCE_CHECK();

    ISession* s = nullptr;

    if (handle)
    {
        IListener* l = (IListener*)net_tcp_get_listener_data(handle);

        s = l->OnAccept(session);

        if (s == nullptr)
        {
            net_tcp_close_session(session);
            return;
        }

        s->SetSession(session);
        net_tcp_set_session_data(session, s);
    }
    else
    {
        s = (ISession*)net_tcp_get_session_data(session);
    }

    s->OnEstablish();
}

void NetSystem::OnTerminate(HSESSION session)
{
    FUNC_PERFORMANCE_CHECK();

    ISession *svr = (ISession *)net_tcp_get_session_data(session);
    if (svr)
    {
        svr->OnTerminate();

        if (svr->GetListener())
        {
            svr->GetListener()->OnClose(svr);
        }

        svr->SetSession(nullptr);
    }
}

void NetSystem::OnError(HSESSION session,
    net_tcp_error moduleerror, int32_t systemerror)
{
    FUNC_PERFORMANCE_CHECK();

    ISession *svr = (ISession *)net_tcp_get_session_data(session);
    if (svr)
    {
        svr->OnError(moduleerror, systemerror);

        if (moduleerror == error_connect_fail)
        {
            svr->SetSession(nullptr);
        }
    }     
}

void NetSystem::OnRecv(HSESSION session, const char* data, uint32_t len)
{
    FUNC_PERFORMANCE_CHECK();

    ISession *svr = (ISession *)net_tcp_get_session_data(session);
    if (svr)
    {
        svr->RecvRawData(data, len);
    }
}
