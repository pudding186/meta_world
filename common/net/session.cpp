#include "session.hpp"
#include "net_system.hpp"


/// <summary>
/// ISession
/// </summary>
/// <param name="session"></param>
/// <param name="listener"></param>

ISession::ISession(HSESSION session /*= nullptr*/, IListener* listener /*= nullptr*/)
    : m_session(session), m_listener(listener), m_data_check_timer(this)
{

}

ISession::~ISession()
{
    if (m_data_check_timer.GetTimerInfo())
    {
        sTimerSystem.DelTimer(&m_data_check_timer);
    }
}

uint32_t ISession::ParsePacket(HSESSION session, const char* pData, const uint32_t len)
{
    ISession* svr = static_cast<ISession*>(net_tcp_get_session_data(session));

    return svr->OnParsePacket(pData, len);
}

void ISession::Establish(HLISTENER handle, HSESSION session)
{
    ISession* s = nullptr;

    if (handle)
    {
        IListener* l = static_cast<IListener*>(net_tcp_get_listener_data(handle));

        s = l->OnOpen(session);

        if (s == nullptr)
        {
            net_tcp_close_session(session);
            return;
        }

        s->m_session = session;
        net_tcp_set_session_data(session, s);
    }
    else
    {
        s = static_cast<ISession*>(net_tcp_get_session_data(session));
    }
    s->OnEstablish();
}

void ISession::Terminate(HSESSION session)
{
    ISession* svr = static_cast<ISession*>(net_tcp_get_session_data(session));

    svr->m_session = nullptr;

    svr->OnTerminate();

    if (svr->GetListener())
    {
        svr->GetListener()->OnClose(svr);
    }
}

void ISession::Error(HSESSION session, net_tcp_error moduleerror, int32_t systemerror)
{
    ISession* svr = static_cast<ISession*>(net_tcp_get_session_data(session));

    if (moduleerror == error_connect_fail)
    {
        svr->m_session = nullptr;
    }

    svr->OnError(moduleerror, systemerror);
}

void ISession::Recv(HSESSION session, const char* data, uint32_t len)
{
    ISession* svr = static_cast<ISession*>(net_tcp_get_session_data(session));

    svr->RecvRawData(data, len);
}

void ISession::RecvRawData(const char* data, uint32_t len)
{
    if (m_recv_cache.empty())
    {
        OnRecvRawData(data, len);
    }
    else
    {
        if (*(uint32_t*)m_recv_cache.c_str() == (uint32_t)m_recv_cache.size())
        {
            OnRecvRawData(m_recv_cache.c_str(), (uint32_t)(m_recv_cache.size()));

            m_recv_cache.clear();
        }
    }
}

bool ISession::SendRawData(const void* data, uint32_t len)
{
    if (!m_session)
    {
        return false;
    }

    uint32_t free_send_cache_size = net_tcp_get_send_free_size(m_session);

    if (m_send_cache.empty())
    {
        uint32_t data_left = len > free_send_cache_size ? len - free_send_cache_size : 0;

        net_tcp_send(m_session, data, len - data_left);

        if (data_left)
        {
            m_send_cache.append(static_cast<const char*>(data) + free_send_cache_size, data_left);

            ActiveDataCheck();
        }
    }
    else
    {
        uint32_t data_left = ((uint32_t)m_send_cache.size() + len) > free_send_cache_size
            ? ((uint32_t)m_send_cache.size() + len) - free_send_cache_size : 0;

        if (data_left)
        {
            if (free_send_cache_size >= (uint32_t)m_send_cache.size())
            {
                net_tcp_send(m_session, m_send_cache.c_str(), (uint32_t)m_send_cache.size());
                m_send_cache.clear();
            }
            else
            {
                net_tcp_send(m_session, m_send_cache.c_str(), free_send_cache_size);
                m_send_cache.erase(m_send_cache.begin(), 
                    m_send_cache.begin() + free_send_cache_size);
            }

            m_send_cache.append(static_cast<const char*>(data), len);

            ActiveDataCheck();
        }
        else
        {
            net_tcp_send(m_session, m_send_cache.c_str(), (uint32_t)m_send_cache.size());
            net_tcp_send(m_session, data, len);
            m_send_cache.clear();
        }
    }

    return true;
}

void ISession::CacheSendRawData(const void* data, uint32_t len)
{
    m_send_cache.append(static_cast<const char*>(data), len);
}

bool ISession::SendRawDataNoDelay(const void* data, uint32_t len)
{
    if (m_session)
    {
        return net_tcp_send(m_session, data, len);
    }

    return false;
}

bool ISession::Connect(const std::string& ip, uint16_t port, uint32_t recvbuf, uint32_t sendbuf)
{
    if (m_session)
    {
        return true;
    }

    m_session = net_tcp_connect(
        sNetSystem.GetNetComponent(),
        ip.c_str(),
        port,
        recvbuf,
        sendbuf,
        false,
        nullptr,
        0,
        &ISession::Establish,
        &ISession::Terminate,
        &ISession::Error,
        &ISession::Recv,
        &ISession::ParsePacket
    );

    if (m_session)
    {
        net_tcp_set_session_data(m_session, this);
        return true;
    }

    return false;
}

