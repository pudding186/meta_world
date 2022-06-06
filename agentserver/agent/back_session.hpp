#pragma once
#include "session.hpp"
#include "net_data.hpp"
#include "protocol/agent_protocol.hpp"

class CAgent;

class CBackSession :public ISession
{
public:
    CBackSession(CAgent& agent, HSESSION session);
    ~CBackSession() = default;

    void OnEstablish(void) override;

    void OnTerminate(void) override;

    void OnError(net_tcp_error moduleerror, int32_t systemerror) override;

    void OnRecvRawData(const char* data, uint32_t len) override;

    inline CAgent& GetAgent(void) { return m_agent; }

    template<typename T>
    bool SendPkg(TProtocol<T>& proto)
    {
        uint8_t* head = nullptr;

        sAgentProtocolSystem.EnCodeProtocol(proto, &head, sizeof(uint32_t));
        *reinterpret_cast<uint32_t*>(head) = static_cast<uint32_t>(sAgentProtocolSystem.EnCodeData().Length());

        return SendRawData(sAgentProtocolSystem.EnCodeData().Data(), static_cast<uint32_t>(sAgentProtocolSystem.EnCodeData().Length()));
    }
protected:
    CAgent& m_agent;
private:
};