#include "agent_server_config.hpp"
#include "log_system.hpp"
#include "pugixml.hpp"


SessionConfigData session_config_data;

SessionConfigData* CAgentServerConfig::GetGateSessionConfigData(uint32_t zone_id)
{
    StrSafeCopy(session_config_data.ip, "192.168.10.28");
    session_config_data.port = 8999;
    session_config_data.recv_cache = 8096;
    session_config_data.send_cache = 8096;
    return &session_config_data;
}

void CAgentServerConfig::UpdateGetListXML(const std::string& gate_list_xml)
{
    pugi::xml_document doc;

    auto result = doc.load_buffer(gate_list_xml.c_str(), gate_list_xml.size());

    if (!result) {
        LogERR("<{}>: load gatelist from xml string failed, error={}", __func__,
            result.description());
        return;
    }


}

SessionConfigData tcp_listen_config_data;

SessionConfigData* CAgentServerConfig::GetTcpListenConfigData()
{
    StrSafeCopy(tcp_listen_config_data.ip, "0");
    tcp_listen_config_data.port = 8989;
    tcp_listen_config_data.recv_cache = 8096;
    tcp_listen_config_data.send_cache = 8096;
    return &tcp_listen_config_data;
}
