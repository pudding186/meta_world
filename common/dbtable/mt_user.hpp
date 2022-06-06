#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_user
    :public IDBTable
{
public:
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
    class uname
        :public FieldVarChar<255>
    {
    public:
        uname(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        uname(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"uname");
        static constexpr auto ColumnType = FieldVarChar<255>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<255>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"用户名");
        const std::string& GetColumnName(void) const override { static const std::string column_name = uname::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = uname::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = uname::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = uname::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(uname, 1, *this); }
    };
    class billinyb
        :public FieldINT64
    {
    public:
        billinyb(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"billinyb");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"充值元宝");
        const std::string& GetColumnName(void) const override { static const std::string column_name = billinyb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = billinyb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = billinyb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = billinyb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(billinyb, 1, *this); }
    };
    class billoutyb
        :public FieldINT64
    {
    public:
        billoutyb(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"billoutyb");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"消费元宝");
        const std::string& GetColumnName(void) const override { static const std::string column_name = billoutyb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = billoutyb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = billoutyb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = billoutyb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(billoutyb, 1, *this); }
    };
    class gameinyb
        :public FieldINT64
    {
    public:
        gameinyb(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"gameinyb");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"游戏增加元宝");
        const std::string& GetColumnName(void) const override { static const std::string column_name = gameinyb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = gameinyb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = gameinyb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = gameinyb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(gameinyb, 1, *this); }
    };
    class gameoutyb
        :public FieldINT64
    {
    public:
        gameoutyb(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"gameoutyb");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"游戏消费元宝");
        const std::string& GetColumnName(void) const override { static const std::string column_name = gameoutyb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = gameoutyb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = gameoutyb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = gameoutyb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(gameoutyb, 1, *this); }
    };
    class createtime
        :public FieldUINT32
    {
    public:
        createtime(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"createtime");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"账号创建时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = createtime::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = createtime::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = createtime::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = createtime::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(createtime, 1, *this); }
    };
    class lastlogin
        :public FieldUINT32
    {
    public:
        lastlogin(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"lastlogin");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"账号上次登录时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = lastlogin::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = lastlogin::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = lastlogin::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = lastlogin::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(lastlogin, 1, *this); }
    };
    using Row = SFieldList<
        mt_user::uid,
        mt_user::sid,
        mt_user::uname,
        mt_user::billinyb,
        mt_user::billoutyb,
        mt_user::gameinyb,
        mt_user::gameoutyb,
        mt_user::createtime,
        mt_user::lastlogin
    >;
    using PriKey = SFieldList<
        mt_user::uid,
        mt_user::sid
    >;
    mt_user(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_user";
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

    const std::string& GetTableName(void) const override { return mt_user::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_user::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_user::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_user::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_user::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_user::uid,
        mt_user::sid
        > primary_uid_sid;
        return &primary_uid_sid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_user::uid>().GetData(),
        row.Field<mt_user::sid>().GetData()
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
    
    inline DBTableCache<mt_user>& Cache(void) { return m_cache; }
    DBTableCache<mt_user> m_cache;
};
