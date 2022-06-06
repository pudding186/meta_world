#include <new>

#include "ClientGS.hpp"

const char* KeepAliveReq::Handler::m_key_list[1] = {
    u8"seq"
};
const char* KeepAliveReq::Handler::m_alias_list[1] = {
    u8"seq"
};
const std::bitset<1> KeepAliveReq::Handler::template_assigned_bitset { "1" };

const char* KeepAliveAck::Handler::m_key_list[2] = {
    u8"seq",
    u8"tick"
};
const char* KeepAliveAck::Handler::m_alias_list[2] = {
    u8"seq",
    u8"tick"
};
const std::bitset<2> KeepAliveAck::Handler::template_assigned_bitset { "11" };

const char* AttrNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"attrs"
};
const char* AttrNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"attrs"
};
const std::bitset<2> AttrNtf::Handler::template_assigned_bitset { "11" };

const char* PlayerAppearNtf::Handler::m_key_list[8] = {
    u8"guid",
    u8"sn",
    u8"x",
    u8"y",
    u8"attrs",
    u8"buffs",
    u8"custom",
    u8"name"
};
const char* PlayerAppearNtf::Handler::m_alias_list[8] = {
    u8"guid",
    u8"sn",
    u8"x",
    u8"y",
    u8"attrs",
    u8"buffs",
    u8"custom",
    u8"name"
};
const std::bitset<8> PlayerAppearNtf::Handler::template_assigned_bitset { "11111111" };

const char* NPCAppearNtf::Handler::m_key_list[8] = {
    u8"guid",
    u8"template_id",
    u8"x",
    u8"y",
    u8"dir",
    u8"attrs",
    u8"custom",
    u8"name"
};
const char* NPCAppearNtf::Handler::m_alias_list[8] = {
    u8"guid",
    u8"template_id",
    u8"x",
    u8"y",
    u8"dir",
    u8"attrs",
    u8"custom",
    u8"name"
};
const std::bitset<8> NPCAppearNtf::Handler::template_assigned_bitset { "11111111" };

const char* ItemAppearNtf::Handler::m_key_list[7] = {
    u8"guid",
    u8"template_id",
    u8"x",
    u8"y",
    u8"custom",
    u8"name",
    u8"dyn_attrs"
};
const char* ItemAppearNtf::Handler::m_alias_list[7] = {
    u8"guid",
    u8"template_id",
    u8"x",
    u8"y",
    u8"custom",
    u8"name",
    u8"dyn_attrs"
};
const std::bitset<7> ItemAppearNtf::Handler::template_assigned_bitset { "1111111" };

const char* ObjDisAppearNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* ObjDisAppearNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> ObjDisAppearNtf::Handler::template_assigned_bitset { "1" };

const char* ObjMoveNtf::Handler::m_key_list[4] = {
    u8"guid",
    u8"dst_x",
    u8"dst_y",
    u8"type"
};
const char* ObjMoveNtf::Handler::m_alias_list[4] = {
    u8"guid",
    u8"dst_x",
    u8"dst_y",
    u8"type"
};
const std::bitset<4> ObjMoveNtf::Handler::template_assigned_bitset { "1111" };

const char* EnterMapNtf::Handler::m_key_list[3] = {
    u8"template_id",
    u8"dst_x",
    u8"dst_y"
};
const char* EnterMapNtf::Handler::m_alias_list[3] = {
    u8"template_id",
    u8"dst_x",
    u8"dst_y"
};
const std::bitset<3> EnterMapNtf::Handler::template_assigned_bitset { "111" };

const char* MoveReq::Handler::m_key_list[6] = {
    u8"dst_x",
    u8"dst_y",
    u8"cur_x",
    u8"cur_y",
    u8"idx",
    u8"tick"
};
const char* MoveReq::Handler::m_alias_list[6] = {
    u8"dst_x",
    u8"dst_y",
    u8"cur_x",
    u8"cur_y",
    u8"idx",
    u8"tick"
};
const std::bitset<6> MoveReq::Handler::template_assigned_bitset { "111111" };

const char* MoveAck::Handler::m_key_list[4] = {
    u8"errcode",
    u8"dst_x",
    u8"dst_y",
    u8"idx"
};
const char* MoveAck::Handler::m_alias_list[4] = {
    u8"errcode",
    u8"dst_x",
    u8"dst_y",
    u8"idx"
};
const std::bitset<4> MoveAck::Handler::template_assigned_bitset { "1111" };

const char* JumpMapReq::Handler::m_key_list[1] = {
    u8"idx"
};
const char* JumpMapReq::Handler::m_alias_list[1] = {
    u8"idx"
};
const std::bitset<1> JumpMapReq::Handler::template_assigned_bitset { "1" };

const char* JumpMapAck::Handler::m_key_list[1] = {
    u8"errcode"
};
const char* JumpMapAck::Handler::m_alias_list[1] = {
    u8"errcode"
};
const std::bitset<1> JumpMapAck::Handler::template_assigned_bitset { "1" };

const char* AddJumpMapRegionNtf::Handler::m_key_list[2] = {
    u8"idx",
    u8"jump_region"
};
const char* AddJumpMapRegionNtf::Handler::m_alias_list[2] = {
    u8"idx",
    u8"jump_region"
};
const std::bitset<2> AddJumpMapRegionNtf::Handler::template_assigned_bitset { "11" };

const char* DelJumpMapRegionNtf::Handler::m_key_list[1] = {
    u8"idx"
};
const char* DelJumpMapRegionNtf::Handler::m_alias_list[1] = {
    u8"idx"
};
const std::bitset<1> DelJumpMapRegionNtf::Handler::template_assigned_bitset { "1" };

const char* ItemAddNtf::Handler::m_key_list[3] = {
    u8"guid",
    u8"container_type",
    u8"items"
};
const char* ItemAddNtf::Handler::m_alias_list[3] = {
    u8"guid",
    u8"container_type",
    u8"items"
};
const std::bitset<3> ItemAddNtf::Handler::template_assigned_bitset { "111" };

const char* ItemUpdateNtf::Handler::m_key_list[3] = {
    u8"guid",
    u8"container_type",
    u8"attrs"
};
const char* ItemUpdateNtf::Handler::m_alias_list[3] = {
    u8"guid",
    u8"container_type",
    u8"attrs"
};
const std::bitset<3> ItemUpdateNtf::Handler::template_assigned_bitset { "111" };

const char* ItemDestroyNtf::Handler::m_key_list[3] = {
    u8"guid",
    u8"container_type",
    u8"item_guids"
};
const char* ItemDestroyNtf::Handler::m_alias_list[3] = {
    u8"guid",
    u8"container_type",
    u8"item_guids"
};
const std::bitset<3> ItemDestroyNtf::Handler::template_assigned_bitset { "111" };

const char* TipsMsgNtf::Handler::m_key_list[1] = {
    u8"msg"
};
const char* TipsMsgNtf::Handler::m_alias_list[1] = {
    u8"msg"
};
const std::bitset<1> TipsMsgNtf::Handler::template_assigned_bitset { "1" };

const char* TopMsgNtf::Handler::m_key_list[4] = {
    u8"foreground",
    u8"background",
    u8"count",
    u8"msg"
};
const char* TopMsgNtf::Handler::m_alias_list[4] = {
    u8"foreground",
    u8"background",
    u8"count",
    u8"msg"
};
const std::bitset<4> TopMsgNtf::Handler::template_assigned_bitset { "1111" };

const char* SysMsgNtf::Handler::m_key_list[3] = {
    u8"foreground",
    u8"background",
    u8"msg"
};
const char* SysMsgNtf::Handler::m_alias_list[3] = {
    u8"foreground",
    u8"background",
    u8"msg"
};
const std::bitset<3> SysMsgNtf::Handler::template_assigned_bitset { "111" };

