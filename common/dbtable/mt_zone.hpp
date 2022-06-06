#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_zone
    :public IDBTable
{
public:
    class zone_id
        :public FieldUINT32
    {
    public:
        zone_id(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"zone_id");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"区id");
        const std::string& GetColumnName(void) const override { static const std::string column_name = zone_id::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = zone_id::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = zone_id::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = zone_id::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(zone_id, 1, *this); }
    };
    class zone_type
        :public FieldUINT32
    {
    public:
        zone_type(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"zone_type");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"区类型 1=大话 2=梦幻");
        const std::string& GetColumnName(void) const override { static const std::string column_name = zone_type::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = zone_type::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = zone_type::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = zone_type::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(zone_type, 1, *this); }
    };
    class zone_name
        :public FieldVarChar<64>
    {
    public:
        zone_name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        zone_name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"zone_name");
        static constexpr auto ColumnType = FieldVarChar<64>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<64>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"区名");
        const std::string& GetColumnName(void) const override { static const std::string column_name = zone_name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = zone_name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = zone_name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = zone_name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(zone_name, 1, *this); }
    };
    class zone_time
        :public FieldINT64
    {
    public:
        zone_time(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"zone_time");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"上次区关闭时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = zone_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = zone_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = zone_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = zone_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(zone_time, 1, *this); }
    };
    using Row = SFieldList<
        mt_zone::zone_id,
        mt_zone::zone_type,
        mt_zone::zone_name,
        mt_zone::zone_time
    >;
    using PriKey = SFieldList<
        mt_zone::zone_id
    >;
    mt_zone(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_zone";
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

    const std::string& GetTableName(void) const override { return mt_zone::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_zone::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_zone::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_zone::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_zone::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_zone::zone_id
        > primary_zone_id;
        return &primary_zone_id;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_zone::zone_id>().GetData()
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
    
    inline DBTableCache<mt_zone>& Cache(void) { return m_cache; }
    DBTableCache<mt_zone> m_cache;
};
