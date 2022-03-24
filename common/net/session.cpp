#include "session.hpp"
#include "net_system.hpp"

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

void ISession::RecvRawData(const char* data, uint32_t len)
{
    if (m_recv_cache.empty())
    {
        OnRecv(data + sizeof(int32_t), len - sizeof(int32_t));
    }
    else
    {
        if (*(uint32_t*)m_recv_cache.c_str() == (uint32_t)m_recv_cache.size())
        {
            OnRecv(m_recv_cache.c_str() + sizeof(int32_t), (uint32_t)(m_recv_cache.size() - sizeof(int32_t)));

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

bool ISession::Connect(const std::string& ip, uint16_t port, uint32_t recvbuf, uint32_t sendbuf)
{
    if (m_session)
    {
        return true;
    }

    m_session = sNetSystem.Connect(ip, port, recvbuf, sendbuf);

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

bool IListener::Listen(const std::string& ip, uint16_t port, uint32_t sendbuf, uint32_t recvbuf)
{
    if (m_listener)
    {
        return true;
    }

    m_listener = sNetSystem.Listen(ip, port, recvbuf, sendbuf);

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
