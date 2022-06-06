#include "back_session.hpp"
#include "agent.hpp"
#include "pkg_header.hpp"
#include "protocol/agent_protocol.hpp"
#include "log_system.hpp"

CBackSession::CBackSession(CAgent& agent, HSESSION session)
    :ISession(session, nullptr), m_agent(agent)
{

}

void CBackSession::OnEstablish(void)
{
    m_agent.SetBackState(SessionState::ESTABLISH);

    SetSendControl(1024, 5);

    LogWRN(u8"agent={} back establish!", (void*)(&m_agent));

    switch (m_agent.GetFrontState())
    {
    case SessionState::ESTABLISH:
    {
        char tmp_cache[64];

        *reinterpret_cast<uint32_t*>(tmp_cache) = sizeof(OldClientHandPkg) + sizeof(uint32_t);
        OldClientHandPkg* old_client_hand_pkg = reinterpret_cast<OldClientHandPkg*>(tmp_cache + sizeof(uint32_t));

        old_client_hand_pkg->zone_id = m_agent.GetZoneId();

        SendRawDataNoDelay(tmp_cache, *reinterpret_cast<uint32_t*>(tmp_cache));

        //ip_info info;
        //m_agent.GetFrontSession().GetPeerIPPort(info);
        addr_info info;
        m_agent.GetFrontSession().GetPeerSockAddr(info);

        IPNtf ntf;

        if (info.addr_type == addr_info::addr_v4) {
            ntf.ip = info.sock_addr_ptr.v4->sin_addr.s_addr;
            ntf.port = info.sock_addr_ptr.v4->sin_port;
        }
        else if (info.addr_type == addr_info::addr_v6) {
            ntf.ip = 0;
            ntf.port = 0;
        }

        uint8_t* head = nullptr;

        sAgentProtocolSystem.EnCodeProtocol(ntf, &head, sizeof(uint32_t));
        *reinterpret_cast<uint32_t*>(head) = static_cast<uint32_t>(sAgentProtocolSystem.EnCodeData().Length());

        SendRawDataNoDelay(sAgentProtocolSystem.EnCodeData().Data(), static_cast<uint32_t>(sAgentProtocolSystem.EnCodeData().Length()));

        OnDataCheck();
    }
    	break;
    default:
        break;
    }
}

void CBackSession::OnTerminate(void)
{
    LogWRN(u8"agent={} back terminate!", (void*)(&m_agent));
    m_agent.SetBackState(SessionState::TERMINATE);
    m_agent.ActiveTerminateCheck();
}

void CBackSession::OnError(net_tcp_error moduleerror, int32_t systemerror)
{
    LogWRN(u8"agent={} back error!", (void*)(&m_agent));
    (void)systemerror;
    if (moduleerror == error_connect_fail)
    {
        LogWRN(u8"agent={} connect fail!", (void*)(&m_agent));
        m_agent.SetBackState(SessionState::TERMINATE);
        m_agent.ActiveTerminateCheck();
    }
}

void CBackSession::OnRecvRawData(const char* data, uint32_t len)
{
    LogWRN(u8"agent={} recv Bpkg size={}", (void*)(&m_agent), len);
    m_agent.SetBackRecvBytes(m_agent.GetBackRecvBytes() + len);
    switch (m_agent.GetBackState())
    {
    case SessionState::ESTABLISH:
    {
        if (len != sizeof(OldClientHandRetPkg) + sizeof(uint32_t))
        {
            Disconnect();
            return;
        }

        OldClientHandRetPkg* old_client_hand_ret_pkg = reinterpret_cast<OldClientHandRetPkg*>(const_cast<char*>(data) + sizeof(uint32_t));
        
        old_client_hand_ret_pkg->over_load = 500;
        old_client_hand_ret_pkg->delay = 30;

        m_agent.GetFrontSession().SendRawData(data, len);

        m_agent.SetBackState(SessionState::WORK);
    }
    	break;
    case SessionState::WORK:
    {
        if (sAgentProtocolSystem.IsWatchProtocol(
            *reinterpret_cast<const uint16_t*>(data + sizeof(uint32_t)),
            *reinterpret_cast<const uint16_t*>(data + sizeof(uint32_t) + sizeof(uint16_t)),
            ProtocolSide::SERVER
        ))
        {
            sAgentProtocolSystem.SetCurrentProtocolSide(ProtocolSide::SERVER);
            sAgentProtocolSystem.SetCurrentAgent(&m_agent);
            sAgentProtocolSystem.HandleProtocol(data + sizeof(uint32_t), len - sizeof(uint32_t));
            sAgentProtocolSystem.SetCurrentAgent(nullptr);
        }
        else
        {
            m_agent.GetFrontSession().SendRawData(data, len);
        }
    }
        break;
    default:
        break;
    }
}