const char* PopupMsgNtf::Handler::m_key_list[2] = {
    u8"type",
    u8"msg"
};
const char* PopupMsgNtf::Handler::m_alias_list[2] = {
    u8"type",
    u8"msg"
};
const std::bitset<2> PopupMsgNtf::Handler::template_assigned_bitset { "11" };

const char* ItemContainerNtf::Handler::m_key_list[4] = {
    u8"guid",
    u8"container_type",
    u8"capacity",
    u8"items"
};
const char* ItemContainerNtf::Handler::m_alias_list[4] = {
    u8"guid",
    u8"container_type",
    u8"capacity",
    u8"items"
};
const std::bitset<4> ItemContainerNtf::Handler::template_assigned_bitset { "1111" };

const char* ItemContainerUpdateNtf::Handler::m_key_list[3] = {
    u8"guid",
    u8"container_type",
    u8"capacity"
};
const char* ItemContainerUpdateNtf::Handler::m_alias_list[3] = {
    u8"guid",
    u8"container_type",
    u8"capacity"
};
const std::bitset<3> ItemContainerUpdateNtf::Handler::template_assigned_bitset { "111" };

const char* SubmitForm::Handler::m_key_list[3] = {
    u8"form",
    u8"func",
    u8"args"
};
const char* SubmitForm::Handler::m_alias_list[3] = {
    u8"form",
    u8"func",
    u8"args"
};
const std::bitset<3> SubmitForm::Handler::template_assigned_bitset { "111" };

const char* ShowFormNtf::Handler::m_key_list[3] = {
    u8"form",
    u8"compressed",
    u8"context"
};
const char* ShowFormNtf::Handler::m_alias_list[3] = {
    u8"form",
    u8"compressed",
    u8"context"
};
const std::bitset<3> ShowFormNtf::Handler::template_assigned_bitset { "111" };

const char* ExecuteGMReq::Handler::m_key_list[2] = {
    u8"cmd",
    u8"arg"
};
const char* ExecuteGMReq::Handler::m_alias_list[2] = {
    u8"cmd",
    u8"arg"
};
const std::bitset<2> ExecuteGMReq::Handler::template_assigned_bitset { "11" };

const char* FightBeginNtf::Handler::m_key_list[3] = {
    u8"groups",
    u8"self_group",
    u8"is_pvp"
};
const char* FightBeginNtf::Handler::m_alias_list[3] = {
    u8"groups",
    u8"self_group",
    u8"is_pvp"
};
const std::bitset<3> FightBeginNtf::Handler::template_assigned_bitset { "111" };

const char* TurnRoundNtf::Handler::m_key_list[1] = {
    u8"round"
};
const char* TurnRoundNtf::Handler::m_alias_list[1] = {
    u8"round"
};
const std::bitset<1> TurnRoundNtf::Handler::template_assigned_bitset { "1" };

const char* FightOperateListNtf::Handler::m_key_list[3] = {
    u8"player_operates",
    u8"pet_operates",
    u8"count_down"
};
const char* FightOperateListNtf::Handler::m_alias_list[3] = {
    u8"player_operates",
    u8"pet_operates",
    u8"count_down"
};
const std::bitset<3> FightOperateListNtf::Handler::template_assigned_bitset { "111" };

const char* FightOperateReq::Handler::m_key_list[1] = {
    u8"operate"
};
const char* FightOperateReq::Handler::m_alias_list[1] = {
    u8"operate"
};
const std::bitset<1> FightOperateReq::Handler::template_assigned_bitset { "1" };

const char* FightOperateAck::Handler::m_key_list[2] = {
    u8"errcode",
    u8"errmsg"
};
const char* FightOperateAck::Handler::m_alias_list[2] = {
    u8"errcode",
    u8"errmsg"
};
const std::bitset<2> FightOperateAck::Handler::template_assigned_bitset { "11" };

const char* FightOperateNtf::Handler::m_key_list[1] = {
    u8"fighter_id"
};
const char* FightOperateNtf::Handler::m_alias_list[1] = {
    u8"fighter_id"
};
const std::bitset<1> FightOperateNtf::Handler::template_assigned_bitset { "1" };

const char* FightDisplayNtf::Handler::m_key_list[2] = {
    u8"data",
    u8"crypto"
};
const char* FightDisplayNtf::Handler::m_alias_list[2] = {
    u8"data",
    u8"crypto"
};
const std::bitset<2> FightDisplayNtf::Handler::template_assigned_bitset { "11" };

const char* FightDisplayCompleteNtf::Handler::m_key_list[1] = {
    u8"crypto"
};
const char* FightDisplayCompleteNtf::Handler::m_alias_list[1] = {
    u8"crypto"
};
const std::bitset<1> FightDisplayCompleteNtf::Handler::template_assigned_bitset { "1" };

const char* FightAutoReq::Handler::m_key_list[1] = {
    u8"is_auto"
};
const char* FightAutoReq::Handler::m_alias_list[1] = {
    u8"is_auto"
};
const std::bitset<1> FightAutoReq::Handler::template_assigned_bitset { "1" };

