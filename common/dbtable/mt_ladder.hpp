#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_ladder
    :public IDBTable
{
public:
    class player_guid
        :public FieldUINT64
    {
    public:
        player_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"player_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = player_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = player_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = player_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = player_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(player_guid, 1, *this); }
    };
    class rank
        :public FieldUINT32
    {
    public:
        rank(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"rank");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = rank::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = rank::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = rank::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = rank::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(rank, 1, *this); }
    };
    class player_data
        :public FieldLongText
    {
    public:
        player_data(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        player_data(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"player_data");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = player_data::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = player_data::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = player_data::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = player_data::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(player_data, 1, *this); }
    };
    using Row = SFieldList<
        mt_ladder::player_guid,
        mt_ladder::rank,
        mt_ladder::player_data
    >;
    using PriKey = SFieldList<
        mt_ladder::player_guid
    >;
    mt_ladder(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_ladder";
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

    const std::string& GetTableName(void) const override { return mt_ladder::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_ladder::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_ladder::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_ladder::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_ladder::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_ladder::player_guid
        > primary_player_guid;
        return &primary_player_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_ladder::player_guid>().GetData()
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
    
    inline DBTableCache<mt_ladder>& Cache(void) { return m_cache; }
    DBTableCache<mt_ladder> m_cache;
};
