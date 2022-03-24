#include <new>

#include "WSDB.hpp"

template<>
const unsigned short Protocol<LoadUserReq>::module_id = 106;
template<>
const unsigned short Protocol<LoadUserReq>::protocol_id = 1;

const char* LoadUserReq::Handler::m_key_list[2] = {
    u8"uid",
    u8"sid"
};
const char* LoadUserReq::Handler::m_alias_list[2] = {
    u8"uid",
    u8"sid"
};
const std::bitset<2> LoadUserReq::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<LoadUserAck>::module_id = 106;
template<>
const unsigned short Protocol<LoadUserAck>::protocol_id = 2;

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

template<>
const unsigned short Protocol<UpdateUserNtf>::module_id = 106;
template<>
const unsigned short Protocol<UpdateUserNtf>::protocol_id = 3;

const char* UpdateUserNtf::Handler::m_key_list[1] = {
    u8"userdata"
};
const char* UpdateUserNtf::Handler::m_alias_list[1] = {
    u8"userdata"
};
const std::bitset<1> UpdateUserNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<PlayerBriefsReq>::module_id = 106;
template<>
const unsigned short Protocol<PlayerBriefsReq>::protocol_id = 4;

const char* PlayerBriefsReq::Handler::m_key_list[2] = {
    u8"uid",
    u8"sid"
};
const char* PlayerBriefsReq::Handler::m_alias_list[2] = {
    u8"uid",
    u8"sid"
};
const std::bitset<2> PlayerBriefsReq::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<PlayerBriefsAck>::module_id = 106;
template<>
const unsigned short Protocol<PlayerBriefsAck>::protocol_id = 5;

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

template<>
const unsigned short Protocol<NewPlayerReq>::module_id = 106;
template<>
const unsigned short Protocol<NewPlayerReq>::protocol_id = 6;

const char* NewPlayerReq::Handler::m_key_list[1] = {
    u8"playerbrief"
};
const char* NewPlayerReq::Handler::m_alias_list[1] = {
    u8"playerbrief"
};
const std::bitset<1> NewPlayerReq::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<NewPlayerAck>::module_id = 106;
template<>
const unsigned short Protocol<NewPlayerAck>::protocol_id = 7;

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

template<>
const unsigned short Protocol<UpdateBriefNtf>::module_id = 106;
template<>
const unsigned short Protocol<UpdateBriefNtf>::protocol_id = 8;

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

template<>
const unsigned short Protocol<UpdatePlayerNtf>::module_id = 106;
template<>
const unsigned short Protocol<UpdatePlayerNtf>::protocol_id = 9;

const char* UpdatePlayerNtf::Handler::m_key_list[1] = {
    u8"playerdata"
};
const char* UpdatePlayerNtf::Handler::m_alias_list[1] = {
    u8"playerdata"
};
const std::bitset<1> UpdatePlayerNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<LoadPlayerReq>::module_id = 106;
template<>
const unsigned short Protocol<LoadPlayerReq>::protocol_id = 10;

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

template<>
const unsigned short Protocol<LoadPlayerAck>::module_id = 106;
template<>
const unsigned short Protocol<LoadPlayerAck>::protocol_id = 11;

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

template<>
const unsigned short Protocol<BillInNtf>::module_id = 106;
template<>
const unsigned short Protocol<BillInNtf>::protocol_id = 12;

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

template<>
const unsigned short Protocol<LoadForbidReq>::module_id = 106;
template<>
const unsigned short Protocol<LoadForbidReq>::protocol_id = 13;

template<>
const unsigned short Protocol<LoadForbidAck>::module_id = 106;
template<>
const unsigned short Protocol<LoadForbidAck>::protocol_id = 14;

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

template<>
const unsigned short Protocol<AddForbidNtf>::module_id = 106;
template<>
const unsigned short Protocol<AddForbidNtf>::protocol_id = 15;

const char* AddForbidNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddForbidNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddForbidNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<DelForbidNtf>::module_id = 106;
template<>
const unsigned short Protocol<DelForbidNtf>::protocol_id = 16;

const char* DelForbidNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* DelForbidNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> DelForbidNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<RefreshPlayerCacheNtf>::module_id = 106;
template<>
const unsigned short Protocol<RefreshPlayerCacheNtf>::protocol_id = 17;

const char* RefreshPlayerCacheNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* RefreshPlayerCacheNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> RefreshPlayerCacheNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<LoadForbidTalkReq>::module_id = 106;
template<>
const unsigned short Protocol<LoadForbidTalkReq>::protocol_id = 18;

template<>
const unsigned short Protocol<LoadForbidTalkAck>::module_id = 106;
template<>
const unsigned short Protocol<LoadForbidTalkAck>::protocol_id = 19;

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