const char* FightAutoAck::Handler::m_key_list[3] = {
    u8"is_auto",
    u8"errcode",
    u8"errmsg"
};
const char* FightAutoAck::Handler::m_alias_list[3] = {
    u8"is_auto",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> FightAutoAck::Handler::template_assigned_bitset { "111" };

const char* FightAutoNtf::Handler::m_key_list[2] = {
    u8"fighter_id",
    u8"is_auto"
};
const char* FightAutoNtf::Handler::m_alias_list[2] = {
    u8"fighter_id",
    u8"is_auto"
};
const std::bitset<2> FightAutoNtf::Handler::template_assigned_bitset { "11" };

const char* FightAutoSkillReq::Handler::m_key_list[2] = {
    u8"actor",
    u8"skillid"
};
const char* FightAutoSkillReq::Handler::m_alias_list[2] = {
    u8"actor",
    u8"skillid"
};
const std::bitset<2> FightAutoSkillReq::Handler::template_assigned_bitset { "11" };

const char* FightAutoSkillAck::Handler::m_key_list[4] = {
    u8"actor",
    u8"skillid",
    u8"errcode",
    u8"errmsg"
};
const char* FightAutoSkillAck::Handler::m_alias_list[4] = {
    u8"actor",
    u8"skillid",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> FightAutoSkillAck::Handler::template_assigned_bitset { "1111" };

const char* FightEndNtf::Handler::m_key_list[1] = {
    u8"result"
};
const char* FightEndNtf::Handler::m_alias_list[1] = {
    u8"result"
};
const std::bitset<1> FightEndNtf::Handler::template_assigned_bitset { "1" };

const char* AddFighterNtf::Handler::m_key_list[2] = {
    u8"data",
    u8"group"
};
const char* AddFighterNtf::Handler::m_alias_list[2] = {
    u8"data",
    u8"group"
};
const std::bitset<2> AddFighterNtf::Handler::template_assigned_bitset { "11" };

const char* DelFighterNtf::Handler::m_key_list[1] = {
    u8"fighter_id"
};
const char* DelFighterNtf::Handler::m_alias_list[1] = {
    u8"fighter_id"
};
const std::bitset<1> DelFighterNtf::Handler::template_assigned_bitset { "1" };

const char* AddFightPetData::Handler::m_key_list[1] = {
    u8"pet"
};
const char* AddFightPetData::Handler::m_alias_list[1] = {
    u8"pet"
};
const std::bitset<1> AddFightPetData::Handler::template_assigned_bitset { "1" };

const char* UpdateFightPetData::Handler::m_key_list[1] = {
    u8"pet"
};
const char* UpdateFightPetData::Handler::m_alias_list[1] = {
    u8"pet"
};
const std::bitset<1> UpdateFightPetData::Handler::template_assigned_bitset { "1" };

const char* PlayerKillReq::Handler::m_key_list[1] = {
    u8"target"
};
const char* PlayerKillReq::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> PlayerKillReq::Handler::template_assigned_bitset { "1" };

const char* PlayerKillAck::Handler::m_key_list[3] = {
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const char* PlayerKillAck::Handler::m_alias_list[3] = {
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> PlayerKillAck::Handler::template_assigned_bitset { "111" };

const char* EnterFightViewReq::Handler::m_key_list[1] = {
    u8"target"
};
const char* EnterFightViewReq::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> EnterFightViewReq::Handler::template_assigned_bitset { "1" };

const char* EnterFightViewAck::Handler::m_key_list[3] = {
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const char* EnterFightViewAck::Handler::m_alias_list[3] = {
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> EnterFightViewAck::Handler::template_assigned_bitset { "111" };

const char* LeaveFightViewAck::Handler::m_key_list[2] = {
    u8"errcode",
    u8"errmsg"
};
const char* LeaveFightViewAck::Handler::m_alias_list[2] = {
    u8"errcode",
    u8"errmsg"
};
const std::bitset<2> LeaveFightViewAck::Handler::template_assigned_bitset { "11" };

const char* TeamLeaderOprReq::Handler::m_key_list[1] = {
    u8"opr"
};
const char* TeamLeaderOprReq::Handler::m_alias_list[1] = {
    u8"opr"
};
const std::bitset<1> TeamLeaderOprReq::Handler::template_assigned_bitset { "1" };

const char* TeamNtf::Handler::m_key_list[11] = {
    u8"team_guid",
    u8"target",
    u8"permission",
    u8"min_require_level",
    u8"max_require_level",
    u8"leader_guid",
    u8"leader_guards",
    u8"members",
    u8"min_require_reinc",
    u8"max_require_reinc",
    u8"customs"
};
const char* TeamNtf::Handler::m_alias_list[11] = {
    u8"team_guid",
    u8"target",
    u8"permission",
    u8"min_require_level",
    u8"max_require_level",
    u8"leader_guid",
    u8"leader_guards",
    u8"members",
    u8"min_require_reinc",
    u8"max_require_reinc",
    u8"customs"
};
const std::bitset<11> TeamNtf::Handler::template_assigned_bitset { "11111111111" };

const char* TeamLeaderNtf::Handler::m_key_list[2] = {
    u8"leader_guid",
    u8"leader_guards"
};
const char* TeamLeaderNtf::Handler::m_alias_list[2] = {
    u8"leader_guid",
    u8"leader_guards"
};
const std::bitset<2> TeamLeaderNtf::Handler::template_assigned_bitset { "11" };

const char* TeamDestroyNtf::Handler::m_key_list[1] = {
    u8"team_guid"
};
const char* TeamDestroyNtf::Handler::m_alias_list[1] = {
    u8"team_guid"
};
const std::bitset<1> TeamDestroyNtf::Handler::template_assigned_bitset { "1" };

const char* TeamMemberNtf::Handler::m_key_list[1] = {
    u8"member"
};
const char* TeamMemberNtf::Handler::m_alias_list[1] = {
    u8"member"
};
const std::bitset<1> TeamMemberNtf::Handler::template_assigned_bitset { "1" };

const char* TeamMemberLeaveNtf::Handler::m_key_list[2] = {
    u8"action",
    u8"player_guid"
};
const char* TeamMemberLeaveNtf::Handler::m_alias_list[2] = {
    u8"action",
    u8"player_guid"
};
const std::bitset<2> TeamMemberLeaveNtf::Handler::template_assigned_bitset { "11" };

const char* NpcSelectReq::Handler::m_key_list[2] = {
    u8"npc_guid",
    u8"select_id"
};
const char* NpcSelectReq::Handler::m_alias_list[2] = {
    u8"npc_guid",
    u8"select_id"
};
const std::bitset<2> NpcSelectReq::Handler::template_assigned_bitset { "11" };

const char* NpcSelectAck::Handler::m_key_list[3] = {
    u8"npc_guid",
    u8"options",
    u8"msg"
};
const char* NpcSelectAck::Handler::m_alias_list[3] = {
    u8"npc_guid",
    u8"options",
    u8"msg"
};
const std::bitset<3> NpcSelectAck::Handler::template_assigned_bitset { "111" };

const char* NpcTalkReq::Handler::m_key_list[3] = {
    u8"npc_guid",
    u8"compressed",
    u8"talk"
};
const char* NpcTalkReq::Handler::m_alias_list[3] = {
    u8"npc_guid",
    u8"compressed",
    u8"talk"
};
const std::bitset<3> NpcTalkReq::Handler::template_assigned_bitset { "111" };

const char* NpcTalkAck::Handler::m_key_list[3] = {
    u8"npc_guid",
    u8"compressed",
    u8"talk"
};
const char* NpcTalkAck::Handler::m_alias_list[3] = {
    u8"npc_guid",
    u8"compressed",
    u8"talk"
};
const std::bitset<3> NpcTalkAck::Handler::template_assigned_bitset { "111" };

const char* InviteMsgNtf::Handler::m_key_list[4] = {
    u8"inviter_guid",
    u8"type",
    u8"msg",
    u8"interval"
};
const char* InviteMsgNtf::Handler::m_alias_list[4] = {
    u8"inviter_guid",
    u8"type",
    u8"msg",
    u8"interval"
};
const std::bitset<4> InviteMsgNtf::Handler::template_assigned_bitset { "1111" };

const char* ReplyInvite::Handler::m_key_list[3] = {
    u8"inviter_guid",
    u8"type",
    u8"agreed"
};
const char* ReplyInvite::Handler::m_alias_list[3] = {
    u8"inviter_guid",
    u8"type",
    u8"agreed"
};
const std::bitset<3> ReplyInvite::Handler::template_assigned_bitset { "111" };

const char* MoveItem::Handler::m_key_list[2] = {
    u8"item_guid",
    u8"container_type"
};
const char* MoveItem::Handler::m_alias_list[2] = {
    u8"item_guid",
    u8"container_type"
};
const std::bitset<2> MoveItem::Handler::template_assigned_bitset { "11" };

const char* UseItem::Handler::m_key_list[1] = {
    u8"item_guid"
};
const char* UseItem::Handler::m_alias_list[1] = {
    u8"item_guid"
};
const std::bitset<1> UseItem::Handler::template_assigned_bitset { "1" };

const char* RearrangeItem::Handler::m_key_list[1] = {
    u8"container_type"
};
const char* RearrangeItem::Handler::m_alias_list[1] = {
    u8"container_type"
};
const std::bitset<1> RearrangeItem::Handler::template_assigned_bitset { "1" };

const char* SkillContainerNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"skills"
};
const char* SkillContainerNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"skills"
};
const std::bitset<2> SkillContainerNtf::Handler::template_assigned_bitset { "11" };

const char* AddSkillNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"skill"
};
const char* AddSkillNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"skill"
};
const std::bitset<2> AddSkillNtf::Handler::template_assigned_bitset { "11" };

const char* UpdateSkillNtf::Handler::m_key_list[4] = {
    u8"guid",
    u8"id",
    u8"attr",
    u8"value"
};
const char* UpdateSkillNtf::Handler::m_alias_list[4] = {
    u8"guid",
    u8"id",
    u8"attr",
    u8"value"
};
const std::bitset<4> UpdateSkillNtf::Handler::template_assigned_bitset { "1111" };

const char* DelSkillNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"id"
};
const char* DelSkillNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"id"
};
const std::bitset<2> DelSkillNtf::Handler::template_assigned_bitset { "11" };

