#include <new>

#include "Global.hpp"

const char* NewGSNtf::Handler::m_key_list[1] = {
    u8"index"
};
const char* NewGSNtf::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> NewGSNtf::Handler::template_assigned_bitset { "1" };

const char* TerminateNtf::Handler::m_key_list[1] = {
    u8"session"
};
const char* TerminateNtf::Handler::m_alias_list[1] = {
    u8"session"
};
const std::bitset<1> TerminateNtf::Handler::template_assigned_bitset { "1" };

const char* ErrorNtf::Handler::m_key_list[2] = {
    u8"errcode",
    u8"errmsg"
};
const char* ErrorNtf::Handler::m_alias_list[2] = {
    u8"errcode",
    u8"errmsg"
};
const std::bitset<2> ErrorNtf::Handler::template_assigned_bitset { "11" };

const char* ServerConfigNtf::Handler::m_key_list[4] = {
    u8"svr_id",
    u8"limit",
    u8"listen",
    u8"connect"
};
const char* ServerConfigNtf::Handler::m_alias_list[4] = {
    u8"svr_id",
    u8"limit",
    u8"listen",
    u8"connect"
};
const std::bitset<4> ServerConfigNtf::Handler::template_assigned_bitset { "1111" };

const char* DBWrapperPkg::Handler::m_key_list[1] = {
    u8"wrapper"
};
const char* DBWrapperPkg::Handler::m_alias_list[1] = {
    u8"wrapper"
};
const std::bitset<1> DBWrapperPkg::Handler::template_assigned_bitset { "1" };

const char* GSWrapperPkg::Handler::m_key_list[2] = {
    u8"index",
    u8"wrapper"
};
const char* GSWrapperPkg::Handler::m_alias_list[2] = {
    u8"index",
    u8"wrapper"
};
const std::bitset<2> GSWrapperPkg::Handler::template_assigned_bitset { "11" };

const char* LoadAuctionObjectDataReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadAuctionObjectDataReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadAuctionObjectDataReq::Handler::template_assigned_bitset { "1" };

