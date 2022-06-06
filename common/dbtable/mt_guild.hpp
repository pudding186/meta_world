#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_guild
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
    class id
        :public FieldUINT32
    {
    public:
        id(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"id");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = static_string_literal(u8"NOT NULL AUTO_INCREMENT");
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = id::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = id::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = id::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = id::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(id, 1, *this); }
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
    class declaration
        :public FieldVarChar<1024>
    {
    public:
        declaration(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        declaration(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"declaration");
        static constexpr auto ColumnType = FieldVarChar<1024>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<1024>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = declaration::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = declaration::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = declaration::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = declaration::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(declaration, 1, *this); }
    };
    class leader_guid
        :public FieldUINT64
    {
    public:
        leader_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"leader_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = leader_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = leader_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = leader_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = leader_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(leader_guid, 1, *this); }
    };
    class leader_name
        :public FieldVarChar<32>
    {
    public:
        leader_name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        leader_name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"leader_name");
        static constexpr auto ColumnType = FieldVarChar<32>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<32>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = leader_name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = leader_name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = leader_name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = leader_name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(leader_name, 1, *this); }
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
    class base_level
        :public FieldUINT32
    {
    public:
        base_level(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"base_level");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = base_level::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = base_level::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = base_level::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = base_level::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(base_level, 1, *this); }
    };
    class wing_room_level
        :public FieldUINT32
    {
    public:
        wing_room_level(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"wing_room_level");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = wing_room_level::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = wing_room_level::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = wing_room_level::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = wing_room_level::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(wing_room_level, 1, *this); }
    };
    class vault_level
        :public FieldUINT32
    {
    public:
        vault_level(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"vault_level");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = vault_level::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = vault_level::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = vault_level::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = vault_level::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(vault_level, 1, *this); }
    };
    class academy_level
        :public FieldUINT32
    {
    public:
        academy_level(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"academy_level");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = academy_level::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = academy_level::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = academy_level::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = academy_level::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(academy_level, 1, *this); }
    };
    class pharmacy_level
        :public FieldUINT32
    {
    public:
        pharmacy_level(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"pharmacy_level");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = pharmacy_level::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = pharmacy_level::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = pharmacy_level::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = pharmacy_level::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(pharmacy_level, 1, *this); }
    };
    class upgrade_cooldown
        :public FieldUINT32
    {
    public:
        upgrade_cooldown(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"upgrade_cooldown");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = upgrade_cooldown::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = upgrade_cooldown::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = upgrade_cooldown::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = upgrade_cooldown::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(upgrade_cooldown, 1, *this); }
    };
    class fund
        :public FieldUINT32
    {
    public:
        fund(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"fund");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = fund::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = fund::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = fund::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = fund::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(fund, 1, *this); }
    };
    class liveness
        :public FieldUINT32
    {
    public:
        liveness(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"liveness");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = liveness::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = liveness::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = liveness::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = liveness::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(liveness, 1, *this); }
    };
    class build_degree
        :public FieldUINT32
    {
    public:
        build_degree(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"build_degree");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = build_degree::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = build_degree::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = build_degree::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = build_degree::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(build_degree, 1, *this); }
    };
    class prestige
        :public FieldUINT32
    {
    public:
        prestige(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"prestige");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = prestige::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = prestige::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = prestige::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = prestige::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(prestige, 1, *this); }
    };
    class fight_score
        :public FieldUINT32
    {
    public:
        fight_score(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"fight_score");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = fight_score::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = fight_score::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = fight_score::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = fight_score::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(fight_score, 1, *this); }
    };
    class config
        :public FieldUINT32
    {
    public:
        config(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"config");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = config::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = config::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = config::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = config::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(config, 1, *this); }
    };
    class create_time
        :public FieldUINT32
    {
    public:
        create_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"create_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = create_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = create_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = create_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = create_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(create_time, 1, *this); }
    };
    class dismissed_time
        :public FieldUINT32
    {
    public:
        dismissed_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"dismissed_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = dismissed_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = dismissed_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = dismissed_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = dismissed_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(dismissed_time, 1, *this); }
    };
    class impeach_time
        :public FieldUINT32
    {
    public:
        impeach_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"impeach_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = impeach_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = impeach_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = impeach_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = impeach_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(impeach_time, 1, *this); }
    };
    class impeach_guid
        :public FieldUINT64
    {
    public:
        impeach_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"impeach_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = impeach_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = impeach_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = impeach_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = impeach_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(impeach_guid, 1, *this); }
    };
    class notice_time
        :public FieldUINT32
    {
    public:
        notice_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"notice_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = notice_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = notice_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = notice_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = notice_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(notice_time, 1, *this); }
    };
    class notice
        :public FieldVarChar<1024>
    {
    public:
        notice(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        notice(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"notice");
        static constexpr auto ColumnType = FieldVarChar<1024>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<1024>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = notice::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = notice::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = notice::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = notice::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(notice, 1, *this); }
    };
    class events
        :public FieldLongText
    {
    public:
        events(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        events(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"events");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = events::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = events::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = events::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = events::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(events, 1, *this); }
    };
    class items
        :public FieldLongText
    {
    public:
        items(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        items(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"items");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = items::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = items::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = items::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = items::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(items, 1, *this); }
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
        mt_guild::guid,
        mt_guild::id,
        mt_guild::name,
        mt_guild::declaration,
        mt_guild::leader_guid,
        mt_guild::leader_name,
        mt_guild::level,
        mt_guild::base_level,
        mt_guild::wing_room_level,
        mt_guild::vault_level,
        mt_guild::academy_level,
        mt_guild::pharmacy_level,
        mt_guild::upgrade_cooldown,
        mt_guild::fund,
        mt_guild::liveness,
        mt_guild::build_degree,
        mt_guild::prestige,
        mt_guild::fight_score,
        mt_guild::config,
        mt_guild::create_time,
        mt_guild::dismissed_time,
        mt_guild::impeach_time,
        mt_guild::impeach_guid,
        mt_guild::notice_time,
        mt_guild::notice,
        mt_guild::events,
        mt_guild::items,
        mt_guild::customs
    >;
    using PriKey = SFieldList<
        mt_guild::guid
    >;
    mt_guild(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_guild";
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

    const std::string& GetTableName(void) const override { return mt_guild::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_guild::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_guild::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_guild::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_guild::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_guild::guid
        > primary_guid;
        return &primary_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_guild::guid>().GetData()
        );
    }
    std::map<std::string, SFieldList<>*> UniqueKey(void) override
    {
        std::map<std::string, SFieldList<>*> unique_map;
        static SFieldList<
        mt_guild::id
        > unique_id;
        unique_map[u8"id"] = &unique_id;
        return unique_map;
    }
    std::map<std::string, SFieldList<>*> IndexKey(void) override
    {
        std::map<std::string, SFieldList<>*> index_map;
        return index_map;
    }
    
    inline DBTableCache<mt_guild>& Cache(void) { return m_cache; }
    DBTableCache<mt_guild> m_cache;
};
