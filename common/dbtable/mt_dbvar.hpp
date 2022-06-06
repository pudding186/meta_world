#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_dbvar
    :public IDBTable
{
public:
    class key
        :public FieldVarChar<128>
    {
    public:
        key(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        key(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"key");
        static constexpr auto ColumnType = FieldVarChar<128>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<128>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = key::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = key::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = key::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = key::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(key, 1, *this); }
    };
    class value
        :public FieldLongText
    {
    public:
        value(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        value(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"value");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = value::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = value::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = value::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = value::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(value, 1, *this); }
    };
    class value_type
        :public FieldUINT32
    {
    public:
        value_type(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"value_type");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = value_type::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = value_type::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = value_type::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = value_type::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(value_type, 1, *this); }
    };
    class merge_type
        :public FieldUINT32
    {
    public:
        merge_type(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"merge_type");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = merge_type::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = merge_type::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = merge_type::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = merge_type::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(merge_type, 1, *this); }
    };
    using Row = SFieldList<
        mt_dbvar::key,
        mt_dbvar::value,
        mt_dbvar::value_type,
        mt_dbvar::merge_type
    >;
    using PriKey = SFieldList<
        mt_dbvar::key,
        mt_dbvar::value_type
    >;
    mt_dbvar(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_dbvar";
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

    const std::string& GetTableName(void) const override { return mt_dbvar::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_dbvar::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_dbvar::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_dbvar::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_dbvar::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_dbvar::key,
        mt_dbvar::value_type
        > primary_key_value_type;
        return &primary_key_value_type;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_dbvar::key>().GetData(),
        row.Field<mt_dbvar::value_type>().GetData()
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
    
    inline DBTableCache<mt_dbvar>& Cache(void) { return m_cache; }
    DBTableCache<mt_dbvar> m_cache;
};
