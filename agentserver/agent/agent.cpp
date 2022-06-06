#include "agent.hpp"
#include "pkg_header.hpp"
#include "log_system.hpp"

CAgent::CAgent(HSESSION session, IListener* listener)
    :m_front_session(*this, session, listener), 
    m_back_session(*this, nullptr),
    m_front_state(SessionState::NONE),
    m_back_state(SessionState::NONE),
    m_call_check_terminate(nullptr),
    m_call_check_session(nullptr),
    m_zone_id(0),
    m_front_send_bytes(0),
    m_front_recv_bytes(0),
    m_back_send_bytes(0),
    m_back_recv_bytes(0),
    m_last_front_send_bytes(0),
    m_last_front_recv_bytes(0),
    m_last_back_send_bytes(0),
    m_last_back_recv_bytes(0),
    m_last_front_recv_time(0),
    m_last_back_recv_time(0)
{
    ActiveSessionCheck();
}


CAgent::~CAgent(void)
{
    if (m_call_check_terminate)
    {
        sTimerSystem.DestroyCaller(m_call_check_terminate);
        m_call_check_terminate = nullptr;
    }

    if (m_call_check_session)
    {
        sTimerSystem.DestroyCaller(m_call_check_session);
        m_call_check_session = nullptr;
    }
}

void CAgent::ActiveTerminateCheck(void)
{
    if (m_call_check_terminate)
    {
        sTimerSystem.ModifyTimer(m_call_check_terminate, 1000, -1);
    }
    else
    {
        m_call_check_terminate = sTimerSystem.CreateTimeCaller(1000, -1, std::bind(&CAgent::CheckTerminate, this));
    }
}

void CAgent::ActiveSessionCheck(void)
{
    if (m_call_check_session)
    {
        sTimerSystem.ModifyTimer(m_call_check_session, 60000, -1);
    }
    else
    {
        m_call_check_session = sTimerSystem.CreateTimeCaller(60000, -1, std::bind(&CAgent::CheckSession, this));
    }
}

void CAgent::CheckTerminate(void)
{
    if (m_front_state == SessionState::ESTABLISH)
    {
        if (!m_front_session.SendCacheSize())
        {
            LogWRN(u8"agent={} front disconnect", (void*)this);
            m_front_session.Disconnect();
        }
    }

    if (m_back_state == SessionState::ESTABLISH ||
        m_back_state == SessionState::WORK)
    {
        if (!m_back_session.SendCacheSize())
        {
            LogWRN(u8"agent={} back disconnect", (void*)this);
            m_back_session.Disconnect();
        }
    }

    if (m_front_state == SessionState::TERMINATE &&
        m_back_state == SessionState::TERMINATE)
    {
        LogWRN("agent={} DELETE", (void*)this);
        S_DELETE(this);
    }
}

void CAgent::CheckSession(void)
{
    time_t cur_time = get_time();

    if (m_back_state == SessionState::ESTABLISH ||
        m_back_state == SessionState::WORK)
    {
        if (m_back_recv_bytes != m_last_back_recv_bytes)
        {
            m_last_back_recv_bytes = m_back_recv_bytes;
            m_last_back_recv_time = cur_time;
        }
        else
        {
            if (cur_time - m_last_back_recv_time > 60)
            {
                LogWRN(u8"<{}> Agent={} back long time no data, disconnect",
                    __FUNCTION__, (void*)this);

                LogWRN(u8"agent={} long time no data back disconnect", (void*)this);
                m_back_session.Disconnect();
            }
        }
    }

    if (m_front_state == SessionState::ESTABLISH)
    {
        if (m_front_recv_bytes != m_last_front_recv_bytes)
        {
            m_last_front_recv_bytes = m_front_recv_bytes;
            m_last_front_recv_time = cur_time;
        }
        else
        {
            if (cur_time - m_last_front_recv_time > 60)
            {
                LogWRN(u8"<{}> Agent={} front long time no data, disconnect",
                    __FUNCTION__, (void*)this);

                LogWRN(u8"agent={} long time no data front disconnect", (void*)this);
                m_front_session.Disconnect();
            }
        }
    }
}
