#include "database_system.hpp"
#include "log_system.hpp"
//#include "database_connection.hpp"
//#include "database_evt.hpp"
//#include "database_recordset.hpp"
//#include "linq.hpp"
//#include "linq_condition.hpp"

DatabaseSystem::DatabaseSystem()
{
    FUNC_PERFORMANCE_CHECK();

    for (uint32_t i = static_cast<uint32_t>(DBConnectionFlag::SYNC); i < static_cast<uint32_t>(DBConnectionFlag::MAX); ++i)
    {
        m_connection[i] = nullptr;
    }
}

DatabaseSystem::~DatabaseSystem()
{
    FUNC_PERFORMANCE_CHECK();

    //for (int32_t i = 0; i < ConnectionMax; ++i)
    //{
    //    AssertNullExit(m_connection[i]);
    //}
}

void OnDatabaseError(const std::string& err_msg)
{
    FUNC_PERFORMANCE_CHECK();
    sDatabaseSystem.OnError(err_msg);
}

void OnDatabaseWarn(const std::string& err_msg)
{
    FUNC_PERFORMANCE_CHECK();
    sDatabaseSystem.OnWarn(err_msg);
}

bool DatabaseSystem::Initialize(size_t max_db_event_num)
{
    FUNC_PERFORMANCE_CHECK();

    return init_db_manager
    (
        static_cast<uint32_t>(DBConnectionFlag::MAX) - 1,
        max_db_event_num,
        OnDatabaseError,
        OnDatabaseWarn
    );
}

//bool DatabaseSystem::Connect(const DB_CONFIG& db_config)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    if (!_CreateAndUseDataBase(db_config))
//    {
//        return false;
//    }
//
//    if (!_CreateWorkConnections(db_config))
//    {
//        return false;
//    }
//
//    return true;
//}

bool DatabaseSystem::Connect(uint16_t port, const std::string& host, const std::string& user, const std::string& pwd, const std::string& database, const std::string& charset/*="utf8mb4" */)
{
    FUNC_PERFORMANCE_CHECK();

    if (!_CreateAndUseDataBase(
        port,
        host,
        user,
        pwd,
        database,
        charset
    ))
    {
        return false;
    }

    if (!_CreateWorkConnections(
        port,
        host,
        user,
        pwd,
        database,
        charset
    ))
    {
        return false;
    }

    return true;
}

void DatabaseSystem::DisConnect(void)
{
    FUNC_PERFORMANCE_CHECK();

    _DestroyWorkConnections();
}

void DatabaseSystem::Uninitialized(void)
{
    FUNC_PERFORMANCE_CHECK();

    uninit_db_manager();
}

bool DatabaseSystem::OnUpdate(CFuncPerformanceInfo** info)
{
    FUNC_PERFORMANCE_CHECK();
    *info = &s_func_perf_info;

    return db_manager_run();
}

void DatabaseSystem::OnError(const std::string& err_msg)
{
    LogERR(u8"{}", err_msg);
}

void DatabaseSystem::OnWarn(const std::string& err_msg)
{
    LogWRN(u8"{}", err_msg);
}

bool DatabaseSystem::TryStop(void)
{
    FUNC_PERFORMANCE_CHECK();
    db_manager_stop();


    return db_manager_complete_run();
}

HMYSQLCONNECTION DatabaseSystem::GetMysqlAsyncConnection(DBConnectionFlag flag)
{
    if (flag < DBConnectionFlag::MAX && flag > DBConnectionFlag::SYNC)
    {
        return m_connection[static_cast<uint32_t>(flag)];
    }

    return nullptr;
}

HMYSQLCONNECTION DatabaseSystem::GetMysqlSyncConnection(void)
{
    return m_connection[static_cast<uint32_t>(DBConnectionFlag::SYNC)];
}

//HMYSQLCONNECTION DatabaseSystem::GetMysqlConnection(DatabaseConnectionType type)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    if (type < ConnectionMax && type > ConnectionNull)
//    {
//        return m_asyncConnection[type];
//    }
//
//    return nullptr;
//}

