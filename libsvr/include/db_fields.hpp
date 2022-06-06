#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <assert.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include "./static_string.hpp"

extern "C"
{
#include "client_mysql.h"
#include "lib_svr_def.h"
}

#include "smemory.hpp"
#include "list_value_operation.hpp"

#define MAX_ESCAPE_CACHE_SIZE   1024*128
//////////////////////////////////////////////////////////////////////////
enum class FieldDataType :int 
{
    INT8 = 0,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,
    TEXT,
    LONGTEXT,
    VARCHAR,
};

class IField
{
public:
    IField() = default;
    virtual ~IField() = default;
    virtual const std::string& GetColumnName(void) const = 0;
    virtual const std::string& GetColumnType(void) const = 0;
    virtual const std::string& GetColumnExtra(void) const = 0;
    virtual const std::string& GetColumnComment(void) const = 0;
    virtual FieldDataType GetDataType(void) const = 0;
    virtual void FromSQL(CLIENTMYSQLVALUE client_mysql_value) = 0;
    virtual std::string ToSQL(HCLIENTMYSQL client_mysql) = 0;
    virtual IField* Clone() const = 0;
    static const std::string& ColumnNull(void) { static std::string column_null = u8""; return column_null; }
protected:
private:
};

template<typename T>
class FieldINT
    :public IField
{
public:
    FieldINT() : m_data(0) {}
    FieldINT(T data) :m_data(data) {}

    FieldINT(const FieldINT<T>& rhs) :m_data(rhs.m_data) {}
    FieldINT(FieldINT<T>&& rhs) :m_data(std::move(rhs.m_data)) {}

    static constexpr auto ColumnExtra = static_string_literal(u8"NOT NULL DEFAULT '0'");


    inline T GetData(void) const { return m_data; }
    void SetData(T data)
    {
        m_data = data;
    }

    bool operator < (const FieldINT<T>& other) const
    {
        return m_data < other.m_data;
    }

    bool operator > (const FieldINT<T>& other) const
    {
        return m_data > other.m_data;
    }

    bool operator == (const FieldINT<T>& other) const
    {
        return m_data == other.m_data;
    }

    bool operator != (const FieldINT<T>& other) const
    {
        return m_data != other.m_data;
    }

    FieldINT<T>& operator=(const FieldINT<T>& rhs)
    {
        if (this != &rhs)
        {
            m_data = rhs.m_data;
        }

        return *this;
    }

    FieldINT<T>& operator=(FieldINT<T>&& rhs)
    {
        if (this != &rhs)
        {
            m_data = rhs.m_data;
        }

        return *this;
    }

    operator T ()
    {
        return m_data;
    }

    std::string ToSQL(HCLIENTMYSQL client_mysql) override
    {
        return ToSQLImpl(client_mysql);
    }

    std::string ToSQLImpl(HCLIENTMYSQL client_mysql)
    {
        (void)client_mysql;
        return fmt::format("{:d}", m_data);
    }

protected:
private:
    typename std::enable_if<std::is_integral<T>::value, T>::type m_data;
};

class FieldTXT
    :public IField
{
public:
    FieldTXT() :m_data("") {}
    FieldTXT(const std::string& data) :m_data(data) {}
    FieldTXT(const char* data) :m_data(std::string(data, strlen(data))) {}
    template<size_t N>
    FieldTXT(const char(&data)[N]) :m_data(std::string(data, strnlen(data, N))) {}

    FieldTXT(const FieldTXT& rhs) :m_data(rhs.m_data) {}
    FieldTXT(FieldTXT&& rhs) :m_data(std::move(rhs.m_data)) {}

    static constexpr auto ColumnExtra = static_string_literal(u8"NOT NULL");

    inline std::string GetData(void) const { return m_data; }
    void SetData(const std::string& data)
    {
        m_data = data;
    }

    inline const char* CStr(void) const { return m_data.c_str(); }
    inline size_t Length(void) const { return m_data.length(); }

    bool operator < (const FieldTXT& other) const
    {
        return m_data < other.m_data;
    }

    bool operator > (const FieldTXT& other) const
    {
        return m_data > other.m_data;
    }

    bool operator == (const FieldTXT& other) const
    {
        return m_data == other.m_data;
    }

    bool operator != (const FieldTXT& other) const
    {
        return m_data != other.m_data;
    }

    FieldTXT& operator=(const FieldTXT& rhs)
    {
        if (this != &rhs)
        {
            m_data = rhs.m_data;
        }

        return *this;
    }

    FieldTXT& operator=(FieldTXT&& rhs)
    {
        if (this != &rhs)
        {
            m_data = std::move(rhs.m_data);
        }

        return *this;
    }

    operator std::string ()
    {
        return m_data;
    }

    template<size_t N>
    void ToCharArray(char(&data)[N])
    {
        StrSafeCopy(data, m_data);
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(std::string(client_mysql_value.value, client_mysql_value.size));
    }

    std::string ToSQL(HCLIENTMYSQL client_mysql) override
    {
        return ToSQLImpl(client_mysql);
    }

    std::string ToSQLImpl(HCLIENTMYSQL client_mysql)
    {
        char buf[MAX_ESCAPE_CACHE_SIZE];
        char* cache = buf;
        unsigned long cache_size = MAX_ESCAPE_CACHE_SIZE;

        if ((m_data.length() * 2 + 4) > MAX_ESCAPE_CACHE_SIZE)
        {
            cache_size = static_cast<unsigned long>(m_data.length() * 2 + 4);
            cache = S_NEW(char, cache_size);
        }

        unsigned long escape_length = client_mysql_escape_string
        (
            client_mysql,
            m_data.c_str(),
            static_cast<unsigned long>(m_data.length()),
            cache + 1,
            cache_size - 3
        );

        cache[0] = '\'';
        cache[escape_length + 1] = '\'';
        cache[escape_length + 2] = '\0';

        if (cache == buf)
        {
            return std::string(cache, escape_length + 2);
        }
        else
        {
            std::string tmp(cache, escape_length + 2);
            S_DELETE(cache);
            cache = nullptr;
            return tmp;
        }
    }

protected:
private:
    std::string m_data;
};

