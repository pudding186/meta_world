#pragma once
#include "meyers_singleton.hpp"
#include "DataDefine.hpp"

class CDataServerConfig
{
public:
    CDataServerConfig() = default;
    ~CDataServerConfig() = default;

    //SessionConfigData* GetGateSessionConfigData(uint32_t zone_id);
    //inline const std::string& GetSGID() { return m_sg_id; }
    //void UpdateGetListXML(const std::string& gate_list_xml);
    MysqlConfigData* GetMysqlConfigData();

    SessionConfigData* GetWorldServerConfigData();
protected:
private:
};

#define sDataServerConfig MeyersSingleton<CDataServerConfig>::Instance()
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