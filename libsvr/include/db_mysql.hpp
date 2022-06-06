#pragma once
#include "./db_fields.hpp"

class IDBTable;

enum class MysqlOperation : unsigned int
{
    Select = 0,
    Insert,
    Update,
    Delete,
    Duplicate,
};

using OnError = void(*)(const std::string& err_msg);
using OnWarn = void(*)(const std::string& err_msg);

extern bool (init_db_manager)(size_t db_thread_num, size_t max_db_event_num, OnError on_error, OnWarn on_warn);

extern bool (db_manager_run)(unsigned int run_time = 0);

extern void (uninit_db_manager)(void);

extern void (db_manager_stop)(void);

extern bool (db_manager_is_run)(void);

extern bool (db_manager_complete_run)(void);

extern HMYSQLCONNECTION (create_mysql_connection)(
    size_t db_thread_idx,
    const char* host,
    unsigned int port,
    const char* user,
    const char* passwd,
    const char* db,
    const char* charact_set="utf8mb4");

extern void (destroy_mysql_connection)(HMYSQLCONNECTION connection);

extern HCLIENTMYSQL (db_connection_to_client_mysql)(HMYSQLCONNECTION connection);


extern bool db_manager_post_default_cmd(const IDBTable* table, MysqlOperation operation, SFieldList<>* fields, SFieldList<>* conditions, IResult* result, unsigned int try_count = 5);
extern bool db_manager_do_default_cmd(const IDBTable* table, MysqlOperation operation, SFieldList<>* fields, SFieldList<>* conditions, IResult* result);

class DBCommand;

extern bool db_manager_post_custom_cmd(HMYSQLCONNECTION connection, DBCommand* cmd);

class DBCommand
{
public:
    virtual void OnExecute(HCLIENTMYSQL client_mysql) = 0;
    virtual void OnResult(void) = 0;

    template<typename T, typename... Args>
    static bool PostCommand(HMYSQLCONNECTION connection, Args&&... args)
    {
        DBCommand* custom_command = S_NEW(T, 1, std::forward<Args>(args)...);
        if (db_manager_post_custom_cmd(connection, custom_command))
        {
            return true;
        }
        else
        {
            S_DELETE(custom_command);
            return false;
        }
    }

    static void Release(DBCommand* command)
    {
        S_DELETE(command);
    }
protected:
private:
    DBCommand() = default;
    virtual ~DBCommand() = default;
};

extern std::string (db_check_fields)(IDBTable* table);

extern std::string (db_check_index)(IDBTable* table);

extern std::string (db_check_table)(IDBTable* table);

extern bool (db_has_field)(IDBTable* table, const std::string& field_name, std::string& err);