const char* PetAppearNtf::Handler::m_key_list[6] = {
    u8"guid",
    u8"name",
    u8"x",
    u8"y",
    u8"attrs",
    u8"custom"
};
const char* PetAppearNtf::Handler::m_alias_list[6] = {
    u8"guid",
    u8"name",
    u8"x",
    u8"y",
    u8"attrs",
    u8"custom"
};
const std::bitset<6> PetAppearNtf::Handler::template_assigned_bitset { "111111" };

const char* PetContainerNtf::Handler::m_key_list[3] = {
    u8"container_type",
    u8"capacity",
    u8"pets"
};
const char* PetContainerNtf::Handler::m_alias_list[3] = {
    u8"container_type",
    u8"capacity",
    u8"pets"
};
const std::bitset<3> PetContainerNtf::Handler::template_assigned_bitset { "111" };

const char* PetContainerUpdateNtf::Handler::m_key_list[2] = {
    u8"container_type",
    u8"capacity"
};
const char* PetContainerUpdateNtf::Handler::m_alias_list[2] = {
    u8"container_type",
    u8"capacity"
};
const std::bitset<2> PetContainerUpdateNtf::Handler::template_assigned_bitset { "11" };

const char* PetAddNtf::Handler::m_key_list[2] = {
    u8"container_type",
    u8"pet"
};
const char* PetAddNtf::Handler::m_alias_list[2] = {
    u8"container_type",
    u8"pet"
};
const std::bitset<2> PetAddNtf::Handler::template_assigned_bitset { "11" };

const char* PetDestroyNtf::Handler::m_key_list[2] = {
    u8"container_type",
    u8"guid"
};
const char* PetDestroyNtf::Handler::m_alias_list[2] = {
    u8"container_type",
    u8"guid"
};
const std::bitset<2> PetDestroyNtf::Handler::template_assigned_bitset { "11" };

const char* SetPetLineup::Handler::m_key_list[2] = {
    u8"guid",
    u8"lineup"
};
const char* SetPetLineup::Handler::m_alias_list[2] = {
    u8"guid",
    u8"lineup"
};
const std::bitset<2> SetPetLineup::Handler::template_assigned_bitset { "11" };

const char* ShowPet::Handler::m_key_list[2] = {
    u8"guid",
    u8"show"
};
const char* ShowPet::Handler::m_alias_list[2] = {
    u8"guid",
    u8"show"
};
const std::bitset<2> ShowPet::Handler::template_assigned_bitset { "11" };

const char* ReleasePet::Handler::m_key_list[1] = {
    u8"guid"
};
const char* ReleasePet::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> ReleasePet::Handler::template_assigned_bitset { "1" };

const char* MovePet::Handler::m_key_list[2] = {
    u8"guid",
    u8"container_type"
};
const char* MovePet::Handler::m_alias_list[2] = {
    u8"guid",
    u8"container_type"
};
const std::bitset<2> MovePet::Handler::template_assigned_bitset { "11" };

const char* ShopOpenNtf::Handler::m_key_list[5] = {
    u8"shop_id",
    u8"def_item_id",
    u8"type",
    u8"shop_item_list",
    u8"buy_back_list"
};
const char* ShopOpenNtf::Handler::m_alias_list[5] = {
    u8"shop_id",
    u8"def_item_id",
    u8"type",
    u8"shop_item_list",
    u8"buy_back_list"
};
const std::bitset<5> ShopOpenNtf::Handler::template_assigned_bitset { "11111" };

const char* ShopBuyNtf::Handler::m_key_list[3] = {
    u8"shop_id",
    u8"shop_item_id",
    u8"shop_item_num"
};
const char* ShopBuyNtf::Handler::m_alias_list[3] = {
    u8"shop_id",
    u8"shop_item_id",
    u8"shop_item_num"
};
const std::bitset<3> ShopBuyNtf::Handler::template_assigned_bitset { "111" };

const char* SellNtf::Handler::m_key_list[1] = {
    u8"sell_items"
};
const char* SellNtf::Handler::m_alias_list[1] = {
    u8"sell_items"
};
const std::bitset<1> SellNtf::Handler::template_assigned_bitset { "1" };

const char* BuyBackNtf::Handler::m_key_list[1] = {
    u8"buy_guid"
};
const char* BuyBackNtf::Handler::m_alias_list[1] = {
    u8"buy_guid"
};
const std::bitset<1> BuyBackNtf::Handler::template_assigned_bitset { "1" };

const char* BuyBackListNtf::Handler::m_key_list[1] = {
    u8"buy_back_list"
};
const char* BuyBackListNtf::Handler::m_alias_list[1] = {
    u8"buy_back_list"
};
const std::bitset<1> BuyBackListNtf::Handler::template_assigned_bitset { "1" };

const char* TeamAttrNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"attrs"
};
const char* TeamAttrNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"attrs"
};
const std::bitset<2> TeamAttrNtf::Handler::template_assigned_bitset { "11" };

const char* TipsMsgExNtf::Handler::m_key_list[1] = {
    u8"msg"
};
const char* TipsMsgExNtf::Handler::m_alias_list[1] = {
    u8"msg"
};
const std::bitset<1> TipsMsgExNtf::Handler::template_assigned_bitset { "1" };

const char* ItemNewAddNtf::Handler::m_key_list[1] = {
    u8"item_guid"
};
const char* ItemNewAddNtf::Handler::m_alias_list[1] = {
    u8"item_guid"
};
const std::bitset<1> ItemNewAddNtf::Handler::template_assigned_bitset { "1" };

const char* QuestContainerNtf::Handler::m_key_list[1] = {
    u8"buffer"
};
const char* QuestContainerNtf::Handler::m_alias_list[1] = {
    u8"buffer"
};
const std::bitset<1> QuestContainerNtf::Handler::template_assigned_bitset { "1" };

const char* QuestAddNtf::Handler::m_key_list[1] = {
    u8"buffer"
};
const char* QuestAddNtf::Handler::m_alias_list[1] = {
    u8"buffer"
};
const std::bitset<1> QuestAddNtf::Handler::template_assigned_bitset { "1" };

const char* QuestStateNtf::Handler::m_key_list[3] = {
    u8"quest_id",
    u8"quest_state",
    u8"changed_time"
};
const char* QuestStateNtf::Handler::m_alias_list[3] = {
    u8"quest_id",
    u8"quest_state",
    u8"changed_time"
};
const std::bitset<3> QuestStateNtf::Handler::template_assigned_bitset { "111" };

const char* QuestNpcStateNtf::Handler::m_key_list[3] = {
    u8"npc_id",
    u8"state",
    u8"logo"
};
const char* QuestNpcStateNtf::Handler::m_alias_list[3] = {
    u8"npc_id",
    u8"state",
    u8"logo"
};
const std::bitset<3> QuestNpcStateNtf::Handler::template_assigned_bitset { "111" };

const char* QuestTrackCountNtf::Handler::m_key_list[3] = {
    u8"quest_id",
    u8"track_id",
    u8"track_count"
};
const char* QuestTrackCountNtf::Handler::m_alias_list[3] = {
    u8"quest_id",
    u8"track_id",
    u8"track_count"
};
const std::bitset<3> QuestTrackCountNtf::Handler::template_assigned_bitset { "111" };

const char* QuestTalkSelectReq::Handler::m_key_list[3] = {
    u8"npc_guid",
    u8"quest_id",
    u8"option_id"
};
const char* QuestTalkSelectReq::Handler::m_alias_list[3] = {
    u8"npc_guid",
    u8"quest_id",
    u8"option_id"
};
const std::bitset<3> QuestTalkSelectReq::Handler::template_assigned_bitset { "111" };

const char* QuestTalkSelectAck::Handler::m_key_list[3] = {
    u8"npc_guid",
    u8"quest_id",
    u8"talk"
};
const char* QuestTalkSelectAck::Handler::m_alias_list[3] = {
    u8"npc_guid",
    u8"quest_id",
    u8"talk"
};
const std::bitset<3> QuestTalkSelectAck::Handler::template_assigned_bitset { "111" };

