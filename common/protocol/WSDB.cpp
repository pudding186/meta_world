#include <new>

#include "WSDB.hpp"

const char* LoadUserReq::Handler::m_key_list[2] = {
    u8"uid",
    u8"sid"
};
const char* LoadUserReq::Handler::m_alias_list[2] = {
    u8"uid",
    u8"sid"
};
const std::bitset<2> LoadUserReq::Handler::template_assigned_bitset { "11" };

const char* LoadUserAck::Handler::m_key_list[3] = {
    u8"userdata",
    u8"errcode",
    u8"errmsg"
};
const char* LoadUserAck::Handler::m_alias_list[3] = {
    u8"userdata",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> LoadUserAck::Handler::template_assigned_bitset { "111" };

const char* UpdateUserNtf::Handler::m_key_list[1] = {
    u8"userdata"
};
const char* UpdateUserNtf::Handler::m_alias_list[1] = {
    u8"userdata"
};
const std::bitset<1> UpdateUserNtf::Handler::template_assigned_bitset { "1" };

const char* PlayerBriefsReq::Handler::m_key_list[2] = {
    u8"uid",
    u8"sid"
};
const char* PlayerBriefsReq::Handler::m_alias_list[2] = {
    u8"uid",
    u8"sid"
};
const std::bitset<2> PlayerBriefsReq::Handler::template_assigned_bitset { "11" };

const char* PlayerBriefsAck::Handler::m_key_list[5] = {
    u8"uid",
    u8"sid",
    u8"briefs",
    u8"errcode",
    u8"errmsg"
};
const char* PlayerBriefsAck::Handler::m_alias_list[5] = {
    u8"uid",
    u8"sid",
    u8"briefs",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<5> PlayerBriefsAck::Handler::template_assigned_bitset { "11111" };

const char* NewPlayerReq::Handler::m_key_list[1] = {
    u8"playerbrief"
};
const char* NewPlayerReq::Handler::m_alias_list[1] = {
    u8"playerbrief"
};
const std::bitset<1> NewPlayerReq::Handler::template_assigned_bitset { "1" };

const char* NewPlayerAck::Handler::m_key_list[3] = {
    u8"playerbrief",
    u8"errcode",
    u8"errmsg"
};
const char* NewPlayerAck::Handler::m_alias_list[3] = {
    u8"playerbrief",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> NewPlayerAck::Handler::template_assigned_bitset { "111" };

const char* UpdateBriefNtf::Handler::m_key_list[3] = {
    u8"uid",
    u8"sid",
    u8"brief"
};
const char* UpdateBriefNtf::Handler::m_alias_list[3] = {
    u8"uid",
    u8"sid",
    u8"brief"
};
const std::bitset<3> UpdateBriefNtf::Handler::template_assigned_bitset { "111" };

const char* UpdatePlayerNtf::Handler::m_key_list[1] = {
    u8"playerdata"
};
const char* UpdatePlayerNtf::Handler::m_alias_list[1] = {
    u8"playerdata"
};
const std::bitset<1> UpdatePlayerNtf::Handler::template_assigned_bitset { "1" };

const char* LoadPlayerReq::Handler::m_key_list[3] = {
    u8"guid",
    u8"uid",
    u8"sid"
};
const char* LoadPlayerReq::Handler::m_alias_list[3] = {
    u8"guid",
    u8"uid",
    u8"sid"
};
const std::bitset<3> LoadPlayerReq::Handler::template_assigned_bitset { "111" };

const char* LoadPlayerAck::Handler::m_key_list[3] = {
    u8"playerdata",
    u8"errcode",
    u8"errmsg"
};
const char* LoadPlayerAck::Handler::m_alias_list[3] = {
    u8"playerdata",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> LoadPlayerAck::Handler::template_assigned_bitset { "111" };

const char* BillInNtf::Handler::m_key_list[5] = {
    u8"uid",
    u8"sid",
    u8"bill_id",
    u8"billin_yb",
    u8"desc"
};
const char* BillInNtf::Handler::m_alias_list[5] = {
    u8"uid",
    u8"sid",
    u8"bill_id",
    u8"billin_yb",
    u8"desc"
};
const std::bitset<5> BillInNtf::Handler::template_assigned_bitset { "11111" };

const char* LoadForbidAck::Handler::m_key_list[3] = {
    u8"forbids",
    u8"errcode",
    u8"errmsg"
};
const char* LoadForbidAck::Handler::m_alias_list[3] = {
    u8"forbids",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> LoadForbidAck::Handler::template_assigned_bitset { "111" };

const char* AddForbidNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddForbidNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddForbidNtf::Handler::template_assigned_bitset { "1" };

const char* DelForbidNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* DelForbidNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> DelForbidNtf::Handler::template_assigned_bitset { "1" };

const char* RefreshPlayerCacheNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* RefreshPlayerCacheNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> RefreshPlayerCacheNtf::Handler::template_assigned_bitset { "1" };

const char* LoadForbidTalkAck::Handler::m_key_list[3] = {
    u8"forbids",
    u8"errcode",
    u8"errmsg"
};
const char* LoadForbidTalkAck::Handler::m_alias_list[3] = {
    u8"forbids",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> LoadForbidTalkAck::Handler::template_assigned_bitset { "111" };

