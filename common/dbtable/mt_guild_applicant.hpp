#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_guild_applicant
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
    class guild_guid
        :public FieldUINT64
    {
    public:
        guild_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"guild_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = guild_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = guild_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = guild_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = guild_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(guild_guid, 1, *this); }
    };
    class player_name
        :public FieldVarChar<32>
    {
    public:
        player_name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        player_name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"player_name");
        static constexpr auto ColumnType = FieldVarChar<32>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<32>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = player_name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = player_name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = player_name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = player_name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(player_name, 1, *this); }
    };
    class level
        :public FieldUINT32
    {
    public:
        level(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"level");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = level::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = level::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = level::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = level::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(level, 1, *this); }
    };
    class role
        :public FieldUINT32
    {
    public:
        role(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"role");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = role::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = role::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = role::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = role::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(role, 1, *this); }
    };
    class sn
        :public FieldUINT32
    {
    public:
        sn(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"sn");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = sn::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = sn::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = sn::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = sn::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(sn, 1, *this); }
    };
    class guild_contrb
        :public FieldUINT32
    {
    public:
        guild_contrb(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"guild_contrb");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = guild_contrb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = guild_contrb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = guild_contrb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = guild_contrb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(guild_contrb, 1, *this); }
    };
    class last_apply_time
        :public FieldUINT32
    {
    public:
        last_apply_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_apply_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_apply_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_apply_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_apply_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_apply_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_apply_time, 1, *this); }
    };
    class vip
        :public FieldINT32
    {
    public:
        vip(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"vip");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = vip::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = vip::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = vip::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = vip::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(vip, 1, *this); }
    };
    class reincarnation
        :public FieldINT32
    {
    public:
        reincarnation(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"reincarnation");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = reincarnation::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = reincarnation::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = reincarnation::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = reincarnation::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(reincarnation, 1, *this); }
    };
    class total_fight_value
        :public FieldINT64
    {
    public:
        total_fight_value(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"total_fight_value");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = total_fight_value::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = total_fight_value::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = total_fight_value::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = total_fight_value::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(total_fight_value, 1, *this); }
    };
    using Row = SFieldList<
        mt_guild_applicant::player_guid,
        mt_guild_applicant::guild_guid,
        mt_guild_applicant::player_name,
        mt_guild_applicant::level,
        mt_guild_applicant::role,
        mt_guild_applicant::sn,
        mt_guild_applicant::guild_contrb,
        mt_guild_applicant::last_apply_time,
        mt_guild_applicant::vip,
        mt_guild_applicant::reincarnation,
        mt_guild_applicant::total_fight_value
    >;
    using PriKey = SFieldList<
        mt_guild_applicant::player_guid,
        mt_guild_applicant::guild_guid
    >;
    mt_guild_applicant(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_guild_applicant";
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

    const std::string& GetTableName(void) const override { return mt_guild_applicant::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_guild_applicant::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_guild_applicant::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_guild_applicant::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_guild_applicant::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_guild_applicant::player_guid,
        mt_guild_applicant::guild_guid
        > primary_player_guid_guild_guid;
        return &primary_player_guid_guild_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_guild_applicant::player_guid>().GetData(),
        row.Field<mt_guild_applicant::guild_guid>().GetData()
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
    
    inline DBTableCache<mt_guild_applicant>& Cache(void) { return m_cache; }
    DBTableCache<mt_guild_applicant> m_cache;
};
