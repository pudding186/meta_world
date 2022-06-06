#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_auction_cookie
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
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(guid, 1, *this); }
    };
    class coins
        :public FieldText
    {
    public:
        coins(const std::string& data = "") :FieldText(data) {}
        template<size_t N>
        coins(const char(&data)[N]) : FieldText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"coins");
        static constexpr auto ColumnType = FieldText::ColumnType;
        static constexpr auto ColumnExtra = FieldText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = coins::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = coins::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = coins::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = coins::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(coins, 1, *this); }
    };
    class favorites
        :public FieldText
    {
    public:
        favorites(const std::string& data = "") :FieldText(data) {}
        template<size_t N>
        favorites(const char(&data)[N]) : FieldText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"favorites");
        static constexpr auto ColumnType = FieldText::ColumnType;
        static constexpr auto ColumnExtra = FieldText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = favorites::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = favorites::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = favorites::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = favorites::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(favorites, 1, *this); }
    };
    class records
        :public FieldText
    {
    public:
        records(const std::string& data = "") :FieldText(data) {}
        template<size_t N>
        records(const char(&data)[N]) : FieldText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"records");
        static constexpr auto ColumnType = FieldText::ColumnType;
        static constexpr auto ColumnExtra = FieldText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = records::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = records::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = records::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = records::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(records, 1, *this); }
    };
    using Row = SFieldList<
        mt_auction_cookie::guid,
        mt_auction_cookie::coins,
        mt_auction_cookie::favorites,
        mt_auction_cookie::records
    >;
    using PriKey = SFieldList<
        mt_auction_cookie::guid
    >;
    mt_auction_cookie(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_auction_cookie";
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

    const std::string& GetTableName(void) const override { return mt_auction_cookie::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_auction_cookie::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_auction_cookie::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_auction_cookie::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_auction_cookie::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_auction_cookie::guid
        > primary_guid;
        return &primary_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_auction_cookie::guid>().GetData()
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
    
    inline DBTableCache<mt_auction_cookie>& Cache(void) { return m_cache; }
    DBTableCache<mt_auction_cookie> m_cache;
};
