#pragma once

#include "db_table.hpp"
#include "db_cache.hpp"
class mt_pasture
    :public IDBTable
{
public:
    class pet_guid
        :public FieldUINT64
    {
    public:
        pet_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"pet_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"宠物GUID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = pet_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = pet_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = pet_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = pet_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(pet_guid, 1, *this); }
    };
    class owner_guid
        :public FieldUINT64
    {
    public:
        owner_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"owner_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"拥有者GUID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = owner_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = owner_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = owner_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = owner_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(owner_guid, 1, *this); }
    };
    class map
        :public FieldUINT32
    {
    public:
        map(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"map");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"地图");
        const std::string& GetColumnName(void) const override { static const std::string column_name = map::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = map::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = map::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = map::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(map, 1, *this); }
    };
    class x
        :public FieldUINT32
    {
    public:
        x(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"x");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"x坐标");
        const std::string& GetColumnName(void) const override { static const std::string column_name = x::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = x::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = x::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = x::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(x, 1, *this); }
    };
    class y
        :public FieldUINT32
    {
    public:
        y(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"y");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"y坐标");
        const std::string& GetColumnName(void) const override { static const std::string column_name = y::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = y::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = y::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = y::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(y, 1, *this); }
    };
    class placed_time
        :public FieldUINT32
    {
    public:
        placed_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"placed_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"放置时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = placed_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = placed_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = placed_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = placed_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(placed_time, 1, *this); }
    };
    class placed_status
        :public FieldUINT32
    {
    public:
        placed_status(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"placed_status");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"放置状态");
        const std::string& GetColumnName(void) const override { static const std::string column_name = placed_status::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = placed_status::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = placed_status::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = placed_status::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(placed_status, 1, *this); }
    };
    class replaced_time
        :public FieldUINT32
    {
    public:
        replaced_time(unsigned int data = 0) :FieldUINT32(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"replaced_time");
        static constexpr auto ColumnType = FieldUINT32::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT32::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"替换时间");
        const std::string& GetColumnName(void) const override { static const std::string column_name = replaced_time::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = replaced_time::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = replaced_time::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = replaced_time::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(replaced_time, 1, *this); }
    };
    class replaced_player_guid
        :public FieldUINT64
    {
    public:
        replaced_player_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"replaced_player_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"替换的玩家GUID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = replaced_player_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = replaced_player_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = replaced_player_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = replaced_player_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(replaced_player_guid, 1, *this); }
    };
    class replaced_player_name
        :public FieldVarChar<255>
    {
    public:
        replaced_player_name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        replaced_player_name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"replaced_player_name");
        static constexpr auto ColumnType = FieldVarChar<255>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<255>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"替换的玩家名字");
        const std::string& GetColumnName(void) const override { static const std::string column_name = replaced_player_name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = replaced_player_name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = replaced_player_name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = replaced_player_name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(replaced_player_name, 1, *this); }
    };
    class replaced_pet_guid
        :public FieldUINT64
    {
    public:
        replaced_pet_guid(unsigned long long data = 0) :FieldUINT64(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"replaced_pet_guid");
        static constexpr auto ColumnType = FieldUINT64::ColumnType;
        static constexpr auto ColumnExtra = FieldUINT64::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"替换的宠物GUID");
        const std::string& GetColumnName(void) const override { static const std::string column_name = replaced_pet_guid::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = replaced_pet_guid::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = replaced_pet_guid::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = replaced_pet_guid::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(replaced_pet_guid, 1, *this); }
    };
    class replaced_pet_name
        :public FieldVarChar<255>
    {
    public:
        replaced_pet_name(const std::string& data = "") :FieldVarChar(data) {}
        template<size_t N>
        replaced_pet_name(const char(&data)[N]) : FieldVarChar(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"replaced_pet_name");
        static constexpr auto ColumnType = FieldVarChar<255>::ColumnType;
        static constexpr auto ColumnExtra = FieldVarChar<255>::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"替换的宠物名字");
        const std::string& GetColumnName(void) const override { static const std::string column_name = replaced_pet_name::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = replaced_pet_name::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = replaced_pet_name::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = replaced_pet_name::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(replaced_pet_name, 1, *this); }
    };
    class pet_data
        :public FieldLongText
    {
    public:
        pet_data(const std::string& data = "") :FieldLongText(data) {}
        template<size_t N>
        pet_data(const char(&data)[N]) : FieldLongText(data) {}
        static constexpr auto ColumnName = static_string_literal(u8"pet_data");
        static constexpr auto ColumnType = FieldLongText::ColumnType;
        static constexpr auto ColumnExtra = FieldLongText::ColumnExtra;
        static constexpr auto ColumnComment = static_string_literal(u8"宠物数据");
        const std::string& GetColumnName(void) const override { static const std::string column_name = pet_data::ColumnName.s; return column_name; }
        const std::string& GetColumnType(void) const override { static const std::string column_type = pet_data::ColumnType.s; return column_type; }
        const std::string& GetColumnExtra(void) const override { static const std::string column_extra = pet_data::ColumnExtra.s; return column_extra; }
        const std::string& GetColumnComment(void) const override { static const std::string column_comment = pet_data::ColumnComment.s; return column_comment; }
        IField* Clone(void) const override { return S_NEW(pet_data, 1, *this); }
    };
    using Row = SFieldList<
        mt_pasture::pet_guid,
        mt_pasture::owner_guid,
        mt_pasture::map,
        mt_pasture::x,
        mt_pasture::y,
        mt_pasture::placed_time,
        mt_pasture::placed_status,
        mt_pasture::replaced_time,
        mt_pasture::replaced_player_guid,
        mt_pasture::replaced_player_name,
        mt_pasture::replaced_pet_guid,
        mt_pasture::replaced_pet_name,
        mt_pasture::pet_data
    >;
    using PriKey = SFieldList<
        mt_pasture::pet_guid
    >;
    mt_pasture(HMYSQLCONNECTION connection = nullptr)
        :IDBTable(connection), m_cache(*this){}

    static const std::string& TableName(void)
    {
        static std::string table_name = u8"mt_pasture";
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

    const std::string& GetTableName(void) const override { return mt_pasture::TableName(); }
    const std::string& GetTableExtra(void) const override { return mt_pasture::TableExtra(); }
    const std::string& GetTableEngine(void) const override { return mt_pasture::TableEngine(); }
    const std::string& GetTableCharset(void) const override { return mt_pasture::TableCharset(); }
    const std::string& GetTableRowFormat(void) const override { return mt_pasture::TableRowFormat(); }
    SFieldList<>* FieldList(void) override
    {
        static Row field_list;
        return &field_list;
    }
    SFieldList<>* PrimaryKey(void) override
    {
        static SFieldList<
        mt_pasture::pet_guid
        > primary_pet_guid;
        return &primary_pet_guid;
    }
    static PriKey PriKeyFromRow(const Row& row)
    {
        return PriKey(
        row.Field<mt_pasture::pet_guid>().GetData()
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
    
    inline DBTableCache<mt_pasture>& Cache(void) { return m_cache; }
    DBTableCache<mt_pasture> m_cache;
};