class FieldINT8
    :public FieldINT<char>
{
public:
    FieldINT8() :FieldINT<char>(0) {}
    FieldINT8(char data) :FieldINT<char>(data) {}

    static constexpr auto ColumnType = static_string_literal(u8"tinyint(4)");

    static constexpr auto ColumnExtra = FieldINT<char>::ColumnExtra;

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(client_mysql_value_int8(client_mysql_value));
    }

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::INT8;
    }
};

class FieldUINT8
    :public FieldINT<unsigned char>
{
public:
    FieldUINT8() :FieldINT<unsigned char>(0) {}
    FieldUINT8(unsigned char data) :FieldINT<unsigned char>(data) {}

    static constexpr auto ColumnType = static_string_literal(u8"tinyint(4) unsigned");

    static constexpr auto ColumnExtra = FieldINT<unsigned char>::ColumnExtra;

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(client_mysql_value_uint8(client_mysql_value));
    }

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::UINT8;
    }
};

class FieldINT16
    :public FieldINT<short>
{
public:
    FieldINT16() :FieldINT<short>(0) {}
    FieldINT16(short data) :FieldINT<short>(data) {}

    static constexpr auto ColumnType = static_string_literal(u8"smallint(6)");

    static constexpr auto ColumnExtra = FieldINT<short>::ColumnExtra;

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(client_mysql_value_int16(client_mysql_value));
    }

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::INT16;
    }
};

class FieldUINT16
    :public FieldINT<unsigned short>
{
public:
    FieldUINT16() :FieldINT<unsigned short>(0) {}
    FieldUINT16(unsigned short data) :FieldINT<unsigned short>(data) {}

    static constexpr auto ColumnType = static_string_literal(u8"smallint(6) unsigned");

    static constexpr auto ColumnExtra = FieldINT<unsigned short>::ColumnExtra;

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(client_mysql_value_uint16(client_mysql_value));
    }

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::UINT16;
    }
};

class FieldINT32
    :public FieldINT<int>
{
public:
    FieldINT32() :FieldINT<int>(0) {}
    FieldINT32(int data) :FieldINT<int>(data) {}

    static constexpr auto ColumnType = static_string_literal(u8"int(11)");

    static constexpr auto ColumnExtra = FieldINT<int>::ColumnExtra;

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(client_mysql_value_int32(client_mysql_value));
    }

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::INT32;
    }
};

class FieldUINT32
    :public FieldINT<unsigned int>
{
public:
    FieldUINT32() :FieldINT<unsigned int>(0) {}
    FieldUINT32(unsigned int data) :FieldINT<unsigned int>(data) {}

    static constexpr auto ColumnType = static_string_literal(u8"int(11) unsigned");

    static constexpr auto ColumnExtra = FieldINT<unsigned int>::ColumnExtra;

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(client_mysql_value_uint32(client_mysql_value));
    }

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::UINT32;
    }
};

class FieldINT64
    :public FieldINT<long long>
{
public:
    FieldINT64() :FieldINT<long long>(0) {}
    FieldINT64(long long data) :FieldINT<long long>(data) {}

    static constexpr auto ColumnType = static_string_literal(u8"bigint(20)");

    static constexpr auto ColumnExtra = FieldINT<long long>::ColumnExtra;

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(client_mysql_value_int64(client_mysql_value));
    }

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::INT64;
    }
};

class FieldUINT64
    :public FieldINT<unsigned long long>
{
public:
    FieldUINT64() :FieldINT<unsigned long long>(0) {}
    FieldUINT64(unsigned long long data) :FieldINT<unsigned long long>(data) {}

    static constexpr auto ColumnType = static_string_literal(u8"bigint(20) unsigned");

    static constexpr auto ColumnExtra = FieldINT<unsigned long long>::ColumnExtra;

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
    {
        FromSQLImpl(client_mysql_value);
    }

    void FromSQLImpl(CLIENTMYSQLVALUE client_mysql_value)
    {
        SetData(client_mysql_value_uint64(client_mysql_value));
    }

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::UINT64;
    }
};

