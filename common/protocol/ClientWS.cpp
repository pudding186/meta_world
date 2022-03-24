#include <new>

#include "ClientWS.hpp"

template<>
const unsigned short Protocol<RandKeyNtf>::module_id = 101;
template<>
const unsigned short Protocol<RandKeyNtf>::protocol_id = 1;

const char* RandKeyNtf::Handler::m_key_list[1] = {
    u8"code_content"
};
const char* RandKeyNtf::Handler::m_alias_list[1] = {
    u8"code_content"
};
const std::bitset<1> RandKeyNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<LoginReq>::module_id = 101;
template<>
const unsigned short Protocol<LoginReq>::protocol_id = 2;

const char* LoginReq::Handler::m_key_list[12] = {
    u8"sid",
    u8"username",
    u8"ip",
    u8"pwd_content",
    u8"hwid",
    u8"client_type",
    u8"client_version",
    u8"client_protocol_md5",
    u8"longitude",
    u8"latitude",
    u8"session",
    u8"suid"
};
const char* LoginReq::Handler::m_alias_list[12] = {
    u8"sid",
    u8"username",
    u8"ip",
    u8"pwd_content",
    u8"hwid",
    u8"client_type",
    u8"client_version",
    u8"client_protocol_md5",
    u8"longitude",
    u8"latitude",
    u8"session",
    u8"suid"
};
const std::bitset<12> LoginReq::Handler::template_assigned_bitset { "111111111111" };

template<>
const unsigned short Protocol<LoginAck>::module_id = 101;
template<>
const unsigned short Protocol<LoginAck>::protocol_id = 3;

