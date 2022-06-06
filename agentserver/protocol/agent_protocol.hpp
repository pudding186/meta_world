#pragma once
#include "meyers_singleton.hpp"
#include "protocol_system.hpp"

#include "protocol/ClientCS.hpp"
#include "protocol/ClientGS.hpp"
#include "protocol/ClientWS.hpp"
#include "protocol/STClientGS.hpp"
#include "protocol/STGTSG.hpp"

class CAgent;

class ClientCSImpl:public CClientCS<ClientCSImpl>
{
public:
protected:
private:
};

class ClientGSImpl :public CClientGS<ClientGSImpl>
{
public:
protected:
private:
};

class ClientWSImpl :public CClientWS<ClientWSImpl>
{
public:
protected:
private:
};

class STClientGSImpl :public CSTClientGS<STClientGSImpl>
{
public:
    void OnRecv_STNetDelayReq(STNetDelayReq& rstProtocol);
    void OnRecv_STNetDelayAck(STNetDelayAck& rstProtocol);
protected:
private:
};

class STSGGTImpl :public CSTGTSG<STSGGTImpl>
{
public:
protected:
private:
};


class CAgentProtocolSystem:public ProtocolSystem<ClientGSImpl, ClientCSImpl, ClientWSImpl, STClientGSImpl, STSGGTImpl>
{
public:
    CAgentProtocolSystem():m_agent(nullptr), m_protocol_side(ProtocolSide::CLIENT){}

    bool Init(void);

    bool HandleProtocol(const void* data, uint32_t size);

    inline void SetCurrentAgent(CAgent* agent) { m_agent = agent; }
    inline CAgent* GetCurrentAgent(void) { return m_agent; }

    inline void SetCurrentProtocolSide(ProtocolSide side) { m_protocol_side = side; }
    inline ProtocolSide GetCurrentProtocolSide(void) { return m_protocol_side; }
protected:
private:
    CAgent* m_agent;
    ProtocolSide m_protocol_side;
};


#define sAgentProtocolSystem MeyersSingleton<CAgentProtocolSystem>::Instance()