const char* ObjStrCustomNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"custom"
};
const char* ObjStrCustomNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"custom"
};
const std::bitset<2> ObjStrCustomNtf::Handler::template_assigned_bitset { "11" };

const char* ObjIntCustomNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"custom"
};
const char* ObjIntCustomNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"custom"
};
const std::bitset<2> ObjIntCustomNtf::Handler::template_assigned_bitset { "11" };

const char* ObjDynAttrNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"dyn_attr"
};
const char* ObjDynAttrNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"dyn_attr"
};
const std::bitset<2> ObjDynAttrNtf::Handler::template_assigned_bitset { "11" };

const char* CommitQuestItemRsp::Handler::m_key_list[2] = {
    u8"quest_id",
    u8"items"
};
const char* CommitQuestItemRsp::Handler::m_alias_list[2] = {
    u8"quest_id",
    u8"items"
};
const std::bitset<2> CommitQuestItemRsp::Handler::template_assigned_bitset { "11" };

const char* CommitQuestItemNtf::Handler::m_key_list[2] = {
    u8"quest_id",
    u8"items"
};
const char* CommitQuestItemNtf::Handler::m_alias_list[2] = {
    u8"quest_id",
    u8"items"
};
const std::bitset<2> CommitQuestItemNtf::Handler::template_assigned_bitset { "11" };

const char* CommitQuestPetRsp::Handler::m_key_list[2] = {
    u8"quest_id",
    u8"pets"
};
const char* CommitQuestPetRsp::Handler::m_alias_list[2] = {
    u8"quest_id",
    u8"pets"
};
const std::bitset<2> CommitQuestPetRsp::Handler::template_assigned_bitset { "11" };

const char* CommitQuestPetNtf::Handler::m_key_list[2] = {
    u8"quest_id",
    u8"pets"
};
const char* CommitQuestPetNtf::Handler::m_alias_list[2] = {
    u8"quest_id",
    u8"pets"
};
const std::bitset<2> CommitQuestPetNtf::Handler::template_assigned_bitset { "11" };

const char* AbandonQuest::Handler::m_key_list[1] = {
    u8"quest_id"
};
const char* AbandonQuest::Handler::m_alias_list[1] = {
    u8"quest_id"
};
const std::bitset<1> AbandonQuest::Handler::template_assigned_bitset { "1" };

const char* SkillTipsReq::Handler::m_key_list[2] = {
    u8"actor",
    u8"skill_id"
};
const char* SkillTipsReq::Handler::m_alias_list[2] = {
    u8"actor",
    u8"skill_id"
};
const std::bitset<2> SkillTipsReq::Handler::template_assigned_bitset { "11" };

const char* SkillTipsAck::Handler::m_key_list[4] = {
    u8"skill_id",
    u8"tips",
    u8"errcode",
    u8"errmsg"
};
const char* SkillTipsAck::Handler::m_alias_list[4] = {
    u8"skill_id",
    u8"tips",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> SkillTipsAck::Handler::template_assigned_bitset { "1111" };

const char* TeamLeaderOprNtf::Handler::m_key_list[1] = {
    u8"opr"
};
const char* TeamLeaderOprNtf::Handler::m_alias_list[1] = {
    u8"opr"
};
const std::bitset<1> TeamLeaderOprNtf::Handler::template_assigned_bitset { "1" };

const char* KeepAliveCtrlNtf::Handler::m_key_list[1] = {
    u8"opt"
};
const char* KeepAliveCtrlNtf::Handler::m_alias_list[1] = {
    u8"opt"
};
const std::bitset<1> KeepAliveCtrlNtf::Handler::template_assigned_bitset { "1" };

const char* NetDelayReq::Handler::m_key_list[4] = {
    u8"gs_tick",
    u8"gt_tick",
    u8"sg_tick",
    u8"ping"
};
const char* NetDelayReq::Handler::m_alias_list[4] = {
    u8"gs_tick",
    u8"gt_tick",
    u8"sg_tick",
    u8"ping"
};
const std::bitset<4> NetDelayReq::Handler::template_assigned_bitset { "1111" };

const char* NetDelayAck::Handler::m_key_list[7] = {
    u8"gs_elpase",
    u8"gt_elpase",
    u8"sg_elpase",
    u8"gt_cpu",
    u8"baidu_delay",
    u8"taobao_delay",
    u8"sg_id"
};
const char* NetDelayAck::Handler::m_alias_list[7] = {
    u8"gs_elpase",
    u8"gt_elpase",
    u8"sg_elpase",
    u8"gt_cpu",
    u8"baidu_delay",
    u8"taobao_delay",
    u8"sg_id"
};
const std::bitset<7> NetDelayAck::Handler::template_assigned_bitset { "1111111" };

const char* SystemSetupNtf::Handler::m_key_list[1] = {
    u8"attrs"
};
const char* SystemSetupNtf::Handler::m_alias_list[1] = {
    u8"attrs"
};
const std::bitset<1> SystemSetupNtf::Handler::template_assigned_bitset { "1" };

const char* BuffListNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"buffs"
};
const char* BuffListNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"buffs"
};
const std::bitset<2> BuffListNtf::Handler::template_assigned_bitset { "11" };

const char* BuffAddNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"buff"
};
const char* BuffAddNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"buff"
};
const std::bitset<2> BuffAddNtf::Handler::template_assigned_bitset { "11" };

const char* BuffDurationNtf::Handler::m_key_list[3] = {
    u8"guid",
    u8"id",
    u8"duration"
};
const char* BuffDurationNtf::Handler::m_alias_list[3] = {
    u8"guid",
    u8"id",
    u8"duration"
};
const std::bitset<3> BuffDurationNtf::Handler::template_assigned_bitset { "111" };

const char* BuffDynAttrNtf::Handler::m_key_list[3] = {
    u8"guid",
    u8"id",
    u8"dyn_attrs"
};
const char* BuffDynAttrNtf::Handler::m_alias_list[3] = {
    u8"guid",
    u8"id",
    u8"dyn_attrs"
};
const std::bitset<3> BuffDynAttrNtf::Handler::template_assigned_bitset { "111" };

const char* StopBuff::Handler::m_key_list[2] = {
    u8"guid",
    u8"id"
};
const char* StopBuff::Handler::m_alias_list[2] = {
    u8"guid",
    u8"id"
};
const std::bitset<2> StopBuff::Handler::template_assigned_bitset { "11" };

const char* BuffDelNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"id"
};
const char* BuffDelNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"id"
};
const std::bitset<2> BuffDelNtf::Handler::template_assigned_bitset { "11" };

const char* ActivityListAck::Handler::m_key_list[3] = {
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* ActivityListAck::Handler::m_alias_list[3] = {
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> ActivityListAck::Handler::template_assigned_bitset { "111" };

const char* ActivityJoinNtf::Handler::m_key_list[1] = {
    u8"id"
};
const char* ActivityJoinNtf::Handler::m_alias_list[1] = {
    u8"id"
};
const std::bitset<1> ActivityJoinNtf::Handler::template_assigned_bitset { "1" };

const char* GuildListNtf::Handler::m_key_list[3] = {
    u8"pos",
    u8"count",
    u8"guilds"
};
const char* GuildListNtf::Handler::m_alias_list[3] = {
    u8"pos",
    u8"count",
    u8"guilds"
};
const std::bitset<3> GuildListNtf::Handler::template_assigned_bitset { "111" };

const char* GuildNtf::Handler::m_key_list[2] = {
    u8"guild",
    u8"self"
};
const char* GuildNtf::Handler::m_alias_list[2] = {
    u8"guild",
    u8"self"
};
const std::bitset<2> GuildNtf::Handler::template_assigned_bitset { "11" };

const char* GuildBaseNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"name"
};
const char* GuildBaseNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"name"
};
const std::bitset<2> GuildBaseNtf::Handler::template_assigned_bitset { "11" };