//bool DatabaseSystem::_CreateAndUseDataBase(const DB_CONFIG& db_config)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    //const DB_CONFIG& config = sConfigSystem.GetDatabaseConfig();
//    char err_msg[512];
//
//    HCLIENTMYSQL connection = create_client_mysql(
//        db_config.Host.c_str(),
//        db_config.Port,
//        db_config.User.c_str(),
//        db_config.PWD.c_str(),
//        "",
//        db_config.Charset.c_str(),
//        err_msg,
//        sizeof(err_msg)
//    );
//
//    if (!connection)
//    {
//        LogERR("Create Mysql Connection Fail!: {}", err_msg);
//        return false;
//    }
//
//    std::string sql = fmt::format(u8"CREATE DATABASE IF NOT EXISTS `{}` DEFAULT CHARSET {};", db_config.Database, db_config.Charset);
//
//    CLIENTMYSQLRESULT res = client_mysql_query(
//        connection, sql.c_str(), static_cast<unsigned long>(sql.length()));
//
//    bool ret = client_mysql_result_success(&res);
//    if (!ret)
//    {
//        LogERR("Create DataBase {} Fail!: {}", db_config.Database, client_mysql_err(connection));
//    }
//    
//    client_mysql_free_result(&res);
//    destroy_client_mysql(connection);
//
//    return ret;
//}

bool DatabaseSystem::_CreateAndUseDataBase(uint16_t port, const std::string& host, const std::string& user, const std::string& pwd, const std::string& database, const std::string& charset /*= "utf8mb4" */)
{
    for (uint32_t i = static_cast<uint32_t>(DBConnectionFlag::SYNC); i < static_cast<uint32_t>(DBConnectionFlag::MAX); i++)
    {
        m_connection[i] = create_mysql_connection(
            i,
            host.c_str(),
            port,
            user.c_str(),
            pwd.c_str(),
            database.c_str(),
            charset.c_str()
        );

        if (!m_connection[i])
        {
            _DestroyWorkConnections();
            return false;
        }
    }

    return true;
}

//bool DatabaseSystem::_CreateWorkConnections(const DB_CONFIG& db_config)
//{
//    //const DB_CONFIG& config = sConfigSystem.GetDatabaseConfig();
//    char err_msg[512];
//    for (uint32_t i = static_cast<uint32_t>(DBConnectionFlag::SYNC); i < static_cast<uint32_t>(DBConnectionFlag::MAX); i++)
//    {
//        m_connection[i] = create_mysql_connection(
//            i,
//            db_config.Host.c_str(),
//            db_config.Port,
//            db_config.User.c_str(),
//            db_config.PWD.c_str(),
//            db_config.Database.c_str(),
//            db_config.Charset.c_str()
//        );
//
//        if (!m_connection[i])
//        {
//            LogERR("Create Mysql Connection Fail!: {}", err_msg);
//            _DestroyWorkConnections();
//            return false;
//        }
//    }
//
//    return true;
//}

bool DatabaseSystem::_CreateWorkConnections(uint16_t port, const std::string& host, const std::string& user, const std::string& pwd, const std::string& database, const std::string& charset /*= "utf8mb4" */)
{
    for (uint32_t i = static_cast<uint32_t>(DBConnectionFlag::SYNC); i < static_cast<uint32_t>(DBConnectionFlag::MAX); i++)
    {
        m_connection[i] = create_mysql_connection(
            i,
            host.c_str(),
            port,
            user.c_str(),
            pwd.c_str(),
            database.c_str(),
            charset.c_str()
        );

        if (!m_connection[i])
        {
            _DestroyWorkConnections();
            return false;
        }
    }

    return true;
}

void DatabaseSystem::_DestroyWorkConnections(void)
{
    for (uint32_t i = static_cast<uint32_t>(DBConnectionFlag::SYNC); i < static_cast<uint32_t>(DBConnectionFlag::MAX); i++)
    {
        if (m_connection[i])
        {
            destroy_mysql_connection(m_connection[i]);
            m_connection[i] = nullptr;
        }
    }
}

