#pragma once
#include <stdint.h>

enum class HandError:uint16_t
{
    OK = 0,
    VERSION_NOT_MATCH,  //版本不匹配
    ZONE_CONNECT_FAIL,  //该区连接失败
    ZONE_NOT_EXSIT,     //该区不存在
    GEN_KEY_FAIL,       //生成握手密钥失败
    CHECK_KEY_FAIL      //验证五首密钥失败
};


#pragma pack(push, 4)

using ServerHandPkg = struct stServerHandPkg 
{
    uint32_t zone_id;
    uint16_t server_type;
    uint16_t server_index;
    uint16_t version;
    uint16_t extend_data_length;
};

using ServerHandRetPkg = struct stServerHandRetPkg
{
    uint32_t zone_id;
    uint16_t server_type;
    uint16_t server_index;
    uint16_t version;
    uint16_t extend_data_length;
};

using ClientHandPkg = struct stClientHandPkg
{
    uint32_t zone_id;
    uint16_t version;
    uint16_t extend_data_length;
};

using ClientHandRetPkg = struct stClientHandRetPkg
{
    uint16_t result;
    uint16_t extend_data_length;
};

using ServerPkgHeader = struct stServerPkgHeader
{
    using ClientInfo = struct stClientInfo
    {
        uint32_t user_id;
    };

    uint32_t array_size;
    ClientInfo client_info_array[1];
};

using ClinetPkgHeader = struct stClinetPkgHeader
{
    uint32_t user_id;
    uint32_t user_ref;
};


/// <summary>
/// Old Client
/// </summary>

using OldClientHandPkg = struct stOldClientHandPkg
{
    uint32_t version;
    uint32_t zone_id;
};

using OldClientHandRetPkg = struct stOldClientHandRetPkg
{
    uint32_t result;
    int32_t  over_load;
    int32_t  delay;
};

using ClientKeyReq1 = struct stClientKeyReq1 
{
    uint32_t version;
    int64_t seed1;
    int64_t seed2;
    int64_t check;
};

using ClientKeyAck1 = struct stClientKeyAck1
{
    uint32_t version;
    int64_t check;
};

#pragma pack(pop)
