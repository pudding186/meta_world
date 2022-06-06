#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_ranklist
    :public IDBTable
{
public:
    class type
        :public FieldINT32
    {
    public:
        type(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"type");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = type::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = type::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = type::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = type::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(type, 1, *this); }
    };
    class rank_guid
        :public FieldUINT64
    {
    public:
        rank_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"rank_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = rank_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = rank_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = rank_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = rank_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(rank_guid, 1, *this); }
    };
    class object_guid
        :public FieldUINT64
    {
    public:
        object_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"object_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = object_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = object_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = object_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = object_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(object_guid, 1, *this); }
    };
    class rank_data1
        :public FieldINT64
    {
    public:
        rank_data1(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"rank_data1");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = rank_data1::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = rank_data1::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = rank_data1::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = rank_data1::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(rank_data1, 1, *this); }
    };
    class rank_data2
        :public FieldINT64
    {
    public:
        rank_data2(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"rank_data2");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = rank_data2::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = rank_data2::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = rank_data2::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = rank_data2::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(rank_data2, 1, *this); }
    };
    class rank_data3
        :public FieldINT64
    {
    public:
        rank_data3(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"rank_data3");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = rank_data3::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = rank_data3::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = rank_data3::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = rank_data3::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(rank_data3, 1, *this); }
    };
    class name
        :public FieldVarChar<32>
    {
    public:
        name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"name");
        static constexpr auto ColumnType = FieldVarChar<32>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<32>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(name, 1, *this); }
    };
    class info
        :public FieldLongText
    {
    public:
        info(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        info(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"info");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = info::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = info::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = info::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = info::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(info, 1, *this); }
    };
    using Row = SFieldList<
        mt_ranklist::type,
        mt_ranklist::rank_guid,
        mt_ranklist::object_guid,
        mt_ranklist::rank_data1,
        mt_ranklist::rank_data2,
        mt_ranklist::rank_data3,
        mt_ranklist::name,
        mt_ranklist::info
    >;
    using PriKey = SFieldList<
        mt_ranklist::object_guid,
        mt_ranklist::type
    >;
    mt_ranklist(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_ranklist";
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

    const std::string& GetTableName(void) const override { return mt_ranklist::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_ranklist::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_ranklist::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_ranklist::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_ranklist::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_ranklist::object_guid,
        mt_ranklist::type
        > primary_object_guid_type;
        return &primary_object_guid_type;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_ranklist::object_guid>().GetData(),
        row.Field<mt_ranklist::type>().GetData()
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
    
    inline DBTableCache<mt_ranklist>& Cache(void) { return m_cache; }
    DBTableCache<mt_ranklist> m_cache;
};
