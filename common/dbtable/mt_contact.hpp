#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_contact
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
    class contact_guid
        :public FieldUINT64
    {
    public:
        contact_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"contact_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = contact_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = contact_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = contact_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = contact_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(contact_guid, 1, *this); }
    };
    class contact_type
        :public FieldUINT32
    {
    public:
        contact_type(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"contact_type");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = contact_type::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = contact_type::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = contact_type::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = contact_type::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(contact_type, 1, *this); }
    };
    class friendship
        :public FieldUINT32
    {
    public:
        friendship(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"friendship");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = friendship::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = friendship::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = friendship::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = friendship::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(friendship, 1, *this); }
    };
    class last_contact_time
        :public FieldUINT32
    {
    public:
        last_contact_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_contact_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_contact_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_contact_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_contact_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_contact_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_contact_time, 1, *this); }
    };
    class customs
        :public FieldLongText
    {
    public:
        customs(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        customs(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"customs");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = customs::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = customs::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = customs::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = customs::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(customs, 1, *this); }
    };
    using Row = SFieldList<
        mt_contact::player_guid,
        mt_contact::contact_guid,
        mt_contact::contact_type,
        mt_contact::friendship,
        mt_contact::last_contact_time,
        mt_contact::customs
    >;
    using PriKey = SFieldList<
        mt_contact::player_guid,
        mt_contact::contact_guid
    >;
    mt_contact(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_contact";
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

    const std::string& GetTableName(void) const override { return mt_contact::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_contact::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_contact::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_contact::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_contact::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_contact::player_guid,
        mt_contact::contact_guid
        > primary_player_guid_contact_guid;
        return &primary_player_guid_contact_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_contact::player_guid>().GetData(),
        row.Field<mt_contact::contact_guid>().GetData()
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
    
    inline DBTableCache<mt_contact>& Cache(void) { return m_cache; }
    DBTableCache<mt_contact> m_cache;
};