class FieldText
    :public FieldTXT
{
public:
    FieldText() :FieldTXT("") {}
    FieldText(const std::string& data) :FieldTXT(data) {}
    FieldText(const char* data) :FieldTXT(data) {}
    template<size_t N>
    FieldText(const char(&data)[N]) : FieldTXT(data) {};

    static constexpr auto ColumnType = static_string_literal(u8"text");

    static constexpr auto ColumnExtra = FieldTXT::ColumnExtra;

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::TEXT;
    }
};

class FieldLongText
    :public FieldTXT
{
public:
    FieldLongText() :FieldTXT(""){}
    FieldLongText(const std::string& data) :FieldTXT(data) {}
    FieldLongText(const char* data) :FieldTXT(data) {}
    template<size_t N>
    FieldLongText(const char(&data)[N]) : FieldTXT(data) {};

    static constexpr auto ColumnType = static_string_literal(u8"longtext");

    static constexpr auto ColumnExtra = FieldTXT::ColumnExtra;

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::LONGTEXT;
    }
};


template <size_t N>
class FieldVarChar
    :public FieldTXT
{
public:
    FieldVarChar() :FieldTXT(""){}
    FieldVarChar(const std::string& data) :FieldTXT(data) {}
    FieldVarChar(const char* data) :FieldTXT(data) {}
    template<size_t M>
    FieldVarChar(const char(&data)[M]) : FieldTXT(data) {}

    static constexpr auto ColumnType = static_string_concat(static_string_literal(u8"varchar("), static_string_concat(static_integer_to_string<N>(), static_string_literal(u8")")));

    static constexpr auto ColumnExtra = static_string_literal(u8"COLLATE utf8mb4_bin NOT NULL");

    FieldDataType GetDataType(void) const override
    {
        return FieldDataType::VARCHAR;
    }
};

//////////////////////////////////////////////////////////////////////////

template <typename... Args>
struct SFieldList {};

template<>
struct SFieldList<>
{
    SFieldList():m_custom_sql(u8""){}
    SFieldList(const std::string& custom_sql):m_custom_sql(custom_sql){}
    virtual ~SFieldList(){}

    SFieldList(const SFieldList<>& rhs)
    {
        m_custom_sql = rhs.m_custom_sql;
    }

    SFieldList(SFieldList<>&& rhs)
    {
        m_custom_sql = std::move(rhs.m_custom_sql);
    }

    SFieldList<>& operator = (const SFieldList<>& rhs)
    {
        if (this != &rhs)
        {
            m_custom_sql = rhs.m_custom_sql;
        }
        
        return *this;
    }

    SFieldList<>& operator = (SFieldList<>&& rhs)
    {
        if (this != &rhs)
        {
            m_custom_sql = std::move(rhs.m_custom_sql);
        }

        return *this;
    }

    bool operator == (const SFieldList<>& rhs) const
    {
        return m_custom_sql == rhs.m_custom_sql;
    }

    bool operator != (const SFieldList<>& rhs) const
    {
        return m_custom_sql != rhs.m_custom_sql;
    }

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4100 )
#elif __GNUC__

#else
#error "unknown compiler"
#endif

    inline std::string& CustomSQL(void)
    {
        return m_custom_sql;
    }

    ptrdiff_t Compare(const SFieldList<>& other) const
    {
        if (m_custom_sql < other.m_custom_sql)
        {
            return -1;
        }
        else if (m_custom_sql > other.m_custom_sql)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    static constexpr auto ColumnNameList()
    {
        return static_string_literal("");
    }

    static constexpr auto ColumnNameListNext()
    {
        return static_string_literal("");
    }

    virtual std::string ListSQL()
    {
        return "";
    }

    virtual std::string ListNameSQL()
    {
        return "";
    }

    virtual std::string SetSQL(HCLIENTMYSQL& client_mysql)
    {
        return SetSQLImpl(client_mysql);
    }

    std::string SetSQLImpl(HCLIENTMYSQL& client_mysql)
    {
        (void)client_mysql;
        return "";
    }

    virtual std::string AndSQL(HCLIENTMYSQL& client_mysql)
    {
        return AndSQLImpl(client_mysql);
    }

    std::string AndSQLImpl(HCLIENTMYSQL& client_mysql)
    {
        (void)client_mysql;
        return "";
    }

    virtual std::string OrSQL(HCLIENTMYSQL& client_mysql)
    {
        return OrSQLImpl(client_mysql);
    }

    std::string OrSQLImpl(HCLIENTMYSQL& client_mysql)
    {
        (void)client_mysql;
        return "";
    }

    virtual std::string ValueSQL(HCLIENTMYSQL& client_mysql)
    {
        return ValueSQLImpl(client_mysql);
    }

    std::string ValueSQLImpl(HCLIENTMYSQL& client_mysql)
    {
        (void)client_mysql;
        return "";
    }

    virtual std::string CreateSQL(void)
    {
        return "";
    }

    static constexpr auto CreateSQLImpl(void)
    {
        return static_string_literal(u8"");
    }

    virtual void GetDesc(std::vector<std::string>& name_type_list)
    {
        (void)name_type_list;
    }

    virtual size_t size() const
    {
        return 0;
    }

    virtual void LoadData(const CLIENTMYSQLROW& row, unsigned long idx = 0)
    {
        (void)row;
        (void)idx;
    }

private:
    std::string m_custom_sql;

#ifdef _MSC_VER
#pragma warning( pop )
#elif __GNUC__

#else
#error "unknown compiler"
#endif
};

