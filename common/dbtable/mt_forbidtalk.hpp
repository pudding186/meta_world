#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_forbidtalk
    :public IDBTable
{
public:
    class guid
        :public FieldUINT64
    {
    public:
        guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"角色GUID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(guid, 1, *this); }
    };
    class begin
        :public FieldINT64
    {
    public:
        begin(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"begin");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"起始时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = begin::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = begin::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = begin::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = begin::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(begin, 1, *this); }
    };
    class end
        :public FieldINT64
    {
    public:
        end(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"end");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"结束时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = end::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = end::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = end::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = end::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(end, 1, *this); }
    };
    class desc
        :public FieldVarChar<255>
    {
    public:
        desc(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        desc(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"desc");
        static constexpr auto ColumnType = FieldVarChar<255>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<255>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"描述");
        const std::string& GetColumnName(void) const override { static const std::string column_name = desc::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = desc::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = desc::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = desc::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(desc, 1, *this); }
    };
    using Row = SFieldList<
        mt_forbidtalk::guid,
        mt_forbidtalk::begin,
        mt_forbidtalk::end,
        mt_forbidtalk::desc
    >;
    using PriKey = SFieldList<
        mt_forbidtalk::guid
    >;
    mt_forbidtalk(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_forbidtalk";
        return table_name;
    }

    static const std::string& TableEngine(void)
    {
        return IDBTable::DefTableEngine();
    }

    static const std::string& TableCharset(void)
    {
        return IDBTable::DefTableCharset();
    }

    static const std::string& TableRowFormat(void)
    {
        return IDBTable::DefTableRowFormat();
    }

    static const std::string& TableExtra(void)
    {
        return IDBTable::TableNull();
    }

    const std::string& GetTableName(void) const override { return mt_forbidtalk::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_forbidtalk::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_forbidtalk::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_forbidtalk::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_forbidtalk::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_forbidtalk::guid
        > primary_guid;
        return &primary_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_forbidtalk::guid>().GetData()
        );
    }
    std::map<std::string, SFieldList<>*> UniqueKey(void) override
    {
        std::map<std::string, SFieldList<>*> unique_map;
        return unique_map;
    }
    std::map<std::string, SFieldList<>*> IndexKey(void) override
    {
        std::map<std::string, SFieldList<>*> index_map;
        return index_map;
    }
    
    inline DBTableCache<mt_forbidtalk>& Cache(void) { return m_cache; }
    DBTableCache<mt_forbidtalk> m_cache;
};
