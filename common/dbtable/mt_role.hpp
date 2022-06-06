#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_role
    :public IDBTable
{
public:
    class sn
        :public FieldUINT32
    {
    public:
        sn(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"sn");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = static_string_literal(u8"NOT NULL AUTO_INCREMENT");
        static constexpr auto ColumnComment = static_string_literal(u8"角色序列号自增");
        const std::string& GetColumnName(void) const override { static const std::string column_name = sn::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = sn::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = sn::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = sn::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(sn, 1, *this); }
    };
    class uid
        :public FieldUINT32
    {
    public:
        uid(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"uid");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"用户ID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = uid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = uid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = uid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = uid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(uid, 1, *this); }
    };
    class sid
        :public FieldUINT8
    {
    public:
        sid(unsigned char data = 0) :FieldUINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"sid");
        static constexpr auto ColumnType = FieldUINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"小区ID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = sid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = sid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = sid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = sid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(sid, 1, *this); }
    };
    class guid
        :public FieldUINT64
    {
    public:
        guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"角色全局唯一ID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(guid, 1, *this); }
    };
    class unid
        :public FieldVarChar<32>
    {
    public:
        unid(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        unid(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"unid");
        static constexpr auto ColumnType = FieldVarChar<32>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<32>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"角色唯一ID合区不改变");
        const std::string& GetColumnName(void) const override { static const std::string column_name = unid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = unid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = unid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = unid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(unid, 1, *this); }
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
    class cur_map
        :public FieldUINT64
    {
    public:
        cur_map(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"cur_map");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"当前地图GUID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = cur_map::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = cur_map::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = cur_map::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = cur_map::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(cur_map, 1, *this); }
    };
    class cur_x
        :public FieldUINT16
    {
    public:
        cur_x(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"cur_x");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"当前地图x坐标");
        const std::string& GetColumnName(void) const override { static const std::string column_name = cur_x::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = cur_x::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = cur_x::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = cur_x::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(cur_x, 1, *this); }
    };
    class cur_y
        :public FieldUINT16
    {
    public:
        cur_y(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"cur_y");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"当前地图y坐标");
        const std::string& GetColumnName(void) const override { static const std::string column_name = cur_y::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = cur_y::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = cur_y::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = cur_y::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(cur_y, 1, *this); }
    };
    class last_common_map
        :public FieldUINT64
    {
    public:
        last_common_map(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_common_map");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"最后固定地图GUID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_common_map::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_common_map::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_common_map::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_common_map::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_common_map, 1, *this); }
    };
    class last_common_x
        :public FieldUINT16
    {
    public:
        last_common_x(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_common_x");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"最后固定地图x坐标");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_common_x::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_common_x::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_common_x::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_common_x::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_common_x, 1, *this); }
    };
    class last_common_y
        :public FieldUINT16
    {
    public:
        last_common_y(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"last_common_y");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"最后固定地图y坐标");
        const std::string& GetColumnName(void) const override { static const std::string column_name = last_common_y::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = last_common_y::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = last_common_y::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = last_common_y::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(last_common_y, 1, *this); }
    };
    class state
        :public FieldUINT8
    {
    public:
        state(unsigned char data = 0) :FieldUINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"state");
        static constexpr auto ColumnType = FieldUINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = state::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = state::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = state::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = state::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(state, 1, *this); }
    };
    class createtime
        :public FieldUINT32
    {
    public:
        createtime(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"createtime");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = createtime::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = createtime::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = createtime::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = createtime::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(createtime, 1, *this); }
    };
    class destroytime
        :public FieldUINT32
    {
    public:
        destroytime(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"destroytime");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = destroytime::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = destroytime::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = destroytime::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = destroytime::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(destroytime, 1, *this); }
    };
    class lastlogin
        :public FieldUINT32
    {
    public:
        lastlogin(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"lastlogin");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = lastlogin::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = lastlogin::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = lastlogin::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = lastlogin::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(lastlogin, 1, *this); }
    };
    class lastloginip
        :public FieldUINT32
    {
    public:
        lastloginip(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"lastloginip");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = lastloginip::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = lastloginip::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = lastloginip::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = lastloginip::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(lastloginip, 1, *this); }
    };
    class lastlogout
        :public FieldUINT32
    {
    public:
        lastlogout(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"lastlogout");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = lastlogout::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = lastlogout::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = lastlogout::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = lastlogout::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(lastlogout, 1, *this); }
    };
    class onlines
        :public FieldUINT32
    {
    public:
        onlines(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"onlines");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = onlines::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = onlines::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = onlines::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = onlines::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(onlines, 1, *this); }
    };
    class roleid
        :public FieldINT32
    {
    public:
        roleid(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"roleid");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = roleid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = roleid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = roleid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = roleid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(roleid, 1, *this); }
    };
    class race
        :public FieldINT32
    {
    public:
        race(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"race");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = race::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = race::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = race::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = race::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(race, 1, *this); }
    };
    class gender
        :public FieldINT8
    {
    public:
        gender(char data = 0) :FieldINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"gender");
        static constexpr auto ColumnType = FieldINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = gender::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = gender::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = gender::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = gender::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(gender, 1, *this); }
    };
    class level
        :public FieldINT32
    {
    public:
        level(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"level");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = level::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = level::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = level::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = level::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(level, 1, *this); }
    };
    class job1
        :public FieldINT8
    {
    public:
        job1(char data = 0) :FieldINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"job1");
        static constexpr auto ColumnType = FieldINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = job1::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = job1::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = job1::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = job1::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(job1, 1, *this); }
    };
    class job2
        :public FieldINT8
    {
    public:
        job2(char data = 0) :FieldINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"job2");
        static constexpr auto ColumnType = FieldINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = job2::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = job2::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = job2::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = job2::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(job2, 1, *this); }
    };
    class job3
        :public FieldINT8
    {
    public:
        job3(char data = 0) :FieldINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"job3");
        static constexpr auto ColumnType = FieldINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = job3::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = job3::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = job3::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = job3::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(job3, 1, *this); }
    };
    class vitpoint
        :public FieldUINT32
    {
    public:
        vitpoint(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"vitpoint");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = vitpoint::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = vitpoint::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = vitpoint::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = vitpoint::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(vitpoint, 1, *this); }
    };
    class intpoint
        :public FieldUINT32
    {
    public:
        intpoint(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"intpoint");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = intpoint::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = intpoint::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = intpoint::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = intpoint::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(intpoint, 1, *this); }
    };
    class strpoint
        :public FieldUINT32
    {
    public:
        strpoint(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"strpoint");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = strpoint::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = strpoint::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = strpoint::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = strpoint::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(strpoint, 1, *this); }
    };
    class agipoint
        :public FieldUINT32
    {
    public:
        agipoint(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"agipoint");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = agipoint::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = agipoint::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = agipoint::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = agipoint::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(agipoint, 1, *this); }
    };
    class endpoint
        :public FieldUINT32
    {
    public:
        endpoint(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"endpoint");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = endpoint::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = endpoint::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = endpoint::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = endpoint::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(endpoint, 1, *this); }
    };
    class remainpoint
        :public FieldUINT32
    {
    public:
        remainpoint(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"remainpoint");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = remainpoint::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = remainpoint::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = remainpoint::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = remainpoint::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(remainpoint, 1, *this); }
    };
    class hp
        :public FieldINT32
    {
    public:
        hp(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"hp");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = hp::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = hp::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = hp::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = hp::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(hp, 1, *this); }
    };
    class mp
        :public FieldINT32
    {
    public:
        mp(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"mp");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = mp::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = mp::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = mp::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = mp::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(mp, 1, *this); }
    };
    class sp
        :public FieldINT32
    {
    public:
        sp(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"sp");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = sp::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = sp::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = sp::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = sp::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(sp, 1, *this); }
    };
    class vp
        :public FieldINT32
    {
    public:
        vp(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"vp");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = vp::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = vp::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = vp::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = vp::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(vp, 1, *this); }
    };
    class hppool
        :public FieldINT32
    {
    public:
        hppool(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"hppool");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = hppool::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = hppool::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = hppool::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = hppool::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(hppool, 1, *this); }
    };
    class mppool
        :public FieldINT32
    {
    public:
        mppool(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"mppool");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = mppool::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = mppool::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = mppool::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = mppool::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(mppool, 1, *this); }
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
    class fight
        :public FieldINT32
    {
    public:
        fight(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"fight");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = fight::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = fight::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = fight::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = fight::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(fight, 1, *this); }
    };
    class activation
        :public FieldINT32
    {
    public:
        activation(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"activation");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = activation::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = activation::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = activation::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = activation::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(activation, 1, *this); }
    };
    class talent1
        :public FieldINT32
    {
    public:
        talent1(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent1");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent1::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent1::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent1::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent1::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent1, 1, *this); }
    };
    class talent2
        :public FieldINT32
    {
    public:
        talent2(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent2");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent2::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent2::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent2::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent2::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent2, 1, *this); }
    };
    class talent3
        :public FieldINT32
    {
    public:
        talent3(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent3");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent3::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent3::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent3::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent3::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent3, 1, *this); }
    };
    class talent4
        :public FieldINT32
    {
    public:
        talent4(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent4");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent4::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent4::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent4::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent4::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent4, 1, *this); }
    };
    class talent5
        :public FieldINT32
    {
    public:
        talent5(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent5");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent5::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent5::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent5::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent5::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent5, 1, *this); }
    };
    class talent6
        :public FieldINT32
    {
    public:
        talent6(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent6");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent6::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent6::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent6::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent6::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent6, 1, *this); }
    };
    class talent7
        :public FieldINT32
    {
    public:
        talent7(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent7");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent7::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent7::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent7::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent7::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent7, 1, *this); }
    };
    class talent8
        :public FieldINT32
    {
    public:
        talent8(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent8");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent8::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent8::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent8::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent8::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent8, 1, *this); }
    };
    class talent9
        :public FieldINT32
    {
    public:
        talent9(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent9");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent9::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent9::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent9::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent9::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent9, 1, *this); }
    };
    class talent10
        :public FieldINT32
    {
    public:
        talent10(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"talent10");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = talent10::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = talent10::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = talent10::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = talent10::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(talent10, 1, *this); }
    };
    class exp
        :public FieldINT64
    {
    public:
        exp(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"exp");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = exp::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = exp::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = exp::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = exp::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(exp, 1, *this); }
    };
    class bindingot
        :public FieldINT64
    {
    public:
        bindingot(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"bindingot");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = bindingot::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = bindingot::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = bindingot::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = bindingot::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(bindingot, 1, *this); }
    };
    class integral
        :public FieldINT64
    {
    public:
        integral(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"integral");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = integral::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = integral::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = integral::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = integral::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(integral, 1, *this); }
    };
    class gold
        :public FieldINT64
    {
    public:
        gold(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"gold");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = gold::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = gold::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = gold::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = gold::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(gold, 1, *this); }
    };
    class bindgold
        :public FieldINT64
    {
    public:
        bindgold(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"bindgold");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = bindgold::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = bindgold::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = bindgold::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = bindgold::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(bindgold, 1, *this); }
    };
    class warehousegold
        :public FieldINT64
    {
    public:
        warehousegold(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"warehousegold");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = warehousegold::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = warehousegold::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = warehousegold::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = warehousegold::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(warehousegold, 1, *this); }
    };
    class weapon
        :public FieldINT32
    {
    public:
        weapon(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"weapon");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = weapon::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = weapon::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = weapon::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = weapon::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(weapon, 1, *this); }
    };
    class amulet
        :public FieldINT32
    {
    public:
        amulet(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"amulet");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = amulet::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = amulet::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = amulet::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = amulet::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(amulet, 1, *this); }
    };
    class mount
        :public FieldINT32
    {
    public:
        mount(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"mount");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = mount::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = mount::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = mount::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = mount::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(mount, 1, *this); }
    };
    class color1
        :public FieldINT32
    {
    public:
        color1(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"color1");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = color1::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = color1::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = color1::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = color1::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(color1, 1, *this); }
    };
    class color2
        :public FieldINT32
    {
    public:
        color2(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"color2");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = color2::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = color2::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = color2::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = color2::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(color2, 1, *this); }
    };
    class shape
        :public FieldINT32
    {
    public:
        shape(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"shape");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = shape::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = shape::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = shape::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = shape::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(shape, 1, *this); }
    };
    class effect1
        :public FieldINT32
    {
    public:
        effect1(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"effect1");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = effect1::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = effect1::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = effect1::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = effect1::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(effect1, 1, *this); }
    };
    class effect2
        :public FieldINT32
    {
    public:
        effect2(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"effect2");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = effect2::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = effect2::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = effect2::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = effect2::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(effect2, 1, *this); }
    };
    class effect3
        :public FieldINT32
    {
    public:
        effect3(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"effect3");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = effect3::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = effect3::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = effect3::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = effect3::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(effect3, 1, *this); }
    };
    class effect4
        :public FieldINT32
    {
    public:
        effect4(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"effect4");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = effect4::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = effect4::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = effect4::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = effect4::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(effect4, 1, *this); }
    };
    class effect5
        :public FieldINT32
    {
    public:
        effect5(int data = 0) :FieldINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"effect5");
        static constexpr auto ColumnType = FieldINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = effect5::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = effect5::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = effect5::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = effect5::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(effect5, 1, *this); }
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
    class skills
        :public FieldLongText
    {
    public:
        skills(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        skills(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"skills");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = skills::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = skills::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = skills::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = skills::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(skills, 1, *this); }
    };
    class buffs
        :public FieldLongText
    {
    public:
        buffs(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        buffs(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"buffs");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = buffs::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = buffs::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = buffs::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = buffs::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(buffs, 1, *this); }
    };
    class quests
        :public FieldLongText
    {
    public:
        quests(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        quests(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"quests");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = quests::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = quests::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = quests::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = quests::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(quests, 1, *this); }
    };
    class pets
        :public FieldLongText
    {
    public:
        pets(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        pets(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"pets");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = pets::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = pets::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = pets::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = pets::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(pets, 1, *this); }
    };
    class guards
        :public FieldLongText
    {
    public:
        guards(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        guards(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"guards");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = guards::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = guards::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = guards::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = guards::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(guards, 1, *this); }
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
        mt_role::sn,
        mt_role::uid,
        mt_role::sid,
        mt_role::guid,
        mt_role::unid,
        mt_role::name,
        mt_role::cur_map,
        mt_role::cur_x,
        mt_role::cur_y,
        mt_role::last_common_map,
        mt_role::last_common_x,
        mt_role::last_common_y,
        mt_role::state,
        mt_role::createtime,
        mt_role::destroytime,
        mt_role::lastlogin,
        mt_role::lastloginip,
        mt_role::lastlogout,
        mt_role::onlines,
        mt_role::roleid,
        mt_role::race,
        mt_role::gender,
        mt_role::level,
        mt_role::job1,
        mt_role::job2,
        mt_role::job3,
        mt_role::vitpoint,
        mt_role::intpoint,
        mt_role::strpoint,
        mt_role::agipoint,
        mt_role::endpoint,
        mt_role::remainpoint,
        mt_role::hp,
        mt_role::mp,
        mt_role::sp,
        mt_role::vp,
        mt_role::hppool,
        mt_role::mppool,
        mt_role::vip,
        mt_role::fight,
        mt_role::activation,
        mt_role::talent1,
        mt_role::talent2,
        mt_role::talent3,
        mt_role::talent4,
        mt_role::talent5,
        mt_role::talent6,
        mt_role::talent7,
        mt_role::talent8,
        mt_role::talent9,
        mt_role::talent10,
        mt_role::exp,
        mt_role::bindingot,
        mt_role::integral,
        mt_role::gold,
        mt_role::bindgold,
        mt_role::warehousegold,
        mt_role::weapon,
        mt_role::amulet,
        mt_role::mount,
        mt_role::color1,
        mt_role::color2,
        mt_role::shape,
        mt_role::effect1,
        mt_role::effect2,
        mt_role::effect3,
        mt_role::effect4,
        mt_role::effect5,
        mt_role::reincarnation,
        mt_role::items,
        mt_role::skills,
        mt_role::buffs,
        mt_role::quests,
        mt_role::pets,
        mt_role::guards,
        mt_role::customs
    >;
    using PriKey = SFieldList<
        mt_role::guid
    >;
    mt_role(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_role";
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
        static std::string table_extra = u8"auto_increment=14";
        return table_extra;
    }

    const std::string& GetTableName(void) const override { return mt_role::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_role::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_role::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_role::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_role::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_role::guid
        > primary_guid;
        return &primary_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_role::guid>().GetData()
        );
    }
    std::map<std::string, SFieldList<>*> UniqueKey(void) override
    {
        std::map<std::string, SFieldList<>*> unique_map;
        static SFieldList<
        mt_role::sn
        > unique_sn;
        unique_map[u8"sn"] = &unique_sn;
        static SFieldList<
        mt_role::name
        > unique_name;
        unique_map[u8"name"] = &unique_name;
        static SFieldList<
        mt_role::unid
        > unique_unid;
        unique_map[u8"unid"] = &unique_unid;
        return unique_map;
    }
    std::map<std::string, SFieldList<>*> IndexKey(void) override
    {
        std::map<std::string, SFieldList<>*> index_map;
        static SFieldList<
        mt_role::uid,
        mt_role::sid
        > index_uid_sid;
        index_map[u8"uid_sid"] = &index_uid_sid;
        static SFieldList<
        mt_role::state
        > index_state;
        index_map[u8"state"] = &index_state;
        return index_map;
    }
    
    inline DBTableCache<mt_role>& Cache(void) { return m_cache; }
    DBTableCache<mt_role> m_cache;
};
