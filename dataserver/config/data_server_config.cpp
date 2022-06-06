#include "data_server_config.hpp"
#include "log_system.hpp"
#include "pugixml.hpp"


//SessionConfigData session_config_data;
//
//SessionConfigData* CAgentServerConfig::GetGateSessionConfigData(uint32_t zone_id)
//{
//    StrSafeCopy(session_config_data.ip, "192.168.10.28");
//    session_config_data.port = 8999;
//    session_config_data.recv_cache = 8096;
//    session_config_data.send_cache = 8096;
//    return &session_config_data;
//}
//
//void CAgentServerConfig::UpdateGetListXML(const std::string& gate_list_xml)
//{
//    pugi::xml_document doc;
//
//    auto result = doc.load_buffer(gate_list_xml.c_str(), gate_list_xml.size());
//
//    if (!result) {
//        LogERR("<{}>: load gatelist from xml string failed, error={}", __func__,
//            result.description());
//        return;
//    }
//
//
//}

MysqlConfigData mysql_config_data;

MysqlConfigData* CDataServerConfig::GetMysqlConfigData()
{
    mysql_config_data.port = 3306;
    StrSafeCopy(mysql_config_data.host, "192.168.10.28");
    StrSafeCopy(mysql_config_data.user, "root");
    StrSafeCopy(mysql_config_data.password, "root");
    StrSafeCopy(mysql_config_data.database, "dbmhxyzj1239264");
    return &mysql_config_data;
}

SessionConfigData world_server_config_data;

SessionConfigData* CDataServerConfig::GetWorldServerConfigData()
{
    StrSafeCopy(world_server_config_data.ip, "192.168.10.28");
    world_server_config_data.port = 1111;
    world_server_config_data.recv_cache = 1024000;
    world_server_config_data.send_cache = 1024000;

    return &world_server_config_data;
}
