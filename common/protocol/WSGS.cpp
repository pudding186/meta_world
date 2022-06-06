#include <new>

#include "WSGS.hpp"

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

const char* ReloginGSNtf::Handler::m_key_list[2] = {
    u8"gate",
    u8"guid"
};
const char* ReloginGSNtf::Handler::m_alias_list[2] = {
    u8"gate",
    u8"guid"
};
const std::bitset<2> ReloginGSNtf::Handler::template_assigned_bitset { "11" };

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

const char* OfflineNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* OfflineNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> OfflineNtf::Handler::template_assigned_bitset { "1" };

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

const char* SavePlayerNtf::Handler::m_key_list[1] = {
    u8"playerdata"
};
const char* SavePlayerNtf::Handler::m_alias_list[1] = {
    u8"playerdata"
};
const std::bitset<1> SavePlayerNtf::Handler::template_assigned_bitset { "1" };

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

const char* ForceKickNtf::Handler::m_key_list[2] = {
    u8"uid",
    u8"sid"
};
const char* ForceKickNtf::Handler::m_alias_list[2] = {
    u8"uid",
    u8"sid"
};
const std::bitset<2> ForceKickNtf::Handler::template_assigned_bitset { "11" };

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

const char* ForbidNtf::Handler::m_key_list[2] = {
    u8"forbid",
    u8"data"
};
const char* ForbidNtf::Handler::m_alias_list[2] = {
    u8"forbid",
    u8"data"
};
const std::bitset<2> ForbidNtf::Handler::template_assigned_bitset { "11" };

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

const char* NewContactInfoDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* NewContactInfoDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> NewContactInfoDataNtf::Handler::template_assigned_bitset { "1" };