void ISession::Disconnect(void)
{
    if (m_session)
    {
        net_tcp_close_session(m_session);
        m_session = nullptr;
    }
}

void ISession::ActiveDataCheck(void)
{
    if (!sTimerSystem.GetRemainCount(&m_data_check_timer))
    {
        sTimerSystem.ModifyTimer(&m_data_check_timer, 15, 1);
    }
}

void ISession::OnDataCheck(void)
{
    if (m_session &&
        !m_send_cache.empty())
    {
        uint32_t free_send_cache_size = net_tcp_get_send_free_size(m_session);

        uint32_t data_left = (uint32_t)m_send_cache.size() > free_send_cache_size
            ? (uint32_t)m_send_cache.size() - free_send_cache_size : 0;

        if (data_left)
        {
            if (free_send_cache_size >= (uint32_t)m_send_cache.size())
            {
                net_tcp_send(m_session, m_send_cache.c_str(), (uint32_t)m_send_cache.size());
                m_send_cache.clear();
            }
            else
            {
                net_tcp_send(m_session, m_send_cache.c_str(), free_send_cache_size);
                m_send_cache.erase(m_send_cache.begin(), m_send_cache.begin() + free_send_cache_size);
            }

            ActiveDataCheck();
        }
        else
        {
            net_tcp_send(m_session, m_send_cache.c_str(), (uint32_t)m_send_cache.size());
            m_send_cache.clear();
        }
    }
}


uint32_t ISession::OnParsePacket(const char* data, const uint32_t len)
{
    if (m_recv_cache.empty())
    {
        if (len < (uint32_t)sizeof(int32_t))
        {
            return 0;
        }

        uint32_t pkg_len = *(uint32_t*)data;

        if (pkg_len < (uint32_t)sizeof(uint32_t))
        {
            return (uint32_t)-1;
        }

        if (pkg_len > len)
        {
            if (pkg_len > net_tcp_get_recv_max_size(m_session))
            {
                m_recv_cache.append(data, len);
                return len;
            }

            return 0;
        }

        return pkg_len;
    }
    else
    {
        uint32_t large_pkg_len = *(uint32_t*)m_recv_cache.c_str();

        if ((uint32_t)m_recv_cache.size()+len >= large_pkg_len)
        {
            uint32_t left = large_pkg_len - (uint32_t)m_recv_cache.size();

            m_recv_cache.append(data, left);

            return left;
        }
        else
        {
            m_recv_cache.append(data, len);
        }

        return len;
    }
}


bool ISession::GetPeerIPPort(ip_info& info)
{
    return net_tcp_get_peer_ip_port(m_session, &info);
}

bool ISession::GetLocalIPPort(ip_info& info)
{
    return net_tcp_get_local_ip_port(m_session, &info);
}


bool ISession::GetPeerSockAddr(addr_info& info)
{
    return net_tcp_get_peer_sock_addr(m_session, &info);
}

bool ISession::GetLocalSockAddr(addr_info& info)
{
    return net_tcp_get_local_sock_addr(m_session, &info);
}

void ISession::SetSendControl(uint32_t pkg_size, uint32_t delay_tick)
{
    FUNC_PERFORMANCE_CHECK();

    net_tcp_set_send_control(m_session, pkg_size, delay_tick);
}

uint32_t ISession::LeftSendDataSize(void)
{
    if (m_session)
    {
        return net_tcp_unsend_size(m_session);
    }

    return 0;
}

/// <summary>
/// IListener
/// </summary>
/// <param name="listener"></param>

IListener::IListener(HLISTENER listener /*= nullptr*/)
    :m_listener(listener)
{

}

IListener::~IListener()
{

}

bool IListener::Listen(const std::string& ip, uint16_t port, uint32_t recvbuf, uint32_t sendbuf)
{
    if (m_listener)
    {
        return true;
    }

    const char* csz_ip = ip.c_str();
    if (ip.empty() || ip =="0")
    {
        csz_ip = 0;
    }

    m_listener = net_tcp_listen(
        sNetSystem.GetNetComponent(),
        csz_ip,
        port,
        recvbuf,
        sendbuf,
        false,
        &ISession::Establish,
        &ISession::Terminate,
        &ISession::Error,
        &ISession::Recv,
        &ISession::ParsePacket
    );

    if (m_listener)
    {
        net_tcp_set_listener_data(m_listener, this);
        return true;
    }

    return false;
}

void IListener::Close(void)
{
    if (m_listener)
    {
        net_tcp_close_listener(m_listener);
        m_listener = nullptr;
    }
}

DataCheckTimer::DataCheckTimer(ISession* session)
    :m_session(session)
{

}

DataCheckTimer::~DataCheckTimer()
{

}

void DataCheckTimer::OnTimer(void)
{
    m_session->OnDataCheck();
}
