#pragma once
#include "meyers_singleton.hpp"
#include "DataDefine.hpp"

class CAgentServerConfig
{
public:
    CAgentServerConfig():m_sg_id(u8"99"){}

    SessionConfigData* GetGateSessionConfigData(uint32_t zone_id);
    inline const std::string& GetSGID() { return m_sg_id; }
    void UpdateGetListXML(const std::string& gate_list_xml);

    SessionConfigData* GetTcpListenConfigData();
protected:
private:
    std::string              m_sg_id;
};

#define sAgentServerConfig MeyersSingleton<CAgentServerConfig>::Instance()
//class CAgentServerApp:public SvrApp<CAgentServerApp>
//{
//public:
//
//    std::string OnInitialize(void);
//    bool OnUpdate(void);
//    void OnUninitialized(void);
//
//    AsyncRet OnAsyncInit(void);
//    AsyncRet OnAsyncUninit(void);
//
//    static inline std::string GetName() { return "AgentServer"; }
//    static inline std::string GetVersion() { return STRPRODUCTVER; }
//protected:
//private:
//};
//
//#define sAgentServerApp MeyersSingleton<CAgentServerApp>::Instance()