#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_auction_object
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
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = id::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = id::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = id::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = id::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(id, 1, *this); }
    };
    class type
        :public FieldUINT8
    {
    public:
        type(unsigned char data = 0) :FieldUINT8(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"type");
        static constexpr auto ColumnType = FieldUINT8::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT8::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = type::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = type::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = type::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = type::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(type, 1, *this); }
    };
    class custom1
        :public FieldUINT16
    {
    public:
        custom1(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"custom1");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = custom1::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = custom1::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = custom1::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = custom1::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(custom1, 1, *this); }
    };
    class custom2
        :public FieldUINT16
    {
    public:
        custom2(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"custom2");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = custom2::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = custom2::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = custom2::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = custom2::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(custom2, 1, *this); }
    };
    class custom3
        :public FieldUINT16
    {
    public:
        custom3(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"custom3");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = custom3::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = custom3::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = custom3::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = custom3::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(custom3, 1, *this); }
    };
    class level
        :public FieldUINT16
    {
    public:
        level(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"level");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = level::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = level::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = level::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = level::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(level, 1, *this); }
    };
    class fight_value
        :public FieldUINT16
    {
    public:
        fight_value(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"fight_value");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = fight_value::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = fight_value::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = fight_value::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = fight_value::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(fight_value, 1, *this); }
    };
    class coin_type
        :public FieldUINT16
    {
    public:
        coin_type(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"coin_type");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = coin_type::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = coin_type::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = coin_type::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = coin_type::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(coin_type, 1, *this); }
    };
    class coin_value
        :public FieldINT64
    {
    public:
        coin_value(long long data = 0) :FieldINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"coin_value");
        static constexpr auto ColumnType = FieldINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = coin_value::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = coin_value::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = coin_value::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = coin_value::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(coin_value, 1, *this); }
    };
    class amount
        :public FieldUINT16
    {
    public:
        amount(unsigned short data = 0) :FieldUINT16(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"amount");
        static constexpr auto ColumnType = FieldUINT16::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT16::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = amount::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = amount::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = amount::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = amount::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(amount, 1, *this); }
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
    class puton_times
        :public FieldUINT32
    {
    public:
        puton_times(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"puton_times");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = puton_times::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = puton_times::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = puton_times::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = puton_times::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(puton_times, 1, *this); }
    };
    class pulloff_times
        :public FieldUINT32
    {
    public:
        pulloff_times(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"pulloff_times");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = pulloff_times::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = pulloff_times::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = pulloff_times::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = pulloff_times::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(pulloff_times, 1, *this); }
    };
    class seller_guid
        :public FieldUINT64
    {
    public:
        seller_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"seller_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = seller_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = seller_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = seller_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = seller_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(seller_guid, 1, *this); }
    };
    class seller_name
        :public FieldVarChar<32>
    {
    public:
        seller_name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        seller_name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"seller_name");
        static constexpr auto ColumnType = FieldVarChar<32>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<32>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = seller_name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = seller_name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = seller_name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = seller_name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(seller_name, 1, *this); }
    };
    class data
        :public FieldLongText
    {
    public:
        data(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        data(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"data");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"");
        const std::string& GetColumnName(void) const override { static const std::string column_name = data::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = data::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = data::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = data::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(data, 1, *this); }
    };
    using Row = SFieldList<
        mt_auction_object::guid,
        mt_auction_object::id,
        mt_auction_object::type,
        mt_auction_object::custom1,
        mt_auction_object::custom2,
        mt_auction_object::custom3,
        mt_auction_object::level,
        mt_auction_object::fight_value,
        mt_auction_object::coin_type,
        mt_auction_object::coin_value,
        mt_auction_object::amount,
        mt_auction_object::state,
        mt_auction_object::puton_times,
        mt_auction_object::pulloff_times,
        mt_auction_object::seller_guid,
        mt_auction_object::seller_name,
        mt_auction_object::data
    >;
    using PriKey = SFieldList<
        mt_auction_object::guid
    >;
    mt_auction_object(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_auction_object";
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

    const std::string& GetTableName(void) const override { return mt_auction_object::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_auction_object::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_auction_object::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_auction_object::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_auction_object::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_auction_object::guid
        > primary_guid;
        return &primary_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_auction_object::guid>().GetData()
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
    
    inline DBTableCache<mt_auction_object>& Cache(void) { return m_cache; }
    DBTableCache<mt_auction_object> m_cache;
};
