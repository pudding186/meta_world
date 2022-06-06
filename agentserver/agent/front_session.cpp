#include "front_session.hpp"
#include "agent.hpp"
#include "log_system.hpp"
#include "pkg_header.hpp"
#include "config/agent_server_config.hpp"
#include "protocol/agent_protocol.hpp"

CFrontSession::CFrontSession(CAgent& agent, HSESSION session, IListener* listener)
    : ISession(session, listener), m_agent(agent)
{

}

void CFrontSession::OnEstablish(void)
{
    m_agent.SetFrontState(SessionState::ESTABLISH);

    SetSendControl(1024, 5);

    LogWRN(u8"agent={} front establish!", (void*)(&m_agent));

    m_agent.SetBackState(SessionState::WAIT_CONNECT);
}

void CFrontSession::OnTerminate(void)
{
    LogWRN(u8"agent={} front terminate!", (void*)(&m_agent));
    m_agent.SetFrontState(SessionState::TERMINATE);

    if (m_agent.GetBackState() == SessionState::WAIT_CONNECT ||
        m_agent.GetBackState() == SessionState::WAIT_CHECK)
    {
        m_agent.SetBackState(SessionState::TERMINATE);
    }

    m_agent.ActiveTerminateCheck();
}

void CFrontSession::OnError(net_tcp_error moduleerror, int32_t systemerror)
{
    LogWRN(u8"agent={} front error!", (void*)(&m_agent));
    LogERR(u8"{} module error={} system error={}", __FUNCTION__, moduleerror, systemerror);
}

