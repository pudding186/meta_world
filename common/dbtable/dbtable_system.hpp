#pragma once
#include "net_data.hpp"
#include "db_table.hpp"
#include "list_value_operation.hpp"

template<typename... DT>
struct DatabaseTableList {};

template<>
struct DatabaseTableList<> 
{
    std::string CheckTables(void)
    {
        return u8"";
    }

    std::string CheckFields(void)
    {
        return u8"";
    }

    std::string CheckIndexs(void)
    {
        return u8"";
    }
};


template <typename First, typename... Rest>
struct DatabaseTableList<First, Rest...>
    :public DatabaseTableList<Rest...>
{

    std::string CheckTables(void)
    {
        std::string ret = db_check_table(&value);
        if (ret.length())
        {
            return ret;
        }

        return DatabaseTableList<Rest...>::CheckTables();
    }

    std::string CheckFields(void)
    {
        std::string ret = db_check_fields(&value);
        if (ret.length())
        {
            return ret;
        }

        return DatabaseTableList<Rest...>::CheckFields();
    }

    std::string CheckIndexs(void)
    {
        std::string ret = db_check_index(&value);
        if (ret.length())
        {
            return ret;
        }

        return DatabaseTableList<Rest...>::CheckIndexs();
    }

    const static size_t SizeOf = (sizeof...(Rest) + 1);
    typename std::enable_if<
        std::is_base_of<IDBTable, First>::value, First>::type value;
};


template<typename... DTL>
class DatabaseTableSystem
{
public:
    DatabaseTableSystem() = default;
    ~DatabaseTableSystem() = default;

    std::string CheckTables(void)
    {
        return m_database_table_list.CheckTables();
    }

    std::string CheckFields(void)
    {
        return m_database_table_list.CheckFields();
    }

    std::string CheckIndexs(void)
    {
        return m_database_table_list.CheckIndexs();
    }

    template<typename DT>
    DT& GetTable(void)
    {
        return ListGetValueByType<DT>(m_database_table_list);
    }

    template<typename... DT>
    void SetMysqlConnection(HMYSQLCONNECTION connection)
    {
        int array[] = {(GetTable<DT>().SetConnection(connection), 0)...};
        (void)array;
    }

protected:


private:
    DatabaseTableList<DTL...> m_database_table_list;
};

