#pragma once
#include "db_mysql.hpp"
#include "meyers_singleton.hpp"

using DB_CONFIG = struct ST_DB_CONFIG
{
    uint16_t Port;
    std::string Host;
    std::string User;
    std::string PWD;
    std::string Charset;
    std::string Database;
};

enum class DBConnectionFlag :uint32_t
{
    SYNC = 0,
    ASYNC1,
    ASYNC2,
    ASYNC3,
    ASYNC4,
    MAX
};

class DatabaseSystem
{
public:
    DatabaseSystem();

    ~DatabaseSystem();

public:
    bool Initialize(size_t max_db_event_num);

    void Uninitialized(void);

    bool Connect(const DB_CONFIG& db_config);

    void DisConnect(void);

    bool OnUpdate(CFuncPerformanceInfo** info);

    void OnError(const std::string& err_msg);

    void OnWarn(const std::string& err_msg);

    HMYSQLCONNECTION GetMysqlAsyncConnection(DBConnectionFlag flag);

    HMYSQLCONNECTION GetMysqlSyncConnection(void);

private:
    bool _CreateAndUseDataBase(const DB_CONFIG& db_config);
    bool _CreateWorkConnections(const DB_CONFIG& db_config);
    void _DestroyWorkConnections(void);
private:
    HMYSQLCONNECTION m_connection[static_cast<uint32_t>(DBConnectionFlag::MAX)];
};

//#define sDatabaseSystem (*DatabaseSystem::Instance())
#define sDatabaseSystem MeyersSingleton<DatabaseSystem>::Instance()