const char* GuildNoticeNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"notice"
};
const char* GuildNoticeNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"notice"
};
const std::bitset<2> GuildNoticeNtf::Handler::template_assigned_bitset { "11" };

const char* GuildBriefNtf::Handler::m_key_list[1] = {
    u8"guild"
};
const char* GuildBriefNtf::Handler::m_alias_list[1] = {
    u8"guild"
};
const std::bitset<1> GuildBriefNtf::Handler::template_assigned_bitset { "1" };

const char* GuildMemberListNtf::Handler::m_key_list[4] = {
    u8"guid",
    u8"pos",
    u8"count",
    u8"members"
};
const char* GuildMemberListNtf::Handler::m_alias_list[4] = {
    u8"guid",
    u8"pos",
    u8"count",
    u8"members"
};
const std::bitset<4> GuildMemberListNtf::Handler::template_assigned_bitset { "1111" };

const char* GuildMemberNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"members"
};
const char* GuildMemberNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"members"
};
const std::bitset<2> GuildMemberNtf::Handler::template_assigned_bitset { "11" };

const char* GuildApplicantListNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"applicants"
};
const char* GuildApplicantListNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"applicants"
};
const std::bitset<2> GuildApplicantListNtf::Handler::template_assigned_bitset { "11" };

const char* GuildOperationNtf::Handler::m_key_list[5] = {
    u8"type",
    u8"param1",
    u8"param2",
    u8"errcode",
    u8"errmsg"
};
const char* GuildOperationNtf::Handler::m_alias_list[5] = {
    u8"type",
    u8"param1",
    u8"param2",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<5> GuildOperationNtf::Handler::template_assigned_bitset { "11111" };

const char* GetTimestampAck::Handler::m_key_list[1] = {
    u8"now"
};
const char* GetTimestampAck::Handler::m_alias_list[1] = {
    u8"now"
};
const std::bitset<1> GetTimestampAck::Handler::template_assigned_bitset { "1" };

const char* ContactListNtf::Handler::m_key_list[2] = {
    u8"contact_type",
    u8"contacts"
};
const char* ContactListNtf::Handler::m_alias_list[2] = {
    u8"contact_type",
    u8"contacts"
};
const std::bitset<2> ContactListNtf::Handler::template_assigned_bitset { "11" };

const char* AddContactNtf::Handler::m_key_list[1] = {
    u8"contact"
};
const char* AddContactNtf::Handler::m_alias_list[1] = {
    u8"contact"
};
const std::bitset<1> AddContactNtf::Handler::template_assigned_bitset { "1" };

const char* UpdateContactNtf::Handler::m_key_list[1] = {
    u8"contact"
};
const char* UpdateContactNtf::Handler::m_alias_list[1] = {
    u8"contact"
};
const std::bitset<1> UpdateContactNtf::Handler::template_assigned_bitset { "1" };

const char* DelContactNtf::Handler::m_key_list[1] = {
    u8"target_guid"
};
const char* DelContactNtf::Handler::m_alias_list[1] = {
    u8"target_guid"
};
const std::bitset<1> DelContactNtf::Handler::template_assigned_bitset { "1" };

const char* AddContactMessageNtf::Handler::m_key_list[1] = {
    u8"messages"
};
const char* AddContactMessageNtf::Handler::m_alias_list[1] = {
    u8"messages"
};
const std::bitset<1> AddContactMessageNtf::Handler::template_assigned_bitset { "1" };

const char* ItemQueryNtf::Handler::m_key_list[1] = {
    u8"item"
};
const char* ItemQueryNtf::Handler::m_alias_list[1] = {
    u8"item"
};
const std::bitset<1> ItemQueryNtf::Handler::template_assigned_bitset { "1" };

const char* PetQueryNtf::Handler::m_key_list[1] = {
    u8"pet"
};
const char* PetQueryNtf::Handler::m_alias_list[1] = {
    u8"pet"
};
const std::bitset<1> PetQueryNtf::Handler::template_assigned_bitset { "1" };

const char* ContactInfoNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* ContactInfoNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> ContactInfoNtf::Handler::template_assigned_bitset { "1" };

const char* MailListNtf::Handler::m_key_list[1] = {
    u8"mails"
};
const char* MailListNtf::Handler::m_alias_list[1] = {
    u8"mails"
};
const std::bitset<1> MailListNtf::Handler::template_assigned_bitset { "1" };

const char* AddMailNtf::Handler::m_key_list[1] = {
    u8"mail"
};
const char* AddMailNtf::Handler::m_alias_list[1] = {
    u8"mail"
};
const std::bitset<1> AddMailNtf::Handler::template_assigned_bitset { "1" };

const char* DelMailNtf::Handler::m_key_list[1] = {
    u8"mail_guid"
};
const char* DelMailNtf::Handler::m_alias_list[1] = {
    u8"mail_guid"
};
const std::bitset<1> DelMailNtf::Handler::template_assigned_bitset { "1" };

const char* MailBodyNtf::Handler::m_key_list[2] = {
    u8"mail_guid",
    u8"mail"
};
const char* MailBodyNtf::Handler::m_alias_list[2] = {
    u8"mail_guid",
    u8"mail"
};
const std::bitset<2> MailBodyNtf::Handler::template_assigned_bitset { "11" };

const char* UpdateMailBodyNtf::Handler::m_key_list[2] = {
    u8"mail_guid",
    u8"mail"
};
const char* UpdateMailBodyNtf::Handler::m_alias_list[2] = {
    u8"mail_guid",
    u8"mail"
};
const std::bitset<2> UpdateMailBodyNtf::Handler::template_assigned_bitset { "11" };

const char* UpdateMailHeadNtf::Handler::m_key_list[2] = {
    u8"mail_guid",
    u8"mail"
};
const char* UpdateMailHeadNtf::Handler::m_alias_list[2] = {
    u8"mail_guid",
    u8"mail"
};
const std::bitset<2> UpdateMailHeadNtf::Handler::template_assigned_bitset { "11" };

const char* RanklistReq::Handler::m_key_list[3] = {
    u8"type",
    u8"begin",
    u8"end"
};
const char* RanklistReq::Handler::m_alias_list[3] = {
    u8"type",
    u8"begin",
    u8"end"
};
const std::bitset<3> RanklistReq::Handler::template_assigned_bitset { "111" };

const char* RanklistAck::Handler::m_key_list[5] = {
    u8"type",
    u8"total",
    u8"begin",
    u8"end",
    u8"data"
};
const char* RanklistAck::Handler::m_alias_list[5] = {
    u8"type",
    u8"total",
    u8"begin",
    u8"end",
    u8"data"
};
const std::bitset<5> RanklistAck::Handler::template_assigned_bitset { "11111" };

const char* GetRankReq::Handler::m_key_list[2] = {
    u8"type",
    u8"guid"
};
const char* GetRankReq::Handler::m_alias_list[2] = {
    u8"type",
    u8"guid"
};
const std::bitset<2> GetRankReq::Handler::template_assigned_bitset { "11" };

const char* GetRankAck::Handler::m_key_list[3] = {
    u8"type",
    u8"rank",
    u8"data"
};
const char* GetRankAck::Handler::m_alias_list[3] = {
    u8"type",
    u8"rank",
    u8"data"
};
const std::bitset<3> GetRankAck::Handler::template_assigned_bitset { "111" };

const char* TitleContainerNtf::Handler::m_key_list[1] = {
    u8"titles"
};
const char* TitleContainerNtf::Handler::m_alias_list[1] = {
    u8"titles"
};
const std::bitset<1> TitleContainerNtf::Handler::template_assigned_bitset { "1" };

const char* TitleAddNtf::Handler::m_key_list[1] = {
    u8"title"
};
const char* TitleAddNtf::Handler::m_alias_list[1] = {
    u8"title"
};
const std::bitset<1> TitleAddNtf::Handler::template_assigned_bitset { "1" };

const char* TitleDelNtf::Handler::m_key_list[1] = {
    u8"title_id"
};
const char* TitleDelNtf::Handler::m_alias_list[1] = {
    u8"title_id"
};
const std::bitset<1> TitleDelNtf::Handler::template_assigned_bitset { "1" };

const char* AgentKeyAck::Handler::m_key_list[1] = {
    u8"key"
};
const char* AgentKeyAck::Handler::m_alias_list[1] = {
    u8"key"
};
const std::bitset<1> AgentKeyAck::Handler::template_assigned_bitset { "1" };

const char* HeadMsgNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"msg"
};
const char* HeadMsgNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"msg"
};
const std::bitset<2> HeadMsgNtf::Handler::template_assigned_bitset { "11" };

