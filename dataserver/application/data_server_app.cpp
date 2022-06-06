#include "data_server_app.hpp"
#include "config/data_server_config.hpp"
#include "database/database_system.hpp"
#include "database/database_table_system.hpp"
#include "net/world_server.hpp"
#include "protocol/ErrorSystem.hpp"

std::string CDataServerApp::OnInitialize(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (!sDatabaseSystem.Initialize(this->MaxDBEventNum()))
    {
        return fmt::format(u8"database system init fail!");
    }
    else
    {
        LogSYS("Database System Init Success!");

        MysqlConfigData* mysql_config_data = sDataServerConfig.GetMysqlConfigData();

        if (!mysql_config_data)
        {
            return fmt::format(u8"Get Mysql Configure Fail!");
        }

        if (!sDatabaseSystem.Connect(
            mysql_config_data->port,
            mysql_config_data->host,
            mysql_config_data->user,
            mysql_config_data->password,
            mysql_config_data->database
        ))
        {
            return fmt::format(u8"Connect Mysql Fail!");
        }
        else
        {
            LogSYS(u8"Connect Mysql {} {} {} Success!",
                mysql_config_data->host,
                mysql_config_data->port,
                mysql_config_data->database);
        }
    }

    return sWMDatabaseTableSystem.Initialize();
}

bool CDataServerApp::OnUpdate(CFuncPerformanceInfo** info)
{
    FUNC_PERFORMANCE_CHECK();
    *info = &s_func_perf_info;

    CFuncPerformanceInfo* info_ptr;
    return sDatabaseSystem.OnUpdate(&info_ptr);
}

void CDataServerApp::OnUninitialized(void)
{
    FUNC_PERFORMANCE_CHECK();
    LogSYS("{}", __FUNCTION__);

    sDatabaseSystem.DisConnect();

    sDatabaseSystem.Uninitialized();
}

AsyncRet CDataServerApp::OnAsyncInit(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (!sWorldServer.IsConnected())
    {
        sWorldServer.ReConnect();
        return AsyncRet::ASYNC_CONTINUE;
    }

    if (!sWorldServer.InWork())
    {
        return AsyncRet::ASYNC_CONTINUE;
    }

    return AsyncRet::ASYNC_SUCCESS;
}

AsyncRet CDataServerApp::OnAsyncUninit(void)
{
    LogSYS("{}", __FUNCTION__);

    if (!sDatabaseSystem.TryStop())
    {
        return AsyncRet::ASYNC_CONTINUE;
    }

    return AsyncRet::ASYNC_SUCCESS;
}

