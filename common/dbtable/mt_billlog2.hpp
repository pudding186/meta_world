#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_billlog2
    :public IDBTable
{
public:
    class bill_id
        :public FieldUINT64
    {
    public:
        bill_id(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"bill_id");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = bill_id::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = bill_id::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = bill_id::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = bill_id::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(bill_id, 1, *this); }
    };
    class user_id
        :public FieldUINT32
    {
    public:
        user_id(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"user_id");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = user_id::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = user_id::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = user_id::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = user_id::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(user_id, 1, *this); }
    };
    class sid
        :public FieldUINT32
    {
    public:
        sid(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"sid");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = sid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = sid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = sid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = sid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(sid, 1, *this); }
    };
    class added_yb
        :public FieldUINT64
    {
    public:
        added_yb(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"added_yb");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = added_yb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = added_yb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = added_yb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = added_yb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(added_yb, 1, *this); }
    };
    class award_yb
        :public FieldUINT64
    {
    public:
        award_yb(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"award_yb");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = award_yb::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = award_yb::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = award_yb::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = award_yb::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(award_yb, 1, *this); }
    };
    class time
        :public FieldUINT32
    {
    public:
        time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(time, 1, *this); }
    };
    class user_name
        :public FieldVarChar<32>
    {
    public:
        user_name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        user_name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"user_name");
        static constexpr auto ColumnType = FieldVarChar<32>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<32>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = user_name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = user_name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = user_name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = user_name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(user_name, 1, *this); }
    };
    class bill_desc
        :public FieldVarChar<1024>
    {
    public:
        bill_desc(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        bill_desc(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"bill_desc");
        static constexpr auto ColumnType = FieldVarChar<1024>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<1024>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = bill_desc::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = bill_desc::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = bill_desc::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = bill_desc::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(bill_desc, 1, *this); }
    };
    using Row = SFieldList<
        mt_billlog2::bill_id,
        mt_billlog2::user_id,
        mt_billlog2::sid,
        mt_billlog2::added_yb,
        mt_billlog2::award_yb,
        mt_billlog2::time,
        mt_billlog2::user_name,
        mt_billlog2::bill_desc
    >;
    using PriKey = SFieldList<
        mt_billlog2::bill_id
    >;
    mt_billlog2(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_billlog2";
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

    const std::string& GetTableName(void) const override { return mt_billlog2::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_billlog2::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_billlog2::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_billlog2::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_billlog2::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_billlog2::bill_id
        > primary_bill_id;
        return &primary_bill_id;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_billlog2::bill_id>().GetData()
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
    
    inline DBTableCache<mt_billlog2>& Cache(void) { return m_cache; }
    DBTableCache<mt_billlog2> m_cache;
};