const char* AutoContainerNtf::Handler::m_key_list[3] = {
    u8"guid",
    u8"is_auto",
    u8"auto_skill"
};
const char* AutoContainerNtf::Handler::m_alias_list[3] = {
    u8"guid",
    u8"is_auto",
    u8"auto_skill"
};
const std::bitset<3> AutoContainerNtf::Handler::template_assigned_bitset { "111" };

const char* PlayerQueryNtf::Handler::m_key_list[1] = {
    u8"brief"
};
const char* PlayerQueryNtf::Handler::m_alias_list[1] = {
    u8"brief"
};
const std::bitset<1> PlayerQueryNtf::Handler::template_assigned_bitset { "1" };

const char* UseAllItem::Handler::m_key_list[2] = {
    u8"item_guid",
    u8"item_amount"
};
const char* UseAllItem::Handler::m_alias_list[2] = {
    u8"item_guid",
    u8"item_amount"
};
const std::bitset<2> UseAllItem::Handler::template_assigned_bitset { "11" };

const char* GuardContainerNtf::Handler::m_key_list[1] = {
    u8"guards"
};
const char* GuardContainerNtf::Handler::m_alias_list[1] = {
    u8"guards"
};
const std::bitset<1> GuardContainerNtf::Handler::template_assigned_bitset { "1" };

const char* GuardAddNtf::Handler::m_key_list[1] = {
    u8"guard"
};
const char* GuardAddNtf::Handler::m_alias_list[1] = {
    u8"guard"
};
const std::bitset<1> GuardAddNtf::Handler::template_assigned_bitset { "1" };

const char* SetGuardLineup::Handler::m_key_list[2] = {
    u8"guid",
    u8"lineup"
};
const char* SetGuardLineup::Handler::m_alias_list[2] = {
    u8"guid",
    u8"lineup"
};
const std::bitset<2> SetGuardLineup::Handler::template_assigned_bitset { "11" };

const char* PetNewAddNtf::Handler::m_key_list[1] = {
    u8"pet_guid"
};
const char* PetNewAddNtf::Handler::m_alias_list[1] = {
    u8"pet_guid"
};
const std::bitset<1> PetNewAddNtf::Handler::template_assigned_bitset { "1" };

const char* TeamPlatformNtf::Handler::m_key_list[1] = {
    u8"teams"
};
const char* TeamPlatformNtf::Handler::m_alias_list[1] = {
    u8"teams"
};
const std::bitset<1> TeamPlatformNtf::Handler::template_assigned_bitset { "1" };

const char* TeamApplicantsNtf::Handler::m_key_list[2] = {
    u8"team_guid",
    u8"applicants"
};
const char* TeamApplicantsNtf::Handler::m_alias_list[2] = {
    u8"team_guid",
    u8"applicants"
};
const std::bitset<2> TeamApplicantsNtf::Handler::template_assigned_bitset { "11" };

const char* TeamOperationNtf::Handler::m_key_list[3] = {
    u8"type",
    u8"errcode",
    u8"errmsg"
};
const char* TeamOperationNtf::Handler::m_alias_list[3] = {
    u8"type",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> TeamOperationNtf::Handler::template_assigned_bitset { "111" };

const char* TeamTargetNtf::Handler::m_key_list[8] = {
    u8"team_guid",
    u8"target",
    u8"permission",
    u8"min_require_level",
    u8"max_require_level",
    u8"leader_guid",
    u8"min_require_reinc",
    u8"max_require_reinc"
};
const char* TeamTargetNtf::Handler::m_alias_list[8] = {
    u8"team_guid",
    u8"target",
    u8"permission",
    u8"min_require_level",
    u8"max_require_level",
    u8"leader_guid",
    u8"min_require_reinc",
    u8"max_require_reinc"
};
const std::bitset<8> TeamTargetNtf::Handler::template_assigned_bitset { "11111111" };

const char* ChangedNameNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"name"
};
const char* ChangedNameNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"name"
};
const std::bitset<2> ChangedNameNtf::Handler::template_assigned_bitset { "11" };

const char* CustomDataNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"custom"
};
const char* CustomDataNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"custom"
};
const std::bitset<2> CustomDataNtf::Handler::template_assigned_bitset { "11" };

const char* SpeedCheckNtf::Handler::m_key_list[2] = {
    u8"type",
    u8"pct"
};
const char* SpeedCheckNtf::Handler::m_alias_list[2] = {
    u8"type",
    u8"pct"
};
const std::bitset<2> SpeedCheckNtf::Handler::template_assigned_bitset { "11" };

const char* ConsoleMsgNtf::Handler::m_key_list[1] = {
    u8"msg"
};
const char* ConsoleMsgNtf::Handler::m_alias_list[1] = {
    u8"msg"
};
const std::bitset<1> ConsoleMsgNtf::Handler::template_assigned_bitset { "1" };

const char* PetSwapNtf::Handler::m_key_list[3] = {
    u8"container_type",
    u8"pet_guid1",
    u8"pet_guid2"
};
const char* PetSwapNtf::Handler::m_alias_list[3] = {
    u8"container_type",
    u8"pet_guid1",
    u8"pet_guid2"
};
const std::bitset<3> PetSwapNtf::Handler::template_assigned_bitset { "111" };

const char* GuardDestroyNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* GuardDestroyNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> GuardDestroyNtf::Handler::template_assigned_bitset { "1" };

const char* ActivateGuard::Handler::m_key_list[1] = {
    u8"guard_id"
};
const char* ActivateGuard::Handler::m_alias_list[1] = {
    u8"guard_id"
};
const std::bitset<1> ActivateGuard::Handler::template_assigned_bitset { "1" };

const char* ReleaseGuard::Handler::m_key_list[1] = {
    u8"guard_guid"
};
const char* ReleaseGuard::Handler::m_alias_list[1] = {
    u8"guard_guid"
};
const std::bitset<1> ReleaseGuard::Handler::template_assigned_bitset { "1" };

const char* TeamMemberSwapNtf::Handler::m_key_list[2] = {
    u8"member_guid1",
    u8"member_guid2"
};
const char* TeamMemberSwapNtf::Handler::m_alias_list[2] = {
    u8"member_guid1",
    u8"member_guid2"
};
const std::bitset<2> TeamMemberSwapNtf::Handler::template_assigned_bitset { "11" };

const char* GuardSwapNtf::Handler::m_key_list[2] = {
    u8"guard_guid1",
    u8"guard_guid2"
};
const char* GuardSwapNtf::Handler::m_alias_list[2] = {
    u8"guard_guid1",
    u8"guard_guid2"
};
const std::bitset<2> GuardSwapNtf::Handler::template_assigned_bitset { "11" };

