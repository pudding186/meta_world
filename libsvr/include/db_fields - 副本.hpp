#pragma once

#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include "./rb_tree.h"

extern "C"
{
#include "client_mysql.h"
#include "lib_svr_def.h"
}

#include "smemory.hpp"
#include "list_value_get.hpp"

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

    static const std::string& ColumnExtra(void)
    {
        static std::string column_attr = u8"NOT NULL DEFAULT '0'";
        return column_attr;
    }

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

    static const std::string& ColumnExtra(void)
    {
        static std::string column_attr = u8"NOT NULL";
        return column_attr;
    }

    inline std::string GetData(void) const { return m_data; }
    void SetData(const std::string& data)
    {
        m_data = data;
    }

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
        SetData(std::string(client_mysql_value.value, client_mysql_value.size));
    }

    std::string ToSQL(HCLIENTMYSQL client_mysql) override
    {
        char buf[MAX_ESCAPE_CACHE_SIZE];
        char* cache = buf;
        unsigned long cache_size = MAX_ESCAPE_CACHE_SIZE;

        if ((m_data.length() * 2 + 1) > MAX_ESCAPE_CACHE_SIZE)
        {
            cache_size = static_cast<unsigned long>(m_data.length() * 2 + 2);
            cache = S_NEW(char, cache_size);
        }

        unsigned long escape_length = client_mysql_escape_string
        (
            client_mysql,
            m_data.c_str(),
            static_cast<unsigned long>(m_data.length()),
            cache,
            cache_size
            );

        //if (escape_length == (unsigned long)-1)
        //{
        //    if (cache != buf)
        //    {
        //        S_DELETE(cache);
        //        cache = nullptr;
        //    }

        //    return "";
        //}
        //else
        //{
        //    std::string escape_str = "'" + std::string(cache, escape_length) + "'";

        //    if (cache != buf)
        //    {
        //        S_DELETE(cache);
        //        cache = nullptr;
        //    }

        //    return escape_str;
        //}
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
    static const std::string& ColumnType(void)
    { 
        static std::string column_type = u8"tinyint(4)"; 
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldINT<char>::ColumnExtra();
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
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

    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"tinyint(4) unsigned";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldINT<unsigned char>::ColumnExtra();
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
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
    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"smallint(6)";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldINT<short>::ColumnExtra();
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
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
    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"smallint(6) unsigned";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldINT<unsigned short>::ColumnExtra();
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
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
    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"int(11)";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldINT<int>::ColumnExtra();
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
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
    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"int(11) unsigned";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldINT<unsigned int>::ColumnExtra();
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
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
    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"bigint(20)";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldINT<long long>::ColumnExtra();
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
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
    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"bigint(20) unsigned";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldINT<unsigned long long>::ColumnExtra();
    }

    void FromSQL(CLIENTMYSQLVALUE client_mysql_value) override
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
    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"text";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldTXT::ColumnExtra();
    }

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
    static const std::string& ColumnType(void)
    {
        static std::string column_type = u8"longtext";
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        return FieldTXT::ColumnExtra();
    }

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
    static const std::string& ColumnType(void)
    {
        static std::string column_type =
            fmt::format(u8"varchar({})", N);
        return column_type;
    }

    static const std::string& ColumnExtra(void)
    {
        static std::string column_attr = u8"COLLATE utf8_bin NOT NULL";
        return column_attr;
    }

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

    virtual std::string ListSQL(size_t idx = 0)
    {
        (void)idx;
        return "";
    }

    virtual std::string ListNameSQL(size_t idx = 0)
    {
        (void)idx;
        return "";
    }

    virtual std::string SetSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0)
    {
        (void)client_mysql;
        (void)idx;
        return "";
    }

    virtual std::string AndSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0)
    {
        (void)client_mysql;
        (void)idx;
        return "";
    }

    virtual std::string OrSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0)
    {
        (void)client_mysql;
        (void)idx;
        return "";
    }

    virtual std::string ValueSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0)
    {
        (void)client_mysql;
        (void)idx;
        return "";
    }

    virtual std::string CreateSQL(void)
    {
        return "";
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

struct SDynaFieldList
    :public SFieldList<>
{
    SDynaFieldList()
    {
        m_dyna_fields = create_rb_tree(0);
    }

    SDynaFieldList(const SDynaFieldList& rhs)
    {
        m_dyna_fields = create_rb_tree(0);

        HRBNODE rhs_node = rb_first(rhs.m_dyna_fields);

        while (rhs_node)
        {
            IField* field = (IField*)rb_node_value_user(rhs_node);
            rb_tree_insert_user(m_dyna_fields, field->GetColumnName().c_str(), field->Clone());
            rhs_node = rb_next(rhs_node);
        }
    }

    ~SDynaFieldList()
    {
        if (m_dyna_fields)
        {
            HRBNODE node = rb_first(m_dyna_fields);
            while (node)
            {
                S_DELETE(rb_node_value_user(node));
                node = rb_next(node);
            }

            destroy_rb_tree(m_dyna_fields);
            m_dyna_fields = nullptr;
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
        || std::is_base_of<FieldTXT, T>::value, bool>::type AddField(const T& field)
    {
        HRBNODE node = nullptr;
        IField* new_field = S_NEW(T, 1, field);

        if (rb_tree_try_insert_user(m_dyna_fields, new_field->GetColumnName().c_str(), new_field, &node))
        {
            return true;
        }

        S_DELETE(new_field);

        return false;
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
        HRBNODE node = nullptr;

        if (rb_tree_try_insert_user(m_dyna_fields, field.GetColumnName().c_str(), nullptr, &node))
        {
            rb_node_set_value_user(node, S_NEW(T, 1, field));
        }
        else
        {
            IField* old_field = static_cast<IField*>(rb_node_value_user(node));
            S_DELETE(old_field);
            rb_node_set_value_user(node, S_NEW(T, 1, field));
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
        T del_field;

        HRBNODE node = rb_tree_find_user(m_dyna_fields, del_field.GetColumnName().c_str());

        if (node)
        {
            S_DELETE(rb_node_value_user(node));
            rb_tree_erase(m_dyna_fields, node);
        }
    }

    void Clear(void)
    {
        HRBNODE node = rb_first(m_dyna_fields);
        while (node)
        {
            S_DELETE(rb_node_value_user(node));
            node = rb_next(node);
        }

        rb_tree_clear(m_dyna_fields);
    }

    std::string ListSQL(size_t idx = 0) override
    {
        std::string sql;

        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            if (idx == 0)
            {
                sql += u8" ";
                idx++;
            }
            else
            {
                sql += u8", ";
            }
            IField* field = (IField*)rb_node_value_user(node);

            sql += u8"`" + field->GetColumnName() + u8"`";

            node = rb_next(node);
        }

        return sql;
    }

    std::string ListNameSQL(size_t idx = 0) override
    {
        std::string sql;

        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            if (idx == 0)
            {
                sql += u8" ";
                idx++;
            }
            else
            {
                sql += u8", ";
            }
            IField* field = (IField*)rb_node_value_user(node);

            sql += u8"`" + field->GetColumnName() + u8"`";

            node = rb_next(node);
        }

        return sql;
    }

    std::string SetSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0) override
    {
        std::string sql;

        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            IField* field = (IField*)rb_node_value_user(node);

            if (idx)
            {
                sql += u8", ";
            }

            sql += u8"`" + field->GetColumnName() + u8"`";
            sql += u8" = ";
            sql += field->ToSQL(client_mysql);

            node = rb_next(node);
            ++idx;
        }

        return sql;
    }

    std::string AndSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0) override
    {
        std::string sql;

        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            IField* field = (IField*)rb_node_value_user(node);

            if (idx)
            {
                sql += u8" AND ";
            }

            sql += u8"`" + field->GetColumnName() + u8"`";
            sql += " = ";
            sql += field->ToSQL(client_mysql);

            node = rb_next(node);
            ++idx;
        }

        return sql;
    }

    std::string OrSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0) override
    {
        std::string sql;

        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            IField* field = (IField*)rb_node_value_user(node);

            if (idx)
            {
                sql += u8" OR ";
            }

            sql += u8"`" + field->GetColumnName() + u8"`";
            sql += " = ";
            sql += field->ToSQL(client_mysql);

            node = rb_next(node);
            ++idx;
        }

        return sql;
    }

    std::string ValueSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0) override
    {
        std::string sql;

        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            IField* field = (IField*)rb_node_value_user(node);

            if (idx)
            {
                sql += u8", ";
            }

            sql += field->ToSQL(client_mysql);

            node = rb_next(node);
            ++idx;
        }

        return sql;
    }

    std::string CreateSQL(void) override
    {
        std::string sql;

        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            IField* field = (IField*)rb_node_value_user(node);

            sql +=
                u8"`" +
                field->GetColumnName() +
                u8"` " +
                field->GetColumnType() +
                u8" " +
                field->GetColumnExtra() +
                u8" ";
            if (field->GetColumnComment().length())
            {
                sql += u8"COMMENT \"";
                sql += field->GetColumnComment();
                sql += u8"\",";
            }
            else
            {
                sql += u8",";
            }

            node = rb_next(node);
        }

        return sql;
    }

    void GetDesc(std::vector<std::string>& name_type_list) override
    {
        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            IField* field = (IField*)rb_node_value_user(node);

            name_type_list.push_back(field->GetColumnName());
            name_type_list.push_back(field->GetColumnType());
            name_type_list.push_back(field->GetColumnExtra());

            node = rb_next(node);
        }
    }

    virtual size_t size() const override
    {
        return rb_tree_size(m_dyna_fields);
    }

    void LoadData(const CLIENTMYSQLROW& row, unsigned long idx = 0) override
    {
        HRBNODE node = rb_first(m_dyna_fields);

        while (node)
        {
            IField* field = (IField*)rb_node_value_user(node);
            field->FromSQL(client_mysql_value(row, idx));
            node = rb_next(node);
            idx++;
        }
    }

    bool operator == (const SDynaFieldList& rhs) const
    {
        if (rb_tree_size(m_dyna_fields) != rb_tree_size(rhs.m_dyna_fields))
        {
            return false;
        }

        HRBNODE node = rb_first(m_dyna_fields);
        HRBNODE rhs_node = rb_first(rhs.m_dyna_fields);

        while (node)
        {
            IField* field = (IField*)rb_node_value_user(node);
            IField* rhs_field = (IField*)rb_node_value_user(rhs_node);

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


            node = rb_next(node);
            rhs_node = rb_next(rhs_node);
        }

        return true;
    }

    bool operator != (const SDynaFieldList& rhs) const
    {
        return !(*this == rhs);
    }

    HRBTREE m_dyna_fields;
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

    std::string ListSQL(size_t idx = 0) override
    {
        std::string sql;

        if (idx == 0)
        {
            sql += u8" ";
        }
        else
        {
            sql += u8", ";
        }

        sql += u8"`" + First::ColumnName() + u8"`";

        return sql += SFieldList<Rest...>::ListSQL(idx + 1);
    }

    std::string ListNameSQL(size_t idx = 0) override
    {
        std::string sql;

        if (idx == 0)
        {
            sql += u8" ";
        }
        else
        {
            sql += u8", ";
        }

        sql += u8"`" + First::ColumnName() + u8"`";

        return sql += SFieldList<Rest...>::ListNameSQL(idx + 1);
    }

    std::string SetSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0) override
    {
        std::string sql;

        if (idx)
        {
            sql += u8", ";
        }
        
        sql += u8"`" + First::ColumnName() + u8"`";
        sql += u8" = ";
        sql += value.ToSQL(client_mysql);

        return sql += SFieldList<Rest...>::SetSQL(client_mysql, idx + 1);
    }

    std::string AndSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0) override
    {
        std::string sql;

        if (idx)
        {
            sql += u8" AND ";
        }

        sql += u8"`" + First::ColumnName() + u8"`";
        sql += " = ";
        sql += value.ToSQL(client_mysql);

        return sql += SFieldList<Rest...>::AndSQL(client_mysql, idx + 1);
    }

    std::string OrSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0) override
    {
        std::string sql;

        if (idx)
        {
            sql += u8" OR ";
        }

        sql += u8"`" + First::ColumnName() + u8"`";
        sql += u8" = ";
        sql += value.ToSQL(client_mysql);

        return sql += SFieldList<Rest...>::OrSQL(client_mysql, idx + 1);
    }

    std::string ValueSQL(HCLIENTMYSQL& client_mysql, size_t idx = 0) override
    {
        std::string sql;

        if (idx)
        {
            sql += u8", ";
        }

        sql += value.ToSQL(client_mysql);

        return sql += SFieldList<Rest...>::ValueSQL(client_mysql, idx + 1);
    }

    std::string CreateSQL(void) override
    {
        std::string sql;

        sql +=
            u8"`" +
            First::ColumnName() +
            u8"` " +
            First::ColumnType() +
            u8" " +
            First::ColumnExtra() +
            u8" ";
            if (First::ColumnComment().length())
            {
                sql += u8"COMMENT \"";
                sql += First::ColumnComment();
                sql += u8"\",";
            }
            else
            {
                sql += u8",";
            }
            
            

        return sql += SFieldList<Rest...>::CreateSQL();
    }

    void GetDesc(std::vector<std::string>& name_type_list) override
    {
        name_type_list.push_back(First::ColumnName());
        name_type_list.push_back(First::ColumnType());
        name_type_list.push_back(First::ColumnExtra());

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
    void Modify(const F& field, SDynaFieldList* modify_field_list = nullptr)
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
    void ModifyList(const SFieldList<F...>& fields, SDynaFieldList* modify_field_list = nullptr)
    {
        int arr[] = { (Modify(fields.template Field<F>(), modify_field_list), 0)... };
        (void)(arr);
    }

    template<typename... F>
    void ModifyEx(SDynaFieldList* modify_field_list, F&&... fields)
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

//template<typename... F>
//void SDynaFromFieldList(const SFieldList<F...>& fields, SDynaFieldList& modify_field_list)
//{
//    int arr[] = { (modify_field_list.UpdField(fields.Field<F>()), 0)... };
//}

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

    void Init(CLIENTMYSQLRES& res)
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


//template <typename First, typename... Rest>
//void SFieldList<First, Rest...>::Modify(const SFieldList<>& fields)
//{
//
//}

//template <typename First, typename... Rest>
//template<typename... F>
//void SFieldList<First, Rest...>::Modify(const SFieldList<F...>& fields)
//{
//    if (this->Field<F>().GetData() != fields.Field<F>().GetData())
//    {
//        this->Field<F>().SetData(fields.Field<F>().GetData());
//    }
//    Modify(static_cast<const SFieldList<FRest...>&>(fields));
//}

//template<typename... F>
//template <typename First, typename... Rest>
//void SFieldList<First, Rest...>::Modify(const SFieldList<F...>& fields)
//{
//
//}

//template <typename First, typename... Rest>
//template<typename... F>
//void SFieldList<First, Rest...>::Modify(const SFieldList<F...>& fields)
//{
//    for (size_t i = 0; i < fields.size(); i++)
//    {
//        std::tuple
//        this->Field<>()
//    }
//}

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
    virtual void OnResult(CLIENTMYSQLRES& res) = 0;
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

    void OnResult(CLIENTMYSQLRES& res) override
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
std::vector<SFieldList<Args...>> Record2FieldList(CLIENTMYSQLRES& res)
{
    std::vector<SFieldList<Args...>> datas;
    datas.resize(client_mysql_rows_num(&res));

    for (auto& field : datas)
    {
        field.LoadData(client_mysql_fetch_row(&res));
    }

    return std::move(datas);
}

extern std::vector<SRecordFieldList> Record2FieldList(CLIENTMYSQLRES& res);

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

    void OnResult(CLIENTMYSQLRES& res) override
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
class ITable
{
public:
    ITable(HMYSQLCONNECTION connection)
        :m_connection(connection) {}
public:
    inline HMYSQLCONNECTION Connection(void) { return m_connection; }
    static const std::string& TableNull(void) { static std::string table_null = u8""; return table_null; }
    static const std::string& TableExtra(void) { static std::string table_extra = u8""; return table_extra; }
    static const std::string& DefTableEngine(void) { static std::string table_engine = u8"InnoDB"; return table_engine; }
    static const std::string& DefTableCharset(void) { static std::string table_charset = u8"utf8"; return table_charset; }
    static const std::string& DefTableRowFormat(void) { static std::string table_row_format = u8"compact"; return table_row_format; }
    virtual const std::string& GetTableName(void) const = 0;
    virtual const std::string& GetTableExtra(void) const = 0;
    virtual const std::string& GetTableEngine(void) const = 0;
    virtual const std::string& GetTableCharset(void) const = 0;
    virtual const std::string& GetTableRowFormat(void) const = 0;
    virtual SFieldList<>* FieldList(void) = 0;
    virtual SFieldList<>* PrimaryKey(void) = 0;
    virtual std::map<std::string, SFieldList<>*> UniqueKey(void) = 0;
    virtual std::map<std::string, SFieldList<>*> IndexKey(void) = 0;
protected:
private:
    HMYSQLCONNECTION   m_connection;
};