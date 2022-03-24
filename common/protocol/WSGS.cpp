#include <new>

#include "WSGS.hpp"

template<>
const unsigned short Protocol<CanLoginGSReq>::module_id = 107;
template<>
const unsigned short Protocol<CanLoginGSReq>::protocol_id = 1;

const char* CanLoginGSReq::Handler::m_key_list[3] = {
    u8"uid",
    u8"sid",
    u8"guid"
};
const char* CanLoginGSReq::Handler::m_alias_list[3] = {
    u8"uid",
    u8"sid",
    u8"guid"
};
const std::bitset<3> CanLoginGSReq::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<CanLoginGSAck>::module_id = 107;
template<>
const unsigned short Protocol<CanLoginGSAck>::protocol_id = 2;

const char* CanLoginGSAck::Handler::m_key_list[5] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"errcode",
    u8"errmsg"
};
const char* CanLoginGSAck::Handler::m_alias_list[5] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<5> CanLoginGSAck::Handler::template_assigned_bitset { "11111" };

template<>
const unsigned short Protocol<LoginGSReq>::module_id = 107;
template<>
const unsigned short Protocol<LoginGSReq>::protocol_id = 3;

const char* LoginGSReq::Handler::m_key_list[4] = {
    u8"gate",
    u8"playerdata",
    u8"longitude",
    u8"latitude"
};
const char* LoginGSReq::Handler::m_alias_list[4] = {
    u8"gate",
    u8"playerdata",
    u8"longitude",
    u8"latitude"
};
const std::bitset<4> LoginGSReq::Handler::template_assigned_bitset { "1111" };

template<>
const unsigned short Protocol<LoginGSAck>::module_id = 107;
template<>
const unsigned short Protocol<LoginGSAck>::protocol_id = 4;

const char* LoginGSAck::Handler::m_key_list[6] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"arrayid",
    u8"errcode",
    u8"errmsg"
};
const char* LoginGSAck::Handler::m_alias_list[6] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"arrayid",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<6> LoginGSAck::Handler::template_assigned_bitset { "111111" };

template<>
const unsigned short Protocol<ReloginGSNtf>::module_id = 107;
template<>
const unsigned short Protocol<ReloginGSNtf>::protocol_id = 5;

const char* ReloginGSNtf::Handler::m_key_list[2] = {
    u8"gate",
    u8"guid"
};
const char* ReloginGSNtf::Handler::m_alias_list[2] = {
    u8"gate",
    u8"guid"
};
const std::bitset<2> ReloginGSNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<LogoutGSReq>::module_id = 107;
template<>
const unsigned short Protocol<LogoutGSReq>::protocol_id = 6;

const char* LogoutGSReq::Handler::m_key_list[4] = {
    u8"uid",
    u8"sid",
    u8"type",
    u8"guid"
};
const char* LogoutGSReq::Handler::m_alias_list[4] = {
    u8"uid",
    u8"sid",
    u8"type",
    u8"guid"
};
const std::bitset<4> LogoutGSReq::Handler::template_assigned_bitset { "1111" };

template<>
const unsigned short Protocol<LogoutGSAck>::module_id = 107;
template<>
const unsigned short Protocol<LogoutGSAck>::protocol_id = 7;

const char* LogoutGSAck::Handler::m_key_list[4] = {
    u8"playerdata",
    u8"type",
    u8"errcode",
    u8"errmsg"
};
const char* LogoutGSAck::Handler::m_alias_list[4] = {
    u8"playerdata",
    u8"type",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> LogoutGSAck::Handler::template_assigned_bitset { "1111" };

template<>
const unsigned short Protocol<OfflineNtf>::module_id = 107;
template<>
const unsigned short Protocol<OfflineNtf>::protocol_id = 8;

const char* OfflineNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* OfflineNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> OfflineNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<WrapperPkg>::module_id = 107;
template<>
const unsigned short Protocol<WrapperPkg>::protocol_id = 9;

const char* WrapperPkg::Handler::m_key_list[3] = {
    u8"uid",
    u8"sid",
    u8"wrapper"
};
const char* WrapperPkg::Handler::m_alias_list[3] = {
    u8"uid",
    u8"sid",
    u8"wrapper"
};
const std::bitset<3> WrapperPkg::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<SavePlayerNtf>::module_id = 107;
template<>
const unsigned short Protocol<SavePlayerNtf>::protocol_id = 10;

const char* SavePlayerNtf::Handler::m_key_list[1] = {
    u8"playerdata"
};
const char* SavePlayerNtf::Handler::m_alias_list[1] = {
    u8"playerdata"
};
const std::bitset<1> SavePlayerNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<BillInGSNtf>::module_id = 107;
template<>
const unsigned short Protocol<BillInGSNtf>::protocol_id = 11;

const char* BillInGSNtf::Handler::m_key_list[6] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"bill_id",
    u8"billin_yb",
    u8"desc"
};
const char* BillInGSNtf::Handler::m_alias_list[6] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"bill_id",
    u8"billin_yb",
    u8"desc"
};
const std::bitset<6> BillInGSNtf::Handler::template_assigned_bitset { "111111" };

template<>
const unsigned short Protocol<ForceKickNtf>::module_id = 107;
template<>
const unsigned short Protocol<ForceKickNtf>::protocol_id = 12;

const char* ForceKickNtf::Handler::m_key_list[2] = {
    u8"uid",
    u8"sid"
};
const char* ForceKickNtf::Handler::m_alias_list[2] = {
    u8"uid",
    u8"sid"
};
const std::bitset<2> ForceKickNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<GameInNtf>::module_id = 107;
template<>
const unsigned short Protocol<GameInNtf>::protocol_id = 13;

const char* GameInNtf::Handler::m_key_list[4] = {
    u8"uid",
    u8"sid",
    u8"gameinyb",
    u8"gameoutyb"
};
const char* GameInNtf::Handler::m_alias_list[4] = {
    u8"uid",
    u8"sid",
    u8"gameinyb",
    u8"gameoutyb"
};
const std::bitset<4> GameInNtf::Handler::template_assigned_bitset { "1111" };

template<>
const unsigned short Protocol<ForbidNtf>::module_id = 107;
template<>
const unsigned short Protocol<ForbidNtf>::protocol_id = 14;

const char* ForbidNtf::Handler::m_key_list[2] = {
    u8"forbid",
    u8"data"
};
const char* ForbidNtf::Handler::m_alias_list[2] = {
    u8"forbid",
    u8"data"
};
const std::bitset<2> ForbidNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<SetCustomVarNtf>::module_id = 107;
template<>
const unsigned short Protocol<SetCustomVarNtf>::protocol_id = 15;

const char* SetCustomVarNtf::Handler::m_key_list[3] = {
    u8"type",
    u8"scope",
    u8"key"
};
const char* SetCustomVarNtf::Handler::m_alias_list[3] = {
    u8"type",
    u8"scope",
    u8"key"
};
const std::bitset<3> SetCustomVarNtf::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<NewContactInfoDataNtf>::module_id = 107;
template<>
const unsigned short Protocol<NewContactInfoDataNtf>::protocol_id = 16;

const char* NewContactInfoDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* NewContactInfoDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> NewContactInfoDataNtf::Handler::template_assigned_bitset { "1" };