//std::map<const char*, IField*> m_dynamic_fields;

struct SDynamicFieldList
    :public SFieldList<>
{
    SDynamicFieldList() = default;

    SDynamicFieldList(const SDynamicFieldList& rhs)
    {
        m_dynamic_fields = rhs.m_dynamic_fields;

        for (auto& it:m_dynamic_fields)
        {
            it.second = it.second->Clone();
        }
    }

    ~SDynamicFieldList()
    {
        Clear();
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<FieldUINT8, T>::value
        || std::is_base_of<FieldINT8, T>::value
        || std::is_base_of<FieldUINT16, T>::value
        || std::is_base_of<FieldINT16, T>::value
        || std::is_base_of<FieldUINT32, T>::value
        || std::is_base_of<FieldINT32, T>::value
        || std::is_base_of<FieldUINT64, T>::value
        || std::is_base_of<FieldINT64, T>::value
        || std::is_base_of<FieldTXT, T>::value, bool>::type AddField(const T& field)
    {
        IField* new_field = S_NEW(T, 1, field);

        auto ret = m_dynamic_fields.insert(std::make_pair(T::ColumnName.s, new_field));

        if (!ret.second)
        {
            S_DELETE(new_field);
        }

        return ret.second;
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<FieldUINT8, T>::value
        || std::is_base_of<FieldINT8, T>::value
        || std::is_base_of<FieldUINT16, T>::value
        || std::is_base_of<FieldINT16, T>::value
        || std::is_base_of<FieldUINT32, T>::value
        || std::is_base_of<FieldINT32, T>::value
        || std::is_base_of<FieldUINT64, T>::value
        || std::is_base_of<FieldINT64, T>::value
        || std::is_base_of<FieldTXT, T>::value, void>::type UpdField(const T& field)
    {
        auto ret = m_dynamic_fields.insert(std::make_pair(T::ColumnName.s, nullptr));

        if (ret.second)
        {
            ret.first->second = S_NEW(T, 1, field);
        }
        else
        {
            T* exist_field = dynamic_cast<T*>(ret.first->second);
            exist_field->SetData(field.GetData());
        }
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<FieldUINT8, T>::value
        || std::is_base_of<FieldINT8, T>::value
        || std::is_base_of<FieldUINT16, T>::value
        || std::is_base_of<FieldINT16, T>::value
        || std::is_base_of<FieldUINT32, T>::value
        || std::is_base_of<FieldINT32, T>::value
        || std::is_base_of<FieldUINT64, T>::value
        || std::is_base_of<FieldINT64, T>::value
        || std::is_base_of<FieldTXT, T>::value>::type DelField()
    {
        auto it = m_dynamic_fields.find(T::ColumnName.s);

        if (it != m_dynamic_fields.end())
        {
            S_DELETE(it->second);
            m_dynamic_fields.erase(it);
        }
    }

    void Clear(void)
    {
        for (auto& it : m_dynamic_fields)
        {
            S_DELETE(it.second);
        }

        m_dynamic_fields.clear();
    }

    std::string ListSQL(void) override
    {
        std::string sql;
        bool is_first = true;


        for (auto it:m_dynamic_fields)
        {
            if (is_first)
            {
                sql += u8" ";
                is_first = false;
            }
            else
            {
                sql += u8", ";
            }

            sql += u8"`" + it.second->GetColumnName() + u8"`";
        }

        return sql;
    }

    std::string ListNameSQL(void) override
    {
        std::string sql;
        bool is_first = true;

        for (auto it : m_dynamic_fields)
        {
            if (is_first)
            {
                sql += u8" ";
                is_first = false;
            }
            else
            {
                sql += u8", ";
            }

            sql += u8"`" + it.second->GetColumnName() + u8"`";
        }

        return sql;
    }

    std::string SetSQL(HCLIENTMYSQL& client_mysql) override
    {
        std::string sql;
        bool is_first = true;

        for (auto it : m_dynamic_fields)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                sql += u8", ";
            }

            sql += u8"`" + it.second->GetColumnName() + u8"`";
            sql += u8" = ";
            sql += it.second->ToSQL(client_mysql);
        }

        return sql;
    }

    std::string AndSQL(HCLIENTMYSQL& client_mysql) override
    {
        std::string sql;
        bool is_first = true;

        for (auto it : m_dynamic_fields)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                sql += u8" AND ";
            }

            sql += u8"`" + it.second->GetColumnName() + u8"`";
            sql += " = ";
            sql += it.second->ToSQL(client_mysql);
        }

        return sql;
    }

    std::string OrSQL(HCLIENTMYSQL& client_mysql) override
    {
        std::string sql;
        bool is_first = true;

        for (auto it : m_dynamic_fields)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                sql += u8" OR ";
            }

            sql += u8"`" + it.second->GetColumnName() + u8"`";
            sql += " = ";
            sql += it.second->ToSQL(client_mysql);
        }

        return sql;
    }

    std::string ValueSQL(HCLIENTMYSQL& client_mysql) override
    {
        std::string sql;
        bool is_first = true;

        for (auto it : m_dynamic_fields)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                sql += u8", ";
            }

            sql += it.second->ToSQL(client_mysql);
        }

        return sql;
    }

    std::string CreateSQL(void) override
    {
        std::string sql;

        for (auto it : m_dynamic_fields)
        {
            sql +=
                u8"`" +
                it.second->GetColumnName() +
                u8"` " +
                it.second->GetColumnType() +
                u8" " +
                it.second->GetColumnExtra() +
                u8" ";
            if (it.second->GetColumnComment().length())
            {
                sql += u8"COMMENT \"";
                sql += it.second->GetColumnComment();
                sql += u8"\",";
            }
            else
            {
                sql += u8",";
            }
        }

        return sql;
    }

    void GetDesc(std::vector<std::string>& name_type_list) override
    {
        for (auto it : m_dynamic_fields)
        {
            IField* field = it.second;

            name_type_list.push_back(field->GetColumnName());
            name_type_list.push_back(field->GetColumnType());
            name_type_list.push_back(field->GetColumnExtra());
        }
    }

    virtual size_t size() const override
    {
        return m_dynamic_fields.size();
    }

    void LoadData(const CLIENTMYSQLROW& row, unsigned long idx = 0) override
    {
        for (auto it : m_dynamic_fields)
        {
            it.second->FromSQL(client_mysql_value(row, idx));
            ++idx;
        }
    }

    bool operator == (const SDynamicFieldList& rhs) const
    {
        if (m_dynamic_fields.size() != rhs.m_dynamic_fields.size())
        {
            return false;
        }

        auto it_rhs = m_dynamic_fields.begin();

        for (auto it : m_dynamic_fields)
        {
            IField* field = it.second;
            IField* rhs_field = it_rhs->second;

            if (field->GetColumnName().c_str() !=
                rhs_field->GetColumnName().c_str())
            {
                return false;
            }

            if (field->GetDataType() != rhs_field->GetDataType())
            {
                return false;
            }

            switch (field->GetDataType())
            {
            case FieldDataType::INT8:
            {
                FieldINT8* field_real = static_cast<FieldINT8*>(field);
                FieldINT8* rhs_field_real = static_cast<FieldINT8*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            case FieldDataType::UINT8:
            {
                FieldUINT8* field_real = static_cast<FieldUINT8*>(field);
                FieldUINT8* rhs_field_real = static_cast<FieldUINT8*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            case FieldDataType::INT16:
            {
                FieldINT16* field_real = static_cast<FieldINT16*>(field);
                FieldINT16* rhs_field_real = static_cast<FieldINT16*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            case FieldDataType::UINT16:
            {
                FieldUINT16* field_real = static_cast<FieldUINT16*>(field);
                FieldUINT16* rhs_field_real = static_cast<FieldUINT16*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            case FieldDataType::INT32:
            {
                FieldINT32* field_real = static_cast<FieldINT32*>(field);
                FieldINT32* rhs_field_real = static_cast<FieldINT32*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            case FieldDataType::UINT32:
            {
                FieldUINT32* field_real = static_cast<FieldUINT32*>(field);
                FieldUINT32* rhs_field_real = static_cast<FieldUINT32*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            case FieldDataType::INT64:
            {
                FieldINT64* field_real = static_cast<FieldINT64*>(field);
                FieldINT64* rhs_field_real = static_cast<FieldINT64*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            case FieldDataType::UINT64:
            {
                FieldUINT64* field_real = static_cast<FieldUINT64*>(field);
                FieldUINT64* rhs_field_real = static_cast<FieldUINT64*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            case FieldDataType::TEXT:
            case FieldDataType::LONGTEXT:
            case FieldDataType::VARCHAR:
            {
                FieldTXT* field_real = static_cast<FieldTXT*>(field);
                FieldTXT* rhs_field_real = static_cast<FieldTXT*>(rhs_field);

                if ((*field_real) != (*rhs_field_real))
                {
                    return false;
                }
            }
            break;
            default:
                return false;
            }

            ++it_rhs;
        }

        return true;
    }

    bool operator != (const SDynamicFieldList& rhs) const
    {
        return !(*this == rhs);
    }

    //HRBTREE m_dyna_fields;
    std::map<const char*, IField*> m_dynamic_fields;
};

template <typename First, typename... Rest>
struct SFieldList<First, Rest...>
    :public SFieldList<Rest...>
{
    SFieldList(){}
    SFieldList(First&& first, Rest&&... rest)
        :SFieldList<Rest...>(std::forward<Rest>(rest)...),
        value(std::forward<First>(first)) {}
    virtual ~SFieldList(){}

    SFieldList(const SFieldList<First, Rest...>& rhs)
        :SFieldList<Rest...>(static_cast<const SFieldList<Rest...>&>(rhs))
    {
        value = rhs.value;
    }

    SFieldList(SFieldList<First, Rest...>&& rhs)
        :SFieldList<Rest...>(static_cast<SFieldList<Rest...>&&>(rhs))
    {
        value = std::move(rhs.value);
    }

    SFieldList<First, Rest...>& operator = (const SFieldList<First, Rest...>& rhs)
    {
        if (this != &rhs)
        {
            value = rhs.value;

            static_cast<SFieldList<Rest...>&>(*this) = static_cast<const SFieldList<Rest...>&>(rhs);
        }

        return *this;
    }

    SFieldList<First, Rest...>& operator = (SFieldList<First, Rest...>&& rhs)
    {
        if (this != &rhs)
        {
            value = std::move(rhs.value);

            static_cast<SFieldList<Rest...>&>(*this) = std::move(static_cast<SFieldList<Rest...>&>(rhs));
        }

        return *this;
    }

    bool operator == (const SFieldList<First, Rest...>& other) const
    {
        if (value == other.value)
        {
            return static_cast<const SFieldList<Rest...>&>(*this) == static_cast<const SFieldList<Rest...>&>(other);
        }
        else
        {
            return false;
        }
    }

    bool operator != (const SFieldList<First, Rest...>& other) const
    {
        return !(*this == other);
    }

    ptrdiff_t Compare(const SFieldList<First, Rest...>& other) const
    {
        if (value < other.value)
        {
            return -1;
        }
        else if (value > other.value)
        {
            return 1;
        }
        else
        {
            return SFieldList<Rest...>::Compare(static_cast<const SFieldList<Rest...>&>(other));
        }
    }

    static constexpr auto ColumnNameList()
    {
        return
            static_string_concat(
                static_string_literal(u8"`"),
                static_string_concat(
                    First::ColumnName,
                    static_string_concat(
                        static_string_literal(u8"`"),
                        SFieldList<Rest...>::ColumnNameListNext()
                    )
                )
            );
    }

    static constexpr auto ColumnNameListNext()
    {
        return
            static_string_concat(
                static_string_literal(u8", `"),
                static_string_concat(
                    First::ColumnName,
                    static_string_concat(
                        static_string_literal(u8"`"),
                        SFieldList<Rest...>::ColumnNameListNext()
                    )
                )
            );
    }

    //template<bool is_first, typename Dummy=int>
    //static constexpr auto ColumnNameList()
    //{
    //    return static_string_literal("");
    //}

    //template<typename Dummy = int>
    //static constexpr auto ColumnNameList<true, Dummy>()
    //{
    //    return
    //        static_string_concat(
    //            static_string_literal(u8"`"),
    //            static_string_concat(
    //                First::ColumnName,
    //                static_string_concat(
    //                    static_string_literal(u8"`"),
    //                    SFieldList<Rest...>::ColumnNameList<false, Dummy>()
    //                )
    //            )
    //        );
    //}

    //template<typename Dummy = int>
    //static constexpr auto ColumnNameList<false, Dummy>()
    //{
    //    return
    //        static_string_concat(
    //            static_string_literal(u8", `"),
    //            static_string_concat(
    //                First::ColumnName,
    //                static_string_concat(
    //                    static_string_literal(u8"`"),
    //                    SFieldList<Rest...>::ColumnNameList<false, Dummy>()
    //                )
    //            )
    //        );
    //}

    std::string ListSQL() override
    {
        return ColumnNameList().s;
    }

    std::string ListNameSQL() override
    {
        return ColumnNameList().s;
    }

    std::string SetSQL(HCLIENTMYSQL& client_mysql) override
    {
        std::string sql;

        sql += static_string_concat(
            static_string_literal(u8"`"),
            static_string_concat(
                First::ColumnName,
                static_string_literal(u8"` = ")
            )
        ).s;

        sql += value.ToSQLImpl(client_mysql);

        return sql += SFieldList<Rest...>::SetSQLImpl(client_mysql);
    }

    std::string SetSQLImpl(HCLIENTMYSQL& client_mysql)
    {
        std::string sql;
        sql += u8", ";

        sql += static_string_concat(
            static_string_literal(u8"`"),
            static_string_concat(
                First::ColumnName,
                static_string_literal(u8"` = ")
            )
        ).s;

        sql += value.ToSQLImpl(client_mysql);

        return sql += SFieldList<Rest...>::SetSQLImpl(client_mysql);
    }

    std::string AndSQL(HCLIENTMYSQL& client_mysql) override
    {
        std::string sql;

        sql += static_string_concat(
            static_string_literal(u8"`"),
            static_string_concat(
                First::ColumnName,
                static_string_literal(u8"` = ")
            )
        ).s;
        sql += value.ToSQLImpl(client_mysql);

        return sql += SFieldList<Rest...>::AndSQLImpl(client_mysql);
    }

    std::string AndSQLImpl(HCLIENTMYSQL& client_mysql)
    {
        std::string sql;

        sql += u8" AND ";

        sql += static_string_concat(
            static_string_literal(u8"`"),
            static_string_concat(
                First::ColumnName,
                static_string_literal(u8"` = ")
            )
        ).s;
        sql += value.ToSQLImpl(client_mysql);

        return sql += SFieldList<Rest...>::AndSQLImpl(client_mysql);
    }

    std::string OrSQL(HCLIENTMYSQL& client_mysql) override
    {
        std::string sql;

        sql += static_string_concat(
            static_string_literal(u8"`"),
            static_string_concat(
                First::ColumnName,
                static_string_literal(u8"` = ")
            )
        ).s;
        sql += value.ToSQLImpl(client_mysql);

        return sql += SFieldList<Rest...>::OrSQLImpl(client_mysql);
    }

    std::string OrSQLImpl(HCLIENTMYSQL& client_mysql)
    {
        std::string sql;

        sql += u8" OR ";

        sql += static_string_concat(
            static_string_literal(u8"`"),
            static_string_concat(
                First::ColumnName,
                static_string_literal(u8"` = ")
            )
        ).s;
        sql += value.ToSQLImpl(client_mysql);

        return sql += SFieldList<Rest...>::OrSQLImpl(client_mysql);
    }

    std::string ValueSQL(HCLIENTMYSQL& client_mysql) override
    {
        std::string sql;

        sql += value.ToSQLImpl(client_mysql);

        return sql += SFieldList<Rest...>::ValueSQLImpl(client_mysql);
    }

    std::string ValueSQLImpl(HCLIENTMYSQL& client_mysql)
    {
        std::string sql;

        sql += u8", ";

        sql += value.ToSQLImpl(client_mysql);

        return sql += SFieldList<Rest...>::ValueSQLImpl(client_mysql);
    }

    static constexpr auto CreateSQLImpl(void)
    {
        return static_string_concat(
            static_string_concat(
                static_string_literal(u8"`"),
                static_string_concat(
                    First::ColumnName,
                    static_string_concat(
                        static_string_literal(u8"` "),
                        static_string_concat(
                            First::ColumnType,
                            static_string_concat(
                                static_string_literal(u8" "),
                                static_string_concat(
                                    First::ColumnExtra,
                                    static_string_concat(
                                        static_string_literal(u8" COMMENT \""),
                                        static_string_concat(
                                            First::ColumnComment,
                                            static_string_literal(u8"\",")
                                        )
                                    )
                                )
                            )
                        )
                    )
                )
            )
            , SFieldList<Rest...>::CreateSQLImpl());
    }

    std::string CreateSQL(void) override
    {
        return CreateSQLImpl().s;
    }

    void GetDesc(std::vector<std::string>& name_type_list) override
    {
        name_type_list.push_back(First::ColumnName.s);
        name_type_list.push_back(First::ColumnType.s);
        name_type_list.push_back(First::ColumnExtra.s);

        SFieldList<Rest...>::GetDesc(name_type_list);
    }

    virtual size_t size() const override
    {
        return sizeof...(Rest) + 1;
    }

    void LoadData(const CLIENTMYSQLROW& row, unsigned long idx = 0) override
    {
        value.FromSQL(client_mysql_value(row, idx));
        SFieldList<Rest...>::LoadData(row, idx + 1);
    }

    template <typename F>
    F& Field() const
    {
        return ListGetValueByType<F>(*this);
    }

    template<typename F>
    void Modify(const F& field, SDynamicFieldList* modify_field_list = nullptr)
    {
        if (field.GetData() != this-> template Field<F>().GetData())
        {
            this->template Field<F>().SetData(field.GetData());
            if (modify_field_list)
            {
                modify_field_list->UpdField(field);
            }
        }
    }

    template<typename... F>
    void ModifyList(const SFieldList<F...>& fields, SDynamicFieldList* modify_field_list = nullptr)
    {
        int arr[] = { (Modify(fields.template Field<F>(), modify_field_list), 0)... };
        (void)(arr);
    }

    template<typename... F>
    void ModifyEx(SDynamicFieldList* modify_field_list, F&&... fields)
    {
        int arr[] = { (Modify(fields, modify_field_list), 0)... };
        (void)(arr);
    }

    typename std::enable_if<
        std::is_base_of<FieldUINT8, First>::value
        || std::is_base_of<FieldINT8, First>::value
        || std::is_base_of<FieldUINT16, First>::value
        || std::is_base_of<FieldINT16, First>::value
        || std::is_base_of<FieldUINT32, First>::value
        || std::is_base_of<FieldINT32, First>::value
        || std::is_base_of<FieldUINT64, First>::value
        || std::is_base_of<FieldINT64, First>::value
        || std::is_base_of<FieldTXT, First>::value
        , First>::type value;
};

struct SRecordFieldList
{
    SRecordFieldList()
    {
        m_name_idx.clear();
        m_data.clear();
    }

    ~SRecordFieldList()
    {
    }

    void Init(CLIENTMYSQLRESULT& res)
    {
        unsigned int field_num = mysql_num_fields(res.record_set);
        MYSQL_FIELD* field = mysql_fetch_fields(res.record_set);

        for (unsigned int i = 0; i < field_num; i++)
        {
            m_name_idx[field[i].name] = i;
        }
        m_data.resize(field_num);
    }

    std::string Field(unsigned int idx)
    {
        if (idx >= m_data.size())
        {
            assert(false);
            return "";
        }

        return m_data[idx];
    }

    std::string Field(const std::string& name)
    {
        auto it = m_name_idx.find(name);
        if (it == m_name_idx.end())
        {
            assert(false);
            return "";
        }

        return Field(it->second);
    }

    void LoadData(const CLIENTMYSQLROW& row)
    {
        for (size_t i = 0; i < m_data.size(); i++)
        {
            CLIENTMYSQLVALUE data = client_mysql_value(row, (unsigned long)i);
            m_data[i] = std::string(data.value, data.size);
        }
    }

    std::map<std::string, unsigned int> m_name_idx;
    std::vector<std::string> m_data;
};

//////////////////////////////////////////////////////////////////////////

template <typename T>
ptrdiff_t FieldListCompare(const void* key1, const void* key2)
{
    const T* d1 = static_cast<const T*>(key1);
    const T* d2 = static_cast<const T*>(key2);

    return d1->Compare(*d2);
}


//////////////////////////////////////////////////////////////////////////
class IResult
{
public:
    IResult():m_err_msg(""),m_wrn_msg(""){}
    virtual ~IResult() {}
    virtual void OnCall(void) = 0;
    virtual void OnResult(CLIENTMYSQLRESULT& res) = 0;
    inline void SetError(const std::string& err_msg) { m_err_msg = err_msg; }
    inline const std::string& GetError(void) { return m_err_msg; }
    inline void SetWarn(const std::string& wrn_msg) { m_wrn_msg = wrn_msg; }
    inline const std::string& GetWarn(void) { return m_wrn_msg; }
protected:
    std::string m_err_msg;
    std::string m_wrn_msg;
};
template <typename... Fields>
class RecordResult
    :public IResult
{
public:
    RecordResult(const std::function<void(bool, const std::vector<SFieldList<Fields...>>&)>& func)
        :m_func(func)
    {
        m_datas.clear();
    }

    void OnCall(void) override
    {
        if (m_func)
        {
            m_func(m_err_msg.empty(), m_datas);
        }
    }

    void OnResult(CLIENTMYSQLRESULT& res) override
    {
        m_err_msg = "";

        m_datas.resize(client_mysql_rows_num(&res));

        for (auto& field : m_datas)
        {
            field.LoadData(client_mysql_fetch_row(&res));
        }
    }

    std::vector<SFieldList<Fields...>> MoveData(void)
    {
        return std::move(m_datas);
    }

protected:
private:
    std::vector<SFieldList<Fields...>> m_datas;
    std::function<void(bool, std::vector<SFieldList<Fields...>>&)> m_func;
};

template<typename... Args>
std::vector<SFieldList<Args...>> Record2FieldList(CLIENTMYSQLRESULT& res)
{
    std::vector<SFieldList<Args...>> datas;
    datas.resize(client_mysql_rows_num(&res));

    for (auto& field : datas)
    {
        field.LoadData(client_mysql_fetch_row(&res));
    }

    return std::move(datas);
}

extern std::vector<SRecordFieldList> Record2FieldList(CLIENTMYSQLRESULT& res);

class AffectResult
    :public IResult
{
public:
    AffectResult(const std::function<void(bool, unsigned long long affect_row)>& func)
        :m_affect_row(0), m_func(func){}

    void OnCall(void) override
    {
        if (m_func)
        {
            m_func(m_err_msg.empty(), m_affect_row);
        }
    }

    void OnResult(CLIENTMYSQLRESULT& res) override
    {
        m_affect_row = client_mysql_result_affected(&res);
    }

    unsigned long long GetAffectRow(void)
    {
        return m_affect_row;
    }

protected:
private:
    unsigned long long m_affect_row;
    std::function<void(bool, unsigned long long)> m_func;
};

//////////////////////////////////////////////////////////////////////////
