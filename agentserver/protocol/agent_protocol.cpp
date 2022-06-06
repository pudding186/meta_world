#include "agent_protocol.hpp"
#include "agent/agent.hpp"
#include "config/agent_server_config.hpp"
#include "timer.h"

bool CAgentProtocolSystem::Init(void)
{
    SetWatchProtocol<STNetDelayReq>(ProtocolSide::SERVER);
    SetWatchProtocol<STNetDelayAck>(ProtocolSide::CLIENT);
    SetWatchProtocol<STNetDelayReq>(ProtocolSide::CLIENT);
    SetWatchProtocol<STEnterGSAck>(ProtocolSide::SERVER);

    return true;
}

bool CAgentProtocolSystem::HandleProtocol(const void* data, uint32_t size)
{
    switch (*reinterpret_cast<const uint16_t*>(data))
    {
    case ClientGSImpl::module_id:
        return DeCodeProtocol<ClientGSImpl>(data, size);
    	break;
    case ClientWSImpl::module_id:
        return DeCodeProtocol<ClientWSImpl>(data, size);
        break;
    case ClientCSImpl::module_id:
        return DeCodeProtocol<ClientCSImpl>(data, size);
        break;
    case STClientGSImpl::module_id:
        return DeCodeProtocol<STClientGSImpl>(data, size);
        break;
    default:
        return false;
    }
}

void STClientGSImpl::OnRecv_STNetDelayReq(STNetDelayReq& rstProtocol)
{
    switch (sAgentProtocolSystem.GetCurrentProtocolSide())
    {
    case ProtocolSide::SERVER:
    {
        rstProtocol.sg_tick = get_tick();
        sAgentProtocolSystem.GetCurrentAgent()->GetFrontSession().SendPkg(rstProtocol);
    }
    break;
    case ProtocolSide::CLIENT:
    {
        STNetDelayAck ack;

        ack.sg_elpase = rstProtocol.sg_tick;
        ack.gs_elpase = rstProtocol.gs_tick;
        ack.gt_elpase = rstProtocol.gt_tick;
        ack.taobao_delay = 0;
        ack.baidu_delay = 0;

        sAgentProtocolSystem.GetCurrentAgent()->GetFrontSession().SendPkg(ack);
    }
    break;
    }
}

void STClientGSImpl::OnRecv_STNetDelayAck(STNetDelayAck& rstProtocol)
{
    rstProtocol.sg_elpase = get_tick() - rstProtocol.sg_elpase;

    StrSafeCopy(rstProtocol.sg_id, sAgentServerConfig.GetSGID());

    if (rstProtocol.gs_elpase || rstProtocol.gt_elpase)
    {
        sAgentProtocolSystem.GetCurrentAgent()->GetBackSession().SendPkg(rstProtocol);
    }
}
