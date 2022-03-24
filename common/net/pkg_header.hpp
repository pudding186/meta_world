#pragma once
//#include "type_def.hpp"
#include <stdint.h>

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

using ClientInfo = struct ClientInfo
{
    uint32_t user_id;
    uint32_t user_ref;
};

using C2SPkgHead = struct stC2SPkgHeader
{
    ClientInfo info;
};

using S2CPkgHead = struct stS2CPkgHead
{
    uint32_t client_size;
};

#pragma pack(pop)