void CFrontSession::OnRecvRawData(const char* data, uint32_t len)
{
    LogWRN(u8"agent={} recv Fpkg size={}", (void*)(&m_agent), len);

    m_agent.SetFrontRecvBytes(m_agent.GetFrontRecvBytes() + len);
    switch (m_agent.GetBackState())
    {
    case SessionState::CONNECT:
    {
        m_agent.GetBackSession().CacheSendRawData(data, len);
    }
    break;
    case SessionState::WAIT_CONNECT:
    {
        OldClientHandRetPkg old_client_hand_ret_pkg;

        if (len != static_cast<uint32_t>(sizeof(OldClientHandPkg) + sizeof(uint32_t)))
        {
            m_agent.SetBackState(SessionState::TERMINATE);
            

            ip_info info;
            GetPeerIPPort(info);

            Disconnect();
            LogWRN(u8"<{}> agent addr={} front ip={} port={} handshake pkg fail!",
                __FUNCTION__, (void*)this, info.ip_str, info.port);

            return;
        }

        const OldClientHandPkg& old_client_hand_pkg = *reinterpret_cast<const OldClientHandPkg*>(data + sizeof(uint32_t));

        switch (old_client_hand_pkg.version)
        {
        case 0:
        case 1:
        {
            SessionConfigData* session_config_data = sAgentServerConfig.GetGateSessionConfigData(old_client_hand_pkg.zone_id);
            if (!session_config_data)
            {
                old_client_hand_ret_pkg.result = static_cast<uint32_t>(HandError::ZONE_NOT_EXSIT);
                goto ERROR_DEAL;
            }

            if (m_agent.GetBackSession().Connect(
                session_config_data->ip,
                session_config_data->port,
                session_config_data->send_cache,
                session_config_data->recv_cache
            ))
            {
                m_agent.SetBackState(SessionState::CONNECT);
                m_agent.SetZoneId(old_client_hand_pkg.zone_id);
                LogWRN(u8"agent={} Connect Back ip={} port={} zone={}", (void*)(&m_agent), session_config_data->ip, session_config_data->port, old_client_hand_pkg.zone_id);
            }
            else
            {
                old_client_hand_ret_pkg.result = static_cast<uint32_t>(HandError::ZONE_CONNECT_FAIL);
                goto ERROR_DEAL;
            }
        }
        break;
        case 2:
        {
            m_agent.SetZoneId(old_client_hand_pkg.zone_id);

            char tmp_buf[sizeof(ClientKeyReq1) + sizeof(uint32_t)];
            *reinterpret_cast<uint32_t*>(tmp_buf) = sizeof(ClientKeyReq1) + sizeof(uint32_t);
            ClientKeyReq1* req = reinterpret_cast<ClientKeyReq1*>(tmp_buf + sizeof(uint32_t));

            req->version = 2;
            req->seed1 = rand_integer(1, 100000000);
            req->seed2 = rand_integer(1, 100000000);
            req->check = rand_integer(1, 100000000);

            if (!SendRawData(tmp_buf, *reinterpret_cast<uint32_t*>(tmp_buf)))
            {
                old_client_hand_ret_pkg.result = static_cast<uint32_t>(HandError::GEN_KEY_FAIL);
                goto ERROR_DEAL;
            }

            m_agent.SetBackState(SessionState::WAIT_CHECK);
        }
        break;
        default:
        {
            old_client_hand_ret_pkg.result = static_cast<uint32_t>(HandError::VERSION_NOT_MATCH);
            goto ERROR_DEAL;
        }
        }

        return;

    ERROR_DEAL:
        m_agent.SetBackState(SessionState::TERMINATE);
        Disconnect();

        ip_info info;
        GetPeerIPPort(info);

        LogWRN(u8"<{}> agent addr={} front ip={} port={} connect zone {} fail, err= {}!",
            __FUNCTION__, (void*)this, info.ip_str, info.port, old_client_hand_pkg.zone_id, old_client_hand_ret_pkg.result);
    }
    break;
    case SessionState::WAIT_CHECK:
    {
        OldClientHandRetPkg old_client_hand_ret_pkg;

        if (len == static_cast<uint32_t>(sizeof(ClientKeyAck1) + sizeof(uint32_t)))
        {
            SessionConfigData* session_config_data = sAgentServerConfig.GetGateSessionConfigData(m_agent.GetZoneId());
            if (!session_config_data)
            {
                old_client_hand_ret_pkg.result = static_cast<uint32_t>(HandError::ZONE_NOT_EXSIT);
                goto ERROR_DEAL1;
            }

            if (m_agent.GetBackSession().Connect(
                session_config_data->ip,
                session_config_data->port,
                session_config_data->send_cache,
                session_config_data->recv_cache
            ))
            {
                m_agent.SetBackState(SessionState::CONNECT);
            }
            else
            {
                old_client_hand_ret_pkg.result = static_cast<uint32_t>(HandError::ZONE_CONNECT_FAIL);
                goto ERROR_DEAL1;
            }
        }
        else
        {
            m_agent.GetBackSession().CacheSendRawData(data, len);
        }

        return;

    ERROR_DEAL1:
        m_agent.SetBackState(SessionState::TERMINATE);
        Disconnect();

        ip_info info;
        GetPeerIPPort(info);

        LogWRN(u8"<{}> agent addr={} front ip={} port={} connect zone {} fail, err= {}!",
            __FUNCTION__, (void*)this, info.ip_str, info.port, m_agent.GetZoneId(), old_client_hand_ret_pkg.result);
    }
    break;
    case SessionState::ESTABLISH:
    case SessionState::WORK:
    {
        if (sAgentProtocolSystem.IsWatchProtocol(
            *reinterpret_cast<const uint16_t*>(data+sizeof(uint32_t)),
            *reinterpret_cast<const uint16_t*>(data + sizeof(uint32_t)+sizeof(uint16_t)),
            ProtocolSide::CLIENT
        ))
        {
            sAgentProtocolSystem.SetCurrentProtocolSide(ProtocolSide::CLIENT);
            sAgentProtocolSystem.SetCurrentAgent(&m_agent);
            sAgentProtocolSystem.HandleProtocol(data + sizeof(uint32_t), len - sizeof(uint32_t));
            sAgentProtocolSystem.SetCurrentAgent(nullptr);
        }
        else
        {
            m_agent.GetBackSession().SendRawData(data, len);
        }
    }
    break;
    default:
        break;
    }
}