const char* LoginAck::Handler::m_key_list[6] = {
    u8"uid",
    u8"guid",
    u8"suid",
    u8"rid",
    u8"errcode",
    u8"errmsg"
};
const char* LoginAck::Handler::m_alias_list[6] = {
    u8"uid",
    u8"guid",
    u8"suid",
    u8"rid",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<6> LoginAck::Handler::template_assigned_bitset { "111111" };

template<>
const unsigned short Protocol<ReloginReq>::module_id = 101;
template<>
const unsigned short Protocol<ReloginReq>::protocol_id = 4;

const char* ReloginReq::Handler::m_key_list[11] = {
    u8"sid",
    u8"uid",
    u8"ip",
    u8"hwid",
    u8"client_type",
    u8"client_version",
    u8"client_protocol_md5",
    u8"longitude",
    u8"latitude",
    u8"session",
    u8"suid"
};
const char* ReloginReq::Handler::m_alias_list[11] = {
    u8"sid",
    u8"uid",
    u8"ip",
    u8"hwid",
    u8"client_type",
    u8"client_version",
    u8"client_protocol_md5",
    u8"longitude",
    u8"latitude",
    u8"session",
    u8"suid"
};
const std::bitset<11> ReloginReq::Handler::template_assigned_bitset { "11111111111" };

template<>
const unsigned short Protocol<ReloginAck>::module_id = 101;
template<>
const unsigned short Protocol<ReloginAck>::protocol_id = 5;

const char* ReloginAck::Handler::m_key_list[6] = {
    u8"guid",
    u8"rid",
    u8"gsindex",
    u8"arrayid",
    u8"errcode",
    u8"errmsg"
};
const char* ReloginAck::Handler::m_alias_list[6] = {
    u8"guid",
    u8"rid",
    u8"gsindex",
    u8"arrayid",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<6> ReloginAck::Handler::template_assigned_bitset { "111111" };

template<>
const unsigned short Protocol<LogoutReq>::module_id = 101;
template<>
const unsigned short Protocol<LogoutReq>::protocol_id = 6;

const char* LogoutReq::Handler::m_key_list[1] = {
    u8"type"
};
const char* LogoutReq::Handler::m_alias_list[1] = {
    u8"type"
};
const std::bitset<1> LogoutReq::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<LogoutAck>::module_id = 101;
template<>
const unsigned short Protocol<LogoutAck>::protocol_id = 7;

const char* LogoutAck::Handler::m_key_list[3] = {
    u8"type",
    u8"errcode",
    u8"errmsg"
};
const char* LogoutAck::Handler::m_alias_list[3] = {
    u8"type",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> LogoutAck::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<KickNtf>::module_id = 101;
template<>
const unsigned short Protocol<KickNtf>::protocol_id = 8;

const char* KickNtf::Handler::m_key_list[2] = {
    u8"errcode",
    u8"errmsg"
};
const char* KickNtf::Handler::m_alias_list[2] = {
    u8"errcode",
    u8"errmsg"
};
const std::bitset<2> KickNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<PlayerListReq>::module_id = 101;
template<>
const unsigned short Protocol<PlayerListReq>::protocol_id = 9;

template<>
const unsigned short Protocol<PlayerListAck>::module_id = 101;
template<>
const unsigned short Protocol<PlayerListAck>::protocol_id = 10;

const char* PlayerListAck::Handler::m_key_list[4] = {
    u8"lastplayer",
    u8"briefs",
    u8"errcode",
    u8"errmsg"
};
const char* PlayerListAck::Handler::m_alias_list[4] = {
    u8"lastplayer",
    u8"briefs",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> PlayerListAck::Handler::template_assigned_bitset { "1111" };

template<>
const unsigned short Protocol<CreatePlayerReq>::module_id = 101;
template<>
const unsigned short Protocol<CreatePlayerReq>::protocol_id = 11;

const char* CreatePlayerReq::Handler::m_key_list[1] = {
    u8"playerbrief"
};
const char* CreatePlayerReq::Handler::m_alias_list[1] = {
    u8"playerbrief"
};
const std::bitset<1> CreatePlayerReq::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<CreatePlayerAck>::module_id = 101;
template<>
const unsigned short Protocol<CreatePlayerAck>::protocol_id = 12;

const char* CreatePlayerAck::Handler::m_key_list[3] = {
    u8"playerbrief",
    u8"errcode",
    u8"errmsg"
};
const char* CreatePlayerAck::Handler::m_alias_list[3] = {
    u8"playerbrief",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> CreatePlayerAck::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<DestroyPlayerReq>::module_id = 101;
template<>
const unsigned short Protocol<DestroyPlayerReq>::protocol_id = 13;

const char* DestroyPlayerReq::Handler::m_key_list[1] = {
    u8"guid"
};
const char* DestroyPlayerReq::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> DestroyPlayerReq::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<DestroyPlayerAck>::module_id = 101;
template<>
const unsigned short Protocol<DestroyPlayerAck>::protocol_id = 14;

const char* DestroyPlayerAck::Handler::m_key_list[3] = {
    u8"guid",
    u8"errcode",
    u8"errmsg"
};
const char* DestroyPlayerAck::Handler::m_alias_list[3] = {
    u8"guid",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> DestroyPlayerAck::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<RestorePlayerReq>::module_id = 101;
template<>
const unsigned short Protocol<RestorePlayerReq>::protocol_id = 15;

const char* RestorePlayerReq::Handler::m_key_list[1] = {
    u8"guid"
};
const char* RestorePlayerReq::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> RestorePlayerReq::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<RestorePlayerAck>::module_id = 101;
template<>
const unsigned short Protocol<RestorePlayerAck>::protocol_id = 16;

const char* RestorePlayerAck::Handler::m_key_list[3] = {
    u8"guid",
    u8"errcode",
    u8"errmsg"
};
const char* RestorePlayerAck::Handler::m_alias_list[3] = {
    u8"guid",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> RestorePlayerAck::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<EnterGSReq>::module_id = 101;
template<>
const unsigned short Protocol<EnterGSReq>::protocol_id = 17;

const char* EnterGSReq::Handler::m_key_list[1] = {
    u8"guid"
};
const char* EnterGSReq::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> EnterGSReq::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<EnterGSAck>::module_id = 101;
template<>
const unsigned short Protocol<EnterGSAck>::protocol_id = 18;

const char* EnterGSAck::Handler::m_key_list[5] = {
    u8"guid",
    u8"gsindex",
    u8"arrayid",
    u8"errcode",
    u8"errmsg"
};
const char* EnterGSAck::Handler::m_alias_list[5] = {
    u8"guid",
    u8"gsindex",
    u8"arrayid",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<5> EnterGSAck::Handler::template_assigned_bitset { "11111" };

template<>
const unsigned short Protocol<TrdLoginReq>::module_id = 101;
template<>
const unsigned short Protocol<TrdLoginReq>::protocol_id = 19;

const char* TrdLoginReq::Handler::m_key_list[13] = {
    u8"sid",
    u8"username",
    u8"platform",
    u8"token",
    u8"hwid",
    u8"ip",
    u8"client_type",
    u8"client_version",
    u8"client_protocol_md5",
    u8"longitude",
    u8"latitude",
    u8"session",
    u8"suid"
};
const char* TrdLoginReq::Handler::m_alias_list[13] = {
    u8"sid",
    u8"username",
    u8"platform",
    u8"token",
    u8"hwid",
    u8"ip",
    u8"client_type",
    u8"client_version",
    u8"client_protocol_md5",
    u8"longitude",
    u8"latitude",
    u8"session",
    u8"suid"
};
const std::bitset<13> TrdLoginReq::Handler::template_assigned_bitset { "1111111111111" };

template<>
const unsigned short Protocol<GetWSTimestampReq>::module_id = 101;
template<>
const unsigned short Protocol<GetWSTimestampReq>::protocol_id = 20;

template<>
const unsigned short Protocol<GetWSTimestampAck>::module_id = 101;
template<>
const unsigned short Protocol<GetWSTimestampAck>::protocol_id = 21;

const char* GetWSTimestampAck::Handler::m_key_list[2] = {
    u8"now",
    u8"zone"
};
const char* GetWSTimestampAck::Handler::m_alias_list[2] = {
    u8"now",
    u8"zone"
};
const std::bitset<2> GetWSTimestampAck::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<RealnameInfoNtf>::module_id = 101;
template<>
const unsigned short Protocol<RealnameInfoNtf>::protocol_id = 22;

const char* RealnameInfoNtf::Handler::m_key_list[3] = {
    u8"realname_status",
    u8"auth",
    u8"realname_token"
};
const char* RealnameInfoNtf::Handler::m_alias_list[3] = {
    u8"realname_status",
    u8"auth",
    u8"realname_token"
};
const std::bitset<3> RealnameInfoNtf::Handler::template_assigned_bitset { "111" };

