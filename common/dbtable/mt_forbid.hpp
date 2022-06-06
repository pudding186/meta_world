#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_forbid
    :public IDBTable
{
public:
    class sid
        :public FieldUINT8
    {
    public:
        sid(unsigned char data = 0) :FieldUINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"sid");
        static constexpr auto ColumnType = FieldUINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"子区id");
        const std::string& GetColumnName(void) const override { static const std::string column_name = sid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = sid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = sid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = sid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(sid, 1, *this); }
    };
    class keyword
        :public FieldVarChar<60>
    {
    public:
        keyword(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        keyword(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"keyword");
        static constexpr auto ColumnType = FieldVarChar<60>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<60>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"关键字");
        const std::string& GetColumnName(void) const override { static const std::string column_name = keyword::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = keyword::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = keyword::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = keyword::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(keyword, 1, *this); }
    };
    class keytype
        :public FieldUINT8
    {
    public:
        keytype(unsigned char data = 0) :FieldUINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"keytype");
        static constexpr auto ColumnType = FieldUINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"关键字类型");
        const std::string& GetColumnName(void) const override { static const std::string column_name = keytype::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = keytype::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = keytype::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = keytype::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(keytype, 1, *this); }
    };
    class fbdtype
        :public FieldUINT8
    {
    public:
        fbdtype(unsigned char data = 0) :FieldUINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"fbdtype");
        static constexpr auto ColumnType = FieldUINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"禁止类型");
        const std::string& GetColumnName(void) const override { static const std::string column_name = fbdtype::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = fbdtype::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = fbdtype::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = fbdtype::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(fbdtype, 1, *this); }
    };
    class begin
        :public FieldINT64
    {
    public:
        begin(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"begin");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"起始时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = begin::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = begin::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = begin::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = begin::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(begin, 1, *this); }
    };
    class end
        :public FieldINT64
    {
    public:
        end(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"end");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"结束时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = end::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = end::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = end::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = end::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(end, 1, *this); }
    };
    class desc
        :public FieldVarChar<60>
    {
    public:
        desc(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        desc(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"desc");
        static constexpr auto ColumnType = FieldVarChar<60>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<60>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"描述");
        const std::string& GetColumnName(void) const override { static const std::string column_name = desc::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = desc::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = desc::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = desc::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(desc, 1, *this); }
    };
    using Row = SFieldList<
        mt_forbid::sid,
        mt_forbid::keyword,
        mt_forbid::keytype,
        mt_forbid::fbdtype,
        mt_forbid::begin,
        mt_forbid::end,
        mt_forbid::desc
    >;
    using PriKey = SFieldList<
        mt_forbid::sid,
        mt_forbid::keyword,
        mt_forbid::keytype,
        mt_forbid::fbdtype
    >;
    mt_forbid(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_forbid";
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

    const std::string& GetTableName(void) const override { return mt_forbid::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_forbid::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_forbid::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_forbid::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_forbid::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_forbid::sid,
        mt_forbid::keyword,
        mt_forbid::keytype,
        mt_forbid::fbdtype
        > primary_sid_keyword_keytype_fbdtype;
        return &primary_sid_keyword_keytype_fbdtype;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_forbid::sid>().GetData(),
        row.Field<mt_forbid::keyword>().GetData(),
        row.Field<mt_forbid::keytype>().GetData(),
        row.Field<mt_forbid::fbdtype>().GetData()
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
    
    inline DBTableCache<mt_forbid>& Cache(void) { return m_cache; }
    DBTableCache<mt_forbid> m_cache;
};
