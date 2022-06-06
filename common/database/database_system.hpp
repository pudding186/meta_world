#pragma once
#include "db_mysql.hpp"
#include "meyers_singleton.hpp"

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

    bool Connect(
        uint16_t port,
        const std::string& host,
        const std::string& user,
        const std::string& pwd,
        const std::string& database,
        const std::string& charset="utf8mb4"
    );

    void DisConnect(void);

    bool OnUpdate(CFuncPerformanceInfo** info);

    void OnError(const std::string& err_msg);

    void OnWarn(const std::string& err_msg);

    bool TryStop(void);

    HMYSQLCONNECTION GetMysqlAsyncConnection(DBConnectionFlag flag);

    HMYSQLCONNECTION GetMysqlSyncConnection(void);

private:
    bool _CreateAndUseDataBase(
        uint16_t port,
        const std::string& host,
        const std::string& user,
        const std::string& pwd,
        const std::string& database,
        const std::string& charset = "utf8mb4"
    );

    bool _CreateWorkConnections(
        uint16_t port,
        const std::string& host,
        const std::string& user,
        const std::string& pwd,
        const std::string& database,
        const std::string& charset = "utf8mb4"
    );

    void _DestroyWorkConnections(void);
private:
    HMYSQLCONNECTION m_connection[static_cast<uint32_t>(DBConnectionFlag::MAX)];
};

//#define sDatabaseSystem (*DatabaseSystem::Instance())
#define sDatabaseSystem MeyersSingleton<DatabaseSystem>::Instance()
