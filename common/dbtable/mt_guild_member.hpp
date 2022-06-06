#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_guild_member
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
    class guild_job
        :public FieldUINT32
    {
    public:
        guild_job(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"guild_job");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = guild_job::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = guild_job::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = guild_job::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = guild_job::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(guild_job, 1, *this); }
    };
    class total_contrb
        :public FieldUINT32
    {
    public:
        total_contrb(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"total_contrb");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = total_contrb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = total_contrb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = total_contrb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = total_contrb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(total_contrb, 1, *this); }
    };
    class surplus_contrb
        :public FieldUINT32
    {
    public:
        surplus_contrb(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"surplus_contrb");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = surplus_contrb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = surplus_contrb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = surplus_contrb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = surplus_contrb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(surplus_contrb, 1, *this); }
    };
    class last_week_contrb
        :public FieldUINT32
    {
    public:
        last_week_contrb(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_week_contrb");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_week_contrb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_week_contrb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_week_contrb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_week_contrb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_week_contrb, 1, *this); }
    };
    class this_week_contrb
        :public FieldUINT32
    {
    public:
        this_week_contrb(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"this_week_contrb");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = this_week_contrb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = this_week_contrb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = this_week_contrb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = this_week_contrb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(this_week_contrb, 1, *this); }
    };
    class permission
        :public FieldUINT32
    {
    public:
        permission(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"permission");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = permission::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = permission::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = permission::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = permission::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(permission, 1, *this); }
    };
    class join_time
        :public FieldUINT32
    {
    public:
        join_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"join_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = join_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = join_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = join_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = join_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(join_time, 1, *this); }
    };
    class donate_bind_gold
        :public FieldUINT32
    {
    public:
        donate_bind_gold(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"donate_bind_gold");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = donate_bind_gold::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = donate_bind_gold::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = donate_bind_gold::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = donate_bind_gold::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(donate_bind_gold, 1, *this); }
    };
    class last_donate_time
        :public FieldUINT32
    {
    public:
        last_donate_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_donate_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_donate_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_donate_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_donate_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_donate_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_donate_time, 1, *this); }
    };
    class last_dividend_time
        :public FieldUINT32
    {
    public:
        last_dividend_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_dividend_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_dividend_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_dividend_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_dividend_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_dividend_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_dividend_time, 1, *this); }
    };
    class last_login_time
        :public FieldUINT32
    {
    public:
        last_login_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_login_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_login_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_login_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_login_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_login_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_login_time, 1, *this); }
    };
    class last_logout_time
        :public FieldUINT32
    {
    public:
        last_logout_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_logout_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_logout_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_logout_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_logout_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_logout_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_logout_time, 1, *this); }
    };
    class forbid_end_time
        :public FieldUINT32
    {
    public:
        forbid_end_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"forbid_end_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = forbid_end_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = forbid_end_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = forbid_end_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = forbid_end_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(forbid_end_time, 1, *this); }
    };
    class status
        :public FieldUINT32
    {
    public:
        status(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"status");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = status::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = status::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = status::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = status::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(status, 1, *this); }
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
    class job
        :public FieldINT8
    {
    public:
        job(char data = 0) :FieldINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"job");
        static constexpr auto ColumnType = FieldINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = job::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = job::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = job::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = job::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(job, 1, *this); }
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
        mt_guild_member::player_guid,
        mt_guild_member::guild_guid,
        mt_guild_member::player_name,
        mt_guild_member::level,
        mt_guild_member::role,
        mt_guild_member::guild_job,
        mt_guild_member::total_contrb,
        mt_guild_member::surplus_contrb,
        mt_guild_member::last_week_contrb,
        mt_guild_member::this_week_contrb,
        mt_guild_member::permission,
        mt_guild_member::join_time,
        mt_guild_member::donate_bind_gold,
        mt_guild_member::last_donate_time,
        mt_guild_member::last_dividend_time,
        mt_guild_member::last_login_time,
        mt_guild_member::last_logout_time,
        mt_guild_member::forbid_end_time,
        mt_guild_member::status,
        mt_guild_member::vip,
        mt_guild_member::reincarnation,
        mt_guild_member::total_fight_value,
        mt_guild_member::job,
        mt_guild_member::customs
    >;
    using PriKey = SFieldList<
        mt_guild_member::player_guid
    >;
    mt_guild_member(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_guild_member";
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

    const std::string& GetTableName(void) const override { return mt_guild_member::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_guild_member::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_guild_member::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_guild_member::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_guild_member::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_guild_member::player_guid
        > primary_player_guid;
        return &primary_player_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_guild_member::player_guid>().GetData()
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
    
    inline DBTableCache<mt_guild_member>& Cache(void) { return m_cache; }
    DBTableCache<mt_guild_member> m_cache;
};