const char* PetReplaceNtf::Handler::m_key_list[3] = {
    u8"container_type",
    u8"dst_pet_guid",
    u8"src_pet"
};
const char* PetReplaceNtf::Handler::m_alias_list[3] = {
    u8"container_type",
    u8"dst_pet_guid",
    u8"src_pet"
};
const std::bitset<3> PetReplaceNtf::Handler::template_assigned_bitset { "111" };

const char* GuardAppearNtf::Handler::m_key_list[7] = {
    u8"guid",
    u8"name",
    u8"x",
    u8"y",
    u8"attrs",
    u8"buffs",
    u8"custom"
};
const char* GuardAppearNtf::Handler::m_alias_list[7] = {
    u8"guid",
    u8"name",
    u8"x",
    u8"y",
    u8"attrs",
    u8"buffs",
    u8"custom"
};
const std::bitset<7> GuardAppearNtf::Handler::template_assigned_bitset { "1111111" };

const char* InstructionContainerNtf::Handler::m_key_list[1] = {
    u8"datas"
};
const char* InstructionContainerNtf::Handler::m_alias_list[1] = {
    u8"datas"
};
const std::bitset<1> InstructionContainerNtf::Handler::template_assigned_bitset { "1" };

const char* InstructionAddReq::Handler::m_key_list[2] = {
    u8"type",
    u8"content"
};
const char* InstructionAddReq::Handler::m_alias_list[2] = {
    u8"type",
    u8"content"
};
const std::bitset<2> InstructionAddReq::Handler::template_assigned_bitset { "11" };

const char* InstructionAddAck::Handler::m_key_list[4] = {
    u8"type",
    u8"content",
    u8"errcode",
    u8"errmsg"
};
const char* InstructionAddAck::Handler::m_alias_list[4] = {
    u8"type",
    u8"content",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> InstructionAddAck::Handler::template_assigned_bitset { "1111" };

const char* InstructionDeleteReq::Handler::m_key_list[2] = {
    u8"type",
    u8"index"
};
const char* InstructionDeleteReq::Handler::m_alias_list[2] = {
    u8"type",
    u8"index"
};
const std::bitset<2> InstructionDeleteReq::Handler::template_assigned_bitset { "11" };

const char* InstructionDeleteAck::Handler::m_key_list[4] = {
    u8"type",
    u8"index",
    u8"errcode",
    u8"errmsg"
};
const char* InstructionDeleteAck::Handler::m_alias_list[4] = {
    u8"type",
    u8"index",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> InstructionDeleteAck::Handler::template_assigned_bitset { "1111" };

const char* InstructionModfityReq::Handler::m_key_list[3] = {
    u8"type",
    u8"index",
    u8"content"
};
const char* InstructionModfityReq::Handler::m_alias_list[3] = {
    u8"type",
    u8"index",
    u8"content"
};
const std::bitset<3> InstructionModfityReq::Handler::template_assigned_bitset { "111" };

const char* InstructionModifyAck::Handler::m_key_list[5] = {
    u8"type",
    u8"index",
    u8"content",
    u8"errcode",
    u8"errmsg"
};
const char* InstructionModifyAck::Handler::m_alias_list[5] = {
    u8"type",
    u8"index",
    u8"content",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<5> InstructionModifyAck::Handler::template_assigned_bitset { "11111" };

const char* InstructionDefaultReq::Handler::m_key_list[1] = {
    u8"type"
};
const char* InstructionDefaultReq::Handler::m_alias_list[1] = {
    u8"type"
};
const std::bitset<1> InstructionDefaultReq::Handler::template_assigned_bitset { "1" };

const char* InstructionDefaultAck::Handler::m_key_list[4] = {
    u8"type",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const char* InstructionDefaultAck::Handler::m_alias_list[4] = {
    u8"type",
    u8"datas",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<4> InstructionDefaultAck::Handler::template_assigned_bitset { "1111" };

const char* InstructionAttachReq::Handler::m_key_list[3] = {
    u8"type",
    u8"index",
    u8"target"
};
const char* InstructionAttachReq::Handler::m_alias_list[3] = {
    u8"type",
    u8"index",
    u8"target"
};
const std::bitset<3> InstructionAttachReq::Handler::template_assigned_bitset { "111" };

const char* InstructionAttachAck::Handler::m_key_list[5] = {
    u8"type",
    u8"index",
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const char* InstructionAttachAck::Handler::m_alias_list[5] = {
    u8"type",
    u8"index",
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<5> InstructionAttachAck::Handler::template_assigned_bitset { "11111" };

const char* InstructionAttachNtf::Handler::m_key_list[2] = {
    u8"fighter_id",
    u8"content"
};
const char* InstructionAttachNtf::Handler::m_alias_list[2] = {
    u8"fighter_id",
    u8"content"
};
const std::bitset<2> InstructionAttachNtf::Handler::template_assigned_bitset { "11" };

const char* InstructionDetachReq::Handler::m_key_list[1] = {
    u8"target"
};
const char* InstructionDetachReq::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> InstructionDetachReq::Handler::template_assigned_bitset { "1" };

const char* InstructionDetachAck::Handler::m_key_list[3] = {
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const char* InstructionDetachAck::Handler::m_alias_list[3] = {
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> InstructionDetachAck::Handler::template_assigned_bitset { "111" };

const char* InstructionDetachNtf::Handler::m_key_list[1] = {
    u8"fighter_id"
};
const char* InstructionDetachNtf::Handler::m_alias_list[1] = {
    u8"fighter_id"
};
const std::bitset<1> InstructionDetachNtf::Handler::template_assigned_bitset { "1" };

const char* PlayerDetailNtf::Handler::m_key_list[1] = {
    u8"data"
};
const char* PlayerDetailNtf::Handler::m_alias_list[1] = {
    u8"data"
};
const std::bitset<1> PlayerDetailNtf::Handler::template_assigned_bitset { "1" };

const char* MapDynBlockPtNtf::Handler::m_key_list[2] = {
    u8"map_id",
    u8"block_pt_list"
};
const char* MapDynBlockPtNtf::Handler::m_alias_list[2] = {
    u8"map_id",
    u8"block_pt_list"
};
const std::bitset<2> MapDynBlockPtNtf::Handler::template_assigned_bitset { "11" };

const char* GuardQueryNtf::Handler::m_key_list[1] = {
    u8"guard"
};
const char* GuardQueryNtf::Handler::m_alias_list[1] = {
    u8"guard"
};
const std::bitset<1> GuardQueryNtf::Handler::template_assigned_bitset { "1" };

const char* BuyBackNtfEx::Handler::m_key_list[2] = {
    u8"buy_guid",
    u8"buy_amount"
};
const char* BuyBackNtfEx::Handler::m_alias_list[2] = {
    u8"buy_guid",
    u8"buy_amount"
};
const std::bitset<2> BuyBackNtfEx::Handler::template_assigned_bitset { "11" };

const char* GuildCustomNtf::Handler::m_key_list[3] = {
    u8"player_guid",
    u8"is_guild",
    u8"customs"
};
const char* GuildCustomNtf::Handler::m_alias_list[3] = {
    u8"player_guid",
    u8"is_guild",
    u8"customs"
};
const std::bitset<3> GuildCustomNtf::Handler::template_assigned_bitset { "111" };

const char* PreTurnRoundNtf::Handler::m_key_list[1] = {
    u8"round"
};
const char* PreTurnRoundNtf::Handler::m_alias_list[1] = {
    u8"round"
};
const std::bitset<1> PreTurnRoundNtf::Handler::template_assigned_bitset { "1" };

const char* FighterSpecialPetNtf::Handler::m_key_list[1] = {
    u8"special_pet"
};
const char* FighterSpecialPetNtf::Handler::m_alias_list[1] = {
    u8"special_pet"
};
const std::bitset<1> FighterSpecialPetNtf::Handler::template_assigned_bitset { "1" };

