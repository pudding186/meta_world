#include <new>

#include "FightData.hpp"

template<>
const unsigned short Protocol<ActionBegin>::module_id = 108;
template<>
const unsigned short Protocol<ActionBegin>::protocol_id = 1;

const char* ActionBegin::Handler::m_key_list[1] = {
    u8"target"
};
const char* ActionBegin::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> ActionBegin::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<ActionEnd>::module_id = 108;
template<>
const unsigned short Protocol<ActionEnd>::protocol_id = 2;

const char* ActionEnd::Handler::m_key_list[2] = {
    u8"target",
    u8"state"
};
const char* ActionEnd::Handler::m_alias_list[2] = {
    u8"target",
    u8"state"
};
const std::bitset<2> ActionEnd::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<AddBuff>::module_id = 108;
template<>
const unsigned short Protocol<AddBuff>::protocol_id = 3;

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

template<>
const unsigned short Protocol<DelBuff>::module_id = 108;
template<>
const unsigned short Protocol<DelBuff>::protocol_id = 4;

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

template<>
const unsigned short Protocol<DiedNtf>::module_id = 108;
template<>
const unsigned short Protocol<DiedNtf>::protocol_id = 5;

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

template<>
const unsigned short Protocol<ReviveNtf>::module_id = 108;
template<>
const unsigned short Protocol<ReviveNtf>::protocol_id = 6;

const char* ReviveNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"hp"
};
const char* ReviveNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"hp"
};
const std::bitset<2> ReviveNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<PaoPaoNtf>::module_id = 108;
template<>
const unsigned short Protocol<PaoPaoNtf>::protocol_id = 7;

const char* PaoPaoNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"text"
};
const char* PaoPaoNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"text"
};
const std::bitset<2> PaoPaoNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<FightAttrNtf>::module_id = 108;
template<>
const unsigned short Protocol<FightAttrNtf>::protocol_id = 8;

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

template<>
const unsigned short Protocol<ShotSKillNtf>::module_id = 108;
template<>
const unsigned short Protocol<ShotSKillNtf>::protocol_id = 9;

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

template<>
const unsigned short Protocol<SkillCooldownNtf>::module_id = 108;
template<>
const unsigned short Protocol<SkillCooldownNtf>::protocol_id = 10;

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

template<>
const unsigned short Protocol<ShowAttrChangeNtf>::module_id = 108;
template<>
const unsigned short Protocol<ShowAttrChangeNtf>::protocol_id = 11;

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

template<>
const unsigned short Protocol<MissNtf>::module_id = 108;
template<>
const unsigned short Protocol<MissNtf>::protocol_id = 12;

const char* MissNtf::Handler::m_key_list[1] = {
    u8"target"
};
const char* MissNtf::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> MissNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<DefenseNtf>::module_id = 108;
template<>
const unsigned short Protocol<DefenseNtf>::protocol_id = 13;

const char* DefenseNtf::Handler::m_key_list[1] = {
    u8"target"
};
const char* DefenseNtf::Handler::m_alias_list[1] = {
    u8"target"
};
const std::bitset<1> DefenseNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<EscapeNtf>::module_id = 108;
template<>
const unsigned short Protocol<EscapeNtf>::protocol_id = 14;

const char* EscapeNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"result"
};
const char* EscapeNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"result"
};
const std::bitset<2> EscapeNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<ProtectNtf>::module_id = 108;
template<>
const unsigned short Protocol<ProtectNtf>::protocol_id = 15;

const char* ProtectNtf::Handler::m_key_list[2] = {
    u8"actor",
    u8"target"
};
const char* ProtectNtf::Handler::m_alias_list[2] = {
    u8"actor",
    u8"target"
};
const std::bitset<2> ProtectNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<HuaWuNtf>::module_id = 108;
template<>
const unsigned short Protocol<HuaWuNtf>::protocol_id = 16;

const char* HuaWuNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"skillid"
};
const char* HuaWuNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"skillid"
};
const std::bitset<2> HuaWuNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<FailNtf>::module_id = 108;
template<>
const unsigned short Protocol<FailNtf>::protocol_id = 17;

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

template<>
const unsigned short Protocol<SummonNtf>::module_id = 108;
template<>
const unsigned short Protocol<SummonNtf>::protocol_id = 18;

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

template<>
const unsigned short Protocol<ShowFighterNtf>::module_id = 108;
template<>
const unsigned short Protocol<ShowFighterNtf>::protocol_id = 19;

const char* ShowFighterNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"replace"
};
const char* ShowFighterNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"replace"
};
const std::bitset<2> ShowFighterNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<HideFighterNtf>::module_id = 108;
template<>
const unsigned short Protocol<HideFighterNtf>::protocol_id = 20;

const char* HideFighterNtf::Handler::m_key_list[2] = {
    u8"target",
    u8"effect"
};
const char* HideFighterNtf::Handler::m_alias_list[2] = {
    u8"target",
    u8"effect"
};
const std::bitset<2> HideFighterNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<CatchNtf>::module_id = 108;
template<>
const unsigned short Protocol<CatchNtf>::protocol_id = 21;

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

template<>
const unsigned short Protocol<ShowEffectNtf>::module_id = 108;
template<>
const unsigned short Protocol<ShowEffectNtf>::protocol_id = 22;

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

template<>
const unsigned short Protocol<StealSkillNtf>::module_id = 108;
template<>
const unsigned short Protocol<StealSkillNtf>::protocol_id = 23;

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

template<>
const unsigned short Protocol<ForgetNtf>::module_id = 108;
template<>
const unsigned short Protocol<ForgetNtf>::protocol_id = 24;

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

