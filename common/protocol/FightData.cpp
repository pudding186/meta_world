#include <new>

#include "FightData.hpp"

const char* ActionBegin::Handler::m_key_list[1] = {
    u8"target"
};
const char* ActionBegin::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> ActionBegin::Handler::template_assigned_bitset { "1" };

const char* ActionEnd::Handler::m_key_list[2] = {
    u8"target",
    u8"state"
};
const char* ActionEnd::Handler::m_alias_list[2] = {
    u8"target",
    u8"state"
};
const std::bitset<2> ActionEnd::Handler::template_assigned_bitset { "11" };

const char* AddBuff::Handler::m_key_list[5] = {
    u8"target",
    u8"skill",
    u8"buff",
    u8"stack",
    u8"remain"
};
const char* AddBuff::Handler::m_alias_list[5] = {
    u8"target",
    u8"skill",
    u8"buff",
    u8"stack",
    u8"remain"
};
const std::bitset<5> AddBuff::Handler::template_assigned_bitset { "11111" };

const char* DelBuff::Handler::m_key_list[4] = {
    u8"target",
    u8"buff",
    u8"stack",
    u8"remain"
};
const char* DelBuff::Handler::m_alias_list[4] = {
    u8"target",
    u8"buff",
    u8"stack",
    u8"remain"
};
const std::bitset<4> DelBuff::Handler::template_assigned_bitset { "1111" };

const char* DiedNtf::Handler::m_key_list[3] = {
    u8"target",
    u8"leave",
    u8"effect"
};
const char* DiedNtf::Handler::m_alias_list[3] = {
    u8"target",
    u8"leave",
    u8"effect"
};
const std::bitset<3> DiedNtf::Handler::template_assigned_bitset { "111" };

const char* ReviveNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"hp"
};
const char* ReviveNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"hp"
};
const std::bitset<2> ReviveNtf::Handler::template_assigned_bitset { "11" };

const char* PaoPaoNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"text"
};
const char* PaoPaoNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"text"
};
const std::bitset<2> PaoPaoNtf::Handler::template_assigned_bitset { "11" };

const char* FightAttrNtf::Handler::m_key_list[3] = {
    u8"target",
    u8"attr",
    u8"value"
};
const char* FightAttrNtf::Handler::m_alias_list[3] = {
    u8"target",
    u8"attr",
    u8"value"
};
const std::bitset<3> FightAttrNtf::Handler::template_assigned_bitset { "111" };

const char* ShotSKillNtf::Handler::m_key_list[4] = {
    u8"actor",
    u8"skillid",
    u8"targets",
    u8"mastery"
};
const char* ShotSKillNtf::Handler::m_alias_list[4] = {
    u8"actor",
    u8"skillid",
    u8"targets",
    u8"mastery"
};
const std::bitset<4> ShotSKillNtf::Handler::template_assigned_bitset { "1111" };

const char* SkillCooldownNtf::Handler::m_key_list[3] = {
    u8"target",
    u8"skillid",
    u8"cd"
};
const char* SkillCooldownNtf::Handler::m_alias_list[3] = {
    u8"target",
    u8"skillid",
    u8"cd"
};
const std::bitset<3> SkillCooldownNtf::Handler::template_assigned_bitset { "111" };

const char* ShowAttrChangeNtf::Handler::m_key_list[6] = {
    u8"target",
    u8"attr",
    u8"value",
    u8"type",
    u8"burst",
    u8"ignore_phydef"
};
const char* ShowAttrChangeNtf::Handler::m_alias_list[6] = {
    u8"target",
    u8"attr",
    u8"value",
    u8"type",
    u8"burst",
    u8"ignore_phydef"
};
const std::bitset<6> ShowAttrChangeNtf::Handler::template_assigned_bitset { "111111" };

const char* MissNtf::Handler::m_key_list[1] = {
    u8"target"
};
const char* MissNtf::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> MissNtf::Handler::template_assigned_bitset { "1" };

const char* DefenseNtf::Handler::m_key_list[1] = {
    u8"target"
};
const char* DefenseNtf::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> DefenseNtf::Handler::template_assigned_bitset { "1" };

const char* EscapeNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"result"
};
const char* EscapeNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"result"
};
const std::bitset<2> EscapeNtf::Handler::template_assigned_bitset { "11" };

const char* ProtectNtf::Handler::m_key_list[2] = {
    u8"actor",
    u8"target"
};
const char* ProtectNtf::Handler::m_alias_list[2] = {
    u8"actor",
    u8"target"
};
const std::bitset<2> ProtectNtf::Handler::template_assigned_bitset { "11" };

const char* HuaWuNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"skillid"
};
const char* HuaWuNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"skillid"
};
const std::bitset<2> HuaWuNtf::Handler::template_assigned_bitset { "11" };

const char* FailNtf::Handler::m_key_list[3] = {
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const char* FailNtf::Handler::m_alias_list[3] = {
    u8"target",
    u8"errcode",
    u8"errmsg"
};
const std::bitset<3> FailNtf::Handler::template_assigned_bitset { "111" };

const char* SummonNtf::Handler::m_key_list[4] = {
    u8"actor",
    u8"action",
    u8"target1",
    u8"target2"
};
const char* SummonNtf::Handler::m_alias_list[4] = {
    u8"actor",
    u8"action",
    u8"target1",
    u8"target2"
};
const std::bitset<4> SummonNtf::Handler::template_assigned_bitset { "1111" };

const char* ShowFighterNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"replace"
};
const char* ShowFighterNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"replace"
};
const std::bitset<2> ShowFighterNtf::Handler::template_assigned_bitset { "11" };

const char* HideFighterNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"effect"
};
const char* HideFighterNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"effect"
};
const std::bitset<2> HideFighterNtf::Handler::template_assigned_bitset { "11" };

const char* CatchNtf::Handler::m_key_list[3] = {
    u8"actor",
    u8"target",
    u8"result"
};
const char* CatchNtf::Handler::m_alias_list[3] = {
    u8"actor",
    u8"target",
    u8"result"
};
const std::bitset<3> CatchNtf::Handler::template_assigned_bitset { "111" };

const char* ShowEffectNtf::Handler::m_key_list[5] = {
    u8"type",
    u8"actor",
    u8"target",
    u8"param1",
    u8"param2"
};
const char* ShowEffectNtf::Handler::m_alias_list[5] = {
    u8"type",
    u8"actor",
    u8"target",
    u8"param1",
    u8"param2"
};
const std::bitset<5> ShowEffectNtf::Handler::template_assigned_bitset { "11111" };

const char* StealSkillNtf::Handler::m_key_list[3] = {
    u8"target",
    u8"skill",
    u8"performance"
};
const char* StealSkillNtf::Handler::m_alias_list[3] = {
    u8"target",
    u8"skill",
    u8"performance"
};
const std::bitset<3> StealSkillNtf::Handler::template_assigned_bitset { "111" };

const char* ForgetNtf::Handler::m_key_list[3] = {
    u8"target",
    u8"skillid",
    u8"forget"
};
const char* ForgetNtf::Handler::m_alias_list[3] = {
    u8"target",
    u8"skillid",
    u8"forget"
};
const std::bitset<3> ForgetNtf::Handler::template_assigned_bitset { "111" };

