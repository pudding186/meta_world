#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_mail
    :public IDBTable
{
public:
    class mail_guid
        :public FieldUINT64
    {
    public:
        mail_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"mail_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = mail_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = mail_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = mail_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = mail_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(mail_guid, 1, *this); }
    };
    class mail_type
        :public FieldUINT32
    {
    public:
        mail_type(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"mail_type");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = mail_type::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = mail_type::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = mail_type::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = mail_type::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(mail_type, 1, *this); }
    };
    class title
        :public FieldVarChar<1024>
    {
    public:
        title(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        title(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"title");
        static constexpr auto ColumnType = FieldVarChar<1024>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<1024>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = title::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = title::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = title::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = title::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(title, 1, *this); }
    };
    class recv_guid
        :public FieldUINT64
    {
    public:
        recv_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"recv_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = recv_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = recv_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = recv_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = recv_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(recv_guid, 1, *this); }
    };
    class send_guid
        :public FieldUINT64
    {
    public:
        send_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"send_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = send_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = send_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = send_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = send_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(send_guid, 1, *this); }
    };
    class send_name
        :public FieldVarChar<32>
    {
    public:
        send_name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        send_name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"send_name");
        static constexpr auto ColumnType = FieldVarChar<32>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<32>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = send_name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = send_name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = send_name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = send_name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(send_name, 1, *this); }
    };
    class send_time
        :public FieldUINT32
    {
    public:
        send_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"send_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = send_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = send_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = send_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = send_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(send_time, 1, *this); }
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
    class content
        :public FieldLongText
    {
    public:
        content(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        content(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"content");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = content::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = content::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = content::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = content::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(content, 1, *this); }
    };
    class attrs
        :public FieldLongText
    {
    public:
        attrs(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        attrs(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"attrs");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = attrs::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = attrs::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = attrs::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = attrs::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(attrs, 1, *this); }
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
    class guids
        :public FieldLongText
    {
    public:
        guids(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        guids(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"guids");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = guids::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = guids::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = guids::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = guids::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(guids, 1, *this); }
    };
    using Row = SFieldList<
        mt_mail::mail_guid,
        mt_mail::mail_type,
        mt_mail::title,
        mt_mail::recv_guid,
        mt_mail::send_guid,
        mt_mail::send_name,
        mt_mail::send_time,
        mt_mail::status,
        mt_mail::content,
        mt_mail::attrs,
        mt_mail::items,
        mt_mail::pets,
        mt_mail::guids
    >;
    using PriKey = SFieldList<
        mt_mail::mail_guid
    >;
    mt_mail(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_mail";
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

    const std::string& GetTableName(void) const override { return mt_mail::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_mail::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_mail::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_mail::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_mail::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_mail::mail_guid
        > primary_mail_guid;
        return &primary_mail_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_mail::mail_guid>().GetData()
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
        static SFieldList<
        mt_mail::recv_guid
        > index_recv_guid;
        index_map[u8"recv_guid"] = &index_recv_guid;
        return index_map;
    }
    
    inline DBTableCache<mt_mail>& Cache(void) { return m_cache; }
    DBTableCache<mt_mail> m_cache;
};
