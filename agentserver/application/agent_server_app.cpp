#include "agent_server_app.hpp"
#include "protocol/agent_protocol.hpp"
#include "config/agent_server_config.hpp"

#include "client_mysql.h"

HCLIENTMYSQL g_mysql_client;

DECLEAR_LOG_PREFIX(OMG, print_color::blue)
#define LogSHIT(f, ...) LogImpl(OMG, sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)

std::string CAgentServerApp::OnInitialize(void)
{
    sAgentProtocolSystem.Init();

    SessionConfigData* listen_config_data = sAgentServerConfig.GetTcpListenConfigData();

    if (!m_tcp_listener.Listen(
        listen_config_data->ip,
        listen_config_data->port, 
        listen_config_data->recv_cache, 
        listen_config_data->send_cache))
    {
        return fmt::format(u8"Listen port=8989 fail");
    }
    else
    {
        LogSYS(u8"Listen port={} success!", listen_config_data->port);
    }

    return "";
}

bool CAgentServerApp::OnUpdate(CFuncPerformanceInfo** info)
{
    (void)info;
    return false;
}

void CAgentServerApp::OnUninitialized(void)
{
    LogSYS("{}", __FUNCTION__);

    m_tcp_listener.Close();
}

AsyncRet CAgentServerApp::OnAsyncInit(void)
{
    LogSYS("{}", __FUNCTION__);
    return AsyncRet::ASYNC_SUCCESS;
}

AsyncRet CAgentServerApp::OnAsyncUninit(void)
{
    LogSYS("{}", __FUNCTION__);
    return AsyncRet::ASYNC_SUCCESS;
}