const char* LoadAuctionObjectDataAck::Handler::m_key_list[6] = {
    u8"index",
    u8"total_num",
    u8"remain_num",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadAuctionObjectDataAck::Handler::m_alias_list[6] = {
    u8"index",
    u8"total_num",
    u8"remain_num",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<6> LoadAuctionObjectDataAck::Handler::template_assigned_bitset { "111111" };

const char* AddAuctionObjectDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddAuctionObjectDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddAuctionObjectDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelAuctionObjectDataNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* DelAuctionObjectDataNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> DelAuctionObjectDataNtf::Handler::template_assigned_bitset { "1" };

const char* UpdateAuctionObjectDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* UpdateAuctionObjectDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> UpdateAuctionObjectDataNtf::Handler::template_assigned_bitset { "1" };

const char* LoadAuctionCookieDataReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadAuctionCookieDataReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadAuctionCookieDataReq::Handler::template_assigned_bitset { "1" };

const char* LoadAuctionCookieDataAck::Handler::m_key_list[6] = {
    u8"index",
    u8"total_num",
    u8"remain_num",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadAuctionCookieDataAck::Handler::m_alias_list[6] = {
    u8"index",
    u8"total_num",
    u8"remain_num",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<6> LoadAuctionCookieDataAck::Handler::template_assigned_bitset { "111111" };

const char* DuplicateAuctionCookieDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* DuplicateAuctionCookieDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> DuplicateAuctionCookieDataNtf::Handler::template_assigned_bitset { "1" };

const char* LoadGuildDataReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadGuildDataReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadGuildDataReq::Handler::template_assigned_bitset { "1" };

const char* LoadGuildDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadGuildDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> LoadGuildDataAck::Handler::template_assigned_bitset { "1111" };

const char* LoadGuildMemberDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadGuildMemberDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> LoadGuildMemberDataAck::Handler::template_assigned_bitset { "1111" };

const char* LoadGuildApplicantDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadGuildApplicantDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> LoadGuildApplicantDataAck::Handler::template_assigned_bitset { "1111" };

const char* AddGuildDataNtf::Handler::m_key_list[1] = {
    u8"guild"
};
const char* AddGuildDataNtf::Handler::m_alias_list[1] = {
    u8"guild"
};
const std::bitset<1> AddGuildDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelGuildDataNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* DelGuildDataNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> DelGuildDataNtf::Handler::template_assigned_bitset { "1" };

const char* UpdateGuildDataNtf::Handler::m_key_list[1] = {
    u8"guild"
};
const char* UpdateGuildDataNtf::Handler::m_alias_list[1] = {
    u8"guild"
};
const std::bitset<1> UpdateGuildDataNtf::Handler::template_assigned_bitset { "1" };

const char* AddGuildMemberDataNtf::Handler::m_key_list[1] = {
    u8"member"
};
const char* AddGuildMemberDataNtf::Handler::m_alias_list[1] = {
    u8"member"
};
const std::bitset<1> AddGuildMemberDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelGuildMemberDataNtf::Handler::m_key_list[1] = {
    u8"player_guid"
};
const char* DelGuildMemberDataNtf::Handler::m_alias_list[1] = {
    u8"player_guid"
};
const std::bitset<1> DelGuildMemberDataNtf::Handler::template_assigned_bitset { "1" };

const char* UpdateGuildMemberDataNtf::Handler::m_key_list[1] = {
    u8"member"
};
const char* UpdateGuildMemberDataNtf::Handler::m_alias_list[1] = {
    u8"member"
};
const std::bitset<1> UpdateGuildMemberDataNtf::Handler::template_assigned_bitset { "1" };

const char* AddGuildApplicantDataNtf::Handler::m_key_list[1] = {
    u8"applicant"
};
const char* AddGuildApplicantDataNtf::Handler::m_alias_list[1] = {
    u8"applicant"
};
const std::bitset<1> AddGuildApplicantDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelGuildApplicantDataNtf::Handler::m_key_list[2] = {
    u8"player_guid",
    u8"guild_guid"
};
const char* DelGuildApplicantDataNtf::Handler::m_alias_list[2] = {
    u8"player_guid",
    u8"guild_guid"
};
const std::bitset<2> DelGuildApplicantDataNtf::Handler::template_assigned_bitset { "11" };

const char* UpdateGuildApplicantDataNtf::Handler::m_key_list[1] = {
    u8"applicant"
};
const char* UpdateGuildApplicantDataNtf::Handler::m_alias_list[1] = {
    u8"applicant"
};
const std::bitset<1> UpdateGuildApplicantDataNtf::Handler::template_assigned_bitset { "1" };

const char* LoadContactDataReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadContactDataReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadContactDataReq::Handler::template_assigned_bitset { "1" };

const char* LoadContactInfoDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadContactInfoDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> LoadContactInfoDataAck::Handler::template_assigned_bitset { "1111" };

const char* LoadContactDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadContactDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> LoadContactDataAck::Handler::template_assigned_bitset { "1111" };

const char* AddContactDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddContactDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddContactDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelContactDataNtf::Handler::m_key_list[2] = {
    u8"player_guid",
    u8"target_guid"
};
const char* DelContactDataNtf::Handler::m_alias_list[2] = {
    u8"player_guid",
    u8"target_guid"
};
const std::bitset<2> DelContactDataNtf::Handler::template_assigned_bitset { "11" };

const char* UpdateContactDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* UpdateContactDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> UpdateContactDataNtf::Handler::template_assigned_bitset { "1" };

const char* LoadDBVarDataReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadDBVarDataReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadDBVarDataReq::Handler::template_assigned_bitset { "1" };

const char* LoadDBVarDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadDBVarDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> LoadDBVarDataAck::Handler::template_assigned_bitset { "1111" };

const char* AddDBVarDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddDBVarDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddDBVarDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelDBVarDataNtf::Handler::m_key_list[2] = {
    u8"key",
    u8"value_type"
};
const char* DelDBVarDataNtf::Handler::m_alias_list[2] = {
    u8"key",
    u8"value_type"
};
const std::bitset<2> DelDBVarDataNtf::Handler::template_assigned_bitset { "11" };

const char* LoadMailDataReq::Handler::m_key_list[2] = {
    u8"index",
    u8"recv_guid"
};
const char* LoadMailDataReq::Handler::m_alias_list[2] = {
    u8"index",
    u8"recv_guid"
};
const std::bitset<2> LoadMailDataReq::Handler::template_assigned_bitset { "11" };

const char* LoadMailDataAck::Handler::m_key_list[5] = {
    u8"index",
    u8"recv_guid",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* LoadMailDataAck::Handler::m_alias_list[5] = {
    u8"index",
    u8"recv_guid",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<5> LoadMailDataAck::Handler::template_assigned_bitset { "11111" };

const char* AddMailDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddMailDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddMailDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelMailDataNtf::Handler::m_key_list[2] = {
    u8"mail_guid",
    u8"recv_guid"
};
const char* DelMailDataNtf::Handler::m_alias_list[2] = {
    u8"mail_guid",
    u8"recv_guid"
};
const std::bitset<2> DelMailDataNtf::Handler::template_assigned_bitset { "11" };

const char* UpdateMailDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* UpdateMailDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> UpdateMailDataNtf::Handler::template_assigned_bitset { "1" };

const char* LoadRanklistDataReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadRanklistDataReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadRanklistDataReq::Handler::template_assigned_bitset { "1" };

const char* LoadRanklistDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"errcode",
    u8"errmsg",
    u8"data"
};
const char* LoadRanklistDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"errcode",
    u8"errmsg",
    u8"data"
};
const std::bitset<4> LoadRanklistDataAck::Handler::template_assigned_bitset { "1111" };

const char* AddRanklistDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddRanklistDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddRanklistDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelRanklistDataNtf::Handler::m_key_list[2] = {
    u8"object_guid",
    u8"rank_type"
};
const char* DelRanklistDataNtf::Handler::m_alias_list[2] = {
    u8"object_guid",
    u8"rank_type"
};
const std::bitset<2> DelRanklistDataNtf::Handler::template_assigned_bitset { "11" };

const char* UpdateRanklistDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* UpdateRanklistDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> UpdateRanklistDataNtf::Handler::template_assigned_bitset { "1" };

const char* BillInMockReq::Handler::m_key_list[6] = {
    u8"bill_id",
    u8"user_id",
    u8"added_yb",
    u8"award_yb",
    u8"user",
    u8"desc"
};
const char* BillInMockReq::Handler::m_alias_list[6] = {
    u8"bill_id",
    u8"user_id",
    u8"added_yb",
    u8"award_yb",
    u8"user",
    u8"desc"
};
const std::bitset<6> BillInMockReq::Handler::template_assigned_bitset { "111111" };

const char* AddPasturePetDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddPasturePetDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddPasturePetDataNtf::Handler::template_assigned_bitset { "1" };

const char* LoadPasturePetDataReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadPasturePetDataReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadPasturePetDataReq::Handler::template_assigned_bitset { "1" };

const char* LoadPasturePetDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"errcode",
    u8"errmsg",
    u8"data"
};
const char* LoadPasturePetDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"errcode",
    u8"errmsg",
    u8"data"
};
const std::bitset<4> LoadPasturePetDataAck::Handler::template_assigned_bitset { "1111" };

const char* FregmentPkg::Handler::m_key_list[2] = {
    u8"frag_mark",
    u8"frag_data"
};
const char* FregmentPkg::Handler::m_alias_list[2] = {
    u8"frag_mark",
    u8"frag_data"
};
const std::bitset<2> FregmentPkg::Handler::template_assigned_bitset { "11" };

const char* DelPasturePetDataNtf::Handler::m_key_list[1] = {
    u8"pet_guid"
};
const char* DelPasturePetDataNtf::Handler::m_alias_list[1] = {
    u8"pet_guid"
};
const std::bitset<1> DelPasturePetDataNtf::Handler::template_assigned_bitset { "1" };

const char* UpdatePasturePetDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* UpdatePasturePetDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> UpdatePasturePetDataNtf::Handler::template_assigned_bitset { "1" };

const char* LoadContactInfoReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadContactInfoReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadContactInfoReq::Handler::template_assigned_bitset { "1" };

const char* LoadLadderDataReq::Handler::m_key_list[1] = {
    u8"index"
};
const char* LoadLadderDataReq::Handler::m_alias_list[1] = {
    u8"index"
};
const std::bitset<1> LoadLadderDataReq::Handler::template_assigned_bitset { "1" };

const char* LoadLadderDataAck::Handler::m_key_list[4] = {
    u8"index",
    u8"errcode",
    u8"errmsg",
    u8"data"
};
const char* LoadLadderDataAck::Handler::m_alias_list[4] = {
    u8"index",
    u8"errcode",
    u8"errmsg",
    u8"data"
};
const std::bitset<4> LoadLadderDataAck::Handler::template_assigned_bitset { "1111" };

const char* AddLadderDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddLadderDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddLadderDataNtf::Handler::template_assigned_bitset { "1" };

const char* DelLadderDataNtf::Handler::m_key_list[1] = {
    u8"player_guid"
};
const char* DelLadderDataNtf::Handler::m_alias_list[1] = {
    u8"player_guid"
};
const std::bitset<1> DelLadderDataNtf::Handler::template_assigned_bitset { "1" };

const char* UpdateLadderDataNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* UpdateLadderDataNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> UpdateLadderDataNtf::Handler::template_assigned_bitset { "1" };

const char* UnloadMailDataReq::Handler::m_key_list[2] = {
    u8"index",
    u8"recv_guid"
};
const char* UnloadMailDataReq::Handler::m_alias_list[2] = {
    u8"index",
    u8"recv_guid"
};
const std::bitset<2> UnloadMailDataReq::Handler::template_assigned_bitset { "11" };

const char* LoadOfflinePlayerReq::Handler::m_key_list[5] = {
    u8"index",
    u8"seq",
    u8"target_guid",
    u8"object_type",
    u8"object_guid"
};
const char* LoadOfflinePlayerReq::Handler::m_alias_list[5] = {
    u8"index",
    u8"seq",
    u8"target_guid",
    u8"object_type",
    u8"object_guid"
};
const std::bitset<5> LoadOfflinePlayerReq::Handler::template_assigned_bitset { "11111" };

const char* LoadOfflinePlayerAck::Handler::m_key_list[8] = {
    u8"index",
    u8"seq",
    u8"target_guid",
    u8"object_type",
    u8"object_guid",
    u8"errcode",
    u8"errmsg",
    u8"offline_data"
};
const char* LoadOfflinePlayerAck::Handler::m_alias_list[8] = {
    u8"index",
    u8"seq",
    u8"target_guid",
    u8"object_type",
    u8"object_guid",
    u8"errcode",
    u8"errmsg",
    u8"offline_data"
};
const std::bitset<8> LoadOfflinePlayerAck::Handler::template_assigned_bitset { "11111111" };

const char* SyncViolateNtf::Handler::m_key_list[1] = {
    u8"violates"
};
const char* SyncViolateNtf::Handler::m_alias_list[1] = {
    u8"violates"
};
const std::bitset<1> SyncViolateNtf::Handler::template_assigned_bitset { "1" };

const char* SyncForbidTalkNtf::Handler::m_key_list[1] = {
    u8"forbids"
};
const char* SyncForbidTalkNtf::Handler::m_alias_list[1] = {
    u8"forbids"
};
const std::bitset<1> SyncForbidTalkNtf::Handler::template_assigned_bitset { "1" };

const char* AddForbidTalkNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* AddForbidTalkNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> AddForbidTalkNtf::Handler::template_assigned_bitset { "1" };

const char* DelForbidTalkNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* DelForbidTalkNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> DelForbidTalkNtf::Handler::template_assigned_bitset { "1" };

const char* LogLevelNtf::Handler::m_key_list[2] = {
    u8"level",
    u8"enable"
};
const char* LogLevelNtf::Handler::m_alias_list[2] = {
    u8"level",
    u8"enable"
};
const std::bitset<2> LogLevelNtf::Handler::template_assigned_bitset { "11" };

