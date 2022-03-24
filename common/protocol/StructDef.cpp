#include <new>

#include "StructDef.hpp"

const char* CustomStrData::Handler::m_key_list[2] = {
    u8"key",
    u8"value"
};
const char* CustomStrData::Handler::m_alias_list[2] = {
    u8"k",
    u8"v"
};
const std::bitset<2> CustomStrData::Handler::template_assigned_bitset { "11" };

const char* CustomIntData::Handler::m_key_list[2] = {
    u8"key",
    u8"value"
};
const char* CustomIntData::Handler::m_alias_list[2] = {
    u8"k",
    u8"v"
};
const std::bitset<2> CustomIntData::Handler::template_assigned_bitset { "11" };

const char* CustomData::Handler::m_key_list[2] = {
    u8"strdata",
    u8"intdata"
};
const char* CustomData::Handler::m_alias_list[2] = {
    u8"s",
    u8"i"
};
const std::bitset<2> CustomData::Handler::template_assigned_bitset { "11" };

const char* SkillData::Handler::m_key_list[6] = {
    u8"id",
    u8"performance",
    u8"max_performance",
    u8"tick",
    u8"enable",
    u8"bind"
};
const char* SkillData::Handler::m_alias_list[6] = {
    u8"i",
    u8"p",
    u8"mp",
    u8"t",
    u8"e",
    u8"b"
};
const std::bitset<6> SkillData::Handler::template_assigned_bitset { "111111" };

const char* DynAttrData::Handler::m_key_list[3] = {
    u8"mark",
    u8"attr",
    u8"value"
};
const char* DynAttrData::Handler::m_alias_list[3] = {
    u8"m",
    u8"a",
    u8"v"
};
const std::bitset<3> DynAttrData::Handler::template_assigned_bitset { "111" };

const char* BuffData::Handler::m_key_list[4] = {
    u8"buff_id",
    u8"start_times",
    u8"duration",
    u8"dyn_attrs"
};
const char* BuffData::Handler::m_alias_list[4] = {
    u8"i",
    u8"st",
    u8"d",
    u8"dyn"
};
const std::bitset<4> BuffData::Handler::template_assigned_bitset { "1111" };

const char* QuestData::Handler::m_key_list[9] = {
    u8"quest_id",
    u8"quest_state",
    u8"accept_npc",
    u8"commit_npc",
    u8"quest_store_id",
    u8"accpet_time",
    u8"limit_time",
    u8"changed_time",
    u8"ext"
};
const char* QuestData::Handler::m_alias_list[9] = {
    u8"qi",
    u8"qs",
    u8"an",
    u8"cn",
    u8"qsi",
    u8"at",
    u8"lt",
    u8"ct",
    u8"e"
};
const std::bitset<9> QuestData::Handler::template_assigned_bitset { "111111111" };

const char* QuestRingData::Handler::m_key_list[2] = {
    u8"quest_id",
    u8"quest_ring"
};
const char* QuestRingData::Handler::m_alias_list[2] = {
    u8"qi",
    u8"qr"
};
const std::bitset<2> QuestRingData::Handler::template_assigned_bitset { "11" };

const char* QuestStoreData::Handler::m_key_list[8] = {
    u8"quest_store_id",
    u8"ring_auto",
    u8"ring_day",
    u8"ring_week",
    u8"ring_display",
    u8"quest_rings",
    u8"last_reset_day",
    u8"last_reset_week"
};
const char* QuestStoreData::Handler::m_alias_list[8] = {
    u8"qsi",
    u8"ring_auto",
    u8"ring_day",
    u8"ring_week",
    u8"ring_display",
    u8"quest_rings",
    u8"last_reset_day",
    u8"last_reset_week"
};
const std::bitset<8> QuestStoreData::Handler::template_assigned_bitset { "11111111" };

const char* QuestCountData::Handler::m_key_list[2] = {
    u8"quest_id",
    u8"count"
};
const char* QuestCountData::Handler::m_alias_list[2] = {
    u8"qi",
    u8"c"
};
const std::bitset<2> QuestCountData::Handler::template_assigned_bitset { "11" };

const char* QuestContainerData::Handler::m_key_list[9] = {
    u8"quests",
    u8"npc_visible",
    u8"npc_invisible",
    u8"stores",
    u8"abandoned",
    u8"finished",
    u8"activated",
    u8"failed_counts",
    u8"finished_counts"
};
const char* QuestContainerData::Handler::m_alias_list[9] = {
    u8"i",
    u8"nv",
    u8"niv",
    u8"s",
    u8"ab",
    u8"fi",
    u8"ac",
    u8"fac",
    u8"fic"
};
const std::bitset<9> QuestContainerData::Handler::template_assigned_bitset { "111111111" };

const char* AttrData::Handler::m_key_list[2] = {
    u8"attr",
    u8"value"
};
const char* AttrData::Handler::m_alias_list[2] = {
    u8"a",
    u8"v"
};
const std::bitset<2> AttrData::Handler::template_assigned_bitset { "11" };

const char* ItemData::Handler::m_key_list[10] = {
    u8"guid",
    u8"id",
    u8"role",
    u8"site",
    u8"battlesite",
    u8"amount",
    u8"isbound",
    u8"life",
    u8"dyn_attrs",
    u8"customs"
};
const char* ItemData::Handler::m_alias_list[10] = {
    u8"g",
    u8"i",
    u8"r",
    u8"s",
    u8"bs",
    u8"a",
    u8"ib",
    u8"l",
    u8"da",
    u8"c"
};
const std::bitset<10> ItemData::Handler::template_assigned_bitset { "1111111111" };

const char* ItemContainerData::Handler::m_key_list[3] = {
    u8"container_type",
    u8"capacity",
    u8"items"
};
const char* ItemContainerData::Handler::m_alias_list[3] = {
    u8"ct",
    u8"c",
    u8"i"
};
const std::bitset<3> ItemContainerData::Handler::template_assigned_bitset { "111" };

const char* UserData::Handler::m_key_list[9] = {
    u8"uid",
    u8"sid",
    u8"uname",
    u8"billinyb",
    u8"billoutyb",
    u8"gameinyb",
    u8"gameoutyb",
    u8"createtime",
    u8"lastlogin"
};
const char* UserData::Handler::m_alias_list[9] = {
    u8"uid",
    u8"sid",
    u8"uname",
    u8"billinyb",
    u8"billoutyb",
    u8"gameinyb",
    u8"gameoutyb",
    u8"createtime",
    u8"lastlogin"
};
const std::bitset<9> UserData::Handler::template_assigned_bitset { "111111111" };

const char* SceneData::Handler::m_key_list[3] = {
    u8"map_guid",
    u8"x",
    u8"y"
};
const char* SceneData::Handler::m_alias_list[3] = {
    u8"map_guid",
    u8"x",
    u8"y"
};
const std::bitset<3> SceneData::Handler::template_assigned_bitset { "111" };

const char* TicketData::Handler::m_key_list[5] = {
    u8"map_guid",
    u8"create_time",
    u8"id",
    u8"x",
    u8"y"
};
const char* TicketData::Handler::m_alias_list[5] = {
    u8"map_guid",
    u8"create_time",
    u8"id",
    u8"x",
    u8"y"
};
const std::bitset<5> TicketData::Handler::template_assigned_bitset { "11111" };

const char* PetData::Handler::m_key_list[9] = {
    u8"guid",
    u8"name",
    u8"equips",
    u8"attrs",
    u8"extra_attrs",
    u8"skills",
    u8"buffs",
    u8"dyn_attrs",
    u8"customs"
};
const char* PetData::Handler::m_alias_list[9] = {
    u8"g",
    u8"n",
    u8"e",
    u8"a",
    u8"ea",
    u8"s",
    u8"b",
    u8"da",
    u8"c"
};
const std::bitset<9> PetData::Handler::template_assigned_bitset { "111111111" };

const char* PetContainerData::Handler::m_key_list[3] = {
    u8"container_type",
    u8"capacity",
    u8"pets"
};
const char* PetContainerData::Handler::m_alias_list[3] = {
    u8"ct",
    u8"c",
    u8"p"
};
const std::bitset<3> PetContainerData::Handler::template_assigned_bitset { "111" };

const char* TitleData::Handler::m_key_list[3] = {
    u8"title_id",
    u8"end_time",
    u8"custom"
};
const char* TitleData::Handler::m_alias_list[3] = {
    u8"ti",
    u8"et",
    u8"c"
};
const std::bitset<3> TitleData::Handler::template_assigned_bitset { "111" };

const char* TitleContainerData::Handler::m_key_list[1] = {
    u8"titles"
};
const char* TitleContainerData::Handler::m_alias_list[1] = {
    u8"t"
};
const std::bitset<1> TitleContainerData::Handler::template_assigned_bitset { "1" };

const char* SeatBasic::Handler::m_key_list[3] = {
    u8"id",
    u8"level",
    u8"score"
};
const char* SeatBasic::Handler::m_alias_list[3] = {
    u8"id",
    u8"level",
    u8"score"
};
const std::bitset<3> SeatBasic::Handler::template_assigned_bitset { "111" };

const char* SeatData::Handler::m_key_list[1] = {
    u8"seats"
};
const char* SeatData::Handler::m_alias_list[1] = {
    u8"seats"
};
const std::bitset<1> SeatData::Handler::template_assigned_bitset { "1" };

const char* LineupBasic::Handler::m_key_list[2] = {
    u8"seat_id",
    u8"lineup"
};
const char* LineupBasic::Handler::m_alias_list[2] = {
    u8"seat_id",
    u8"lineup"
};
const std::bitset<2> LineupBasic::Handler::template_assigned_bitset { "11" };

const char* LineupData::Handler::m_key_list[2] = {
    u8"index",
    u8"lineups"
};
const char* LineupData::Handler::m_alias_list[2] = {
    u8"index",
    u8"lineups"
};
const std::bitset<2> LineupData::Handler::template_assigned_bitset { "11" };

const char* InstructionBasic::Handler::m_key_list[2] = {
    u8"type",
    u8"content"
};
const char* InstructionBasic::Handler::m_alias_list[2] = {
    u8"type",
    u8"content"
};
const std::bitset<2> InstructionBasic::Handler::template_assigned_bitset { "11" };

const char* PetLineupData::Handler::m_key_list[1] = {
    u8"lineup"
};
const char* PetLineupData::Handler::m_alias_list[1] = {
    u8"lineup"
};
const std::bitset<1> PetLineupData::Handler::template_assigned_bitset { "1" };

const char* InstructionData::Handler::m_key_list[2] = {
    u8"frienddata",
    u8"enemydata"
};
const char* InstructionData::Handler::m_alias_list[2] = {
    u8"frienddata",
    u8"enemydata"
};
const std::bitset<2> InstructionData::Handler::template_assigned_bitset { "11" };

const char* EmojiData::Handler::m_key_list[1] = {
    u8"emojis"
};
const char* EmojiData::Handler::m_alias_list[1] = {
    u8"emojis"
};
const std::bitset<1> EmojiData::Handler::template_assigned_bitset { "1" };

const char* PlayerBrief::Handler::m_key_list[14] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"unid",
    u8"name",
    u8"attrs",
    u8"map",
    u8"x",
    u8"y",
    u8"state",
    u8"createtime",
    u8"lastlogin",
    u8"destroytime",
    u8"customs"
};
const char* PlayerBrief::Handler::m_alias_list[14] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"unid",
    u8"name",
    u8"attrs",
    u8"map",
    u8"x",
    u8"y",
    u8"state",
    u8"createtime",
    u8"lastlogin",
    u8"destroytime",
    u8"customs"
};
const std::bitset<14> PlayerBrief::Handler::template_assigned_bitset { "11111111111111" };

const char* GuardData::Handler::m_key_list[8] = {
    u8"guid",
    u8"equips",
    u8"attrs",
    u8"extra_attrs",
    u8"skills",
    u8"buffs",
    u8"dyn_attrs",
    u8"customs"
};
const char* GuardData::Handler::m_alias_list[8] = {
    u8"g",
    u8"e",
    u8"a",
    u8"ea",
    u8"s",
    u8"b",
    u8"da",
    u8"c"
};
const std::bitset<8> GuardData::Handler::template_assigned_bitset { "11111111" };

const char* GuardContainerData::Handler::m_key_list[1] = {
    u8"guards"
};
const char* GuardContainerData::Handler::m_alias_list[1] = {
    u8"g"
};
const std::bitset<1> GuardContainerData::Handler::template_assigned_bitset { "1" };

const char* PlayerData::Handler::m_key_list[36] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"unid",
    u8"name",
    u8"state",
    u8"createtime",
    u8"destroytime",
    u8"lastlogin",
    u8"lastloginip",
    u8"lastlogout",
    u8"onlines",
    u8"cur_map",
    u8"last_common_map",
    u8"attrs",
    u8"dyn_attrs",
    u8"extra_attrs",
    u8"items",
    u8"skills",
    u8"buffs",
    u8"quests",
    u8"pets",
    u8"guards",
    u8"tickets",
    u8"customs",
    u8"gameinyb",
    u8"gameoutyb",
    u8"billinyb",
    u8"billoutyb",
    u8"titles",
    u8"username",
    u8"lineups",
    u8"seats",
    u8"instructions",
    u8"pet_lineups",
    u8"emojis"
};
const char* PlayerData::Handler::m_alias_list[36] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"unid",
    u8"name",
    u8"state",
    u8"createtime",
    u8"destroytime",
    u8"lastlogin",
    u8"lastloginip",
    u8"lastlogout",
    u8"onlines",
    u8"cur_map",
    u8"last_common_map",
    u8"attrs",
    u8"dyn_attrs",
    u8"extra_attrs",
    u8"items",
    u8"skills",
    u8"buffs",
    u8"quests",
    u8"pets",
    u8"guards",
    u8"tickets",
    u8"customs",
    u8"gameinyb",
    u8"gameoutyb",
    u8"billinyb",
    u8"billoutyb",
    u8"titles",
    u8"username",
    u8"lineups",
    u8"seats",
    u8"instructions",
    u8"pet_lineups",
    u8"emojis"
};
const std::bitset<36> PlayerData::Handler::template_assigned_bitset { "011111111111111111111111111111111111" };

const char* MapRegion::Handler::m_key_list[4] = {
    u8"min_x",
    u8"max_x",
    u8"min_y",
    u8"max_y"
};
const char* MapRegion::Handler::m_alias_list[4] = {
    u8"min_x",
    u8"max_x",
    u8"min_y",
    u8"max_y"
};
const std::bitset<4> MapRegion::Handler::template_assigned_bitset { "1111" };

const char* ItemAttrValue64::Handler::m_key_list[2] = {
    u8"attr_type",
    u8"attr_value"
};
const char* ItemAttrValue64::Handler::m_alias_list[2] = {
    u8"attr_type",
    u8"attr_value"
};
const std::bitset<2> ItemAttrValue64::Handler::template_assigned_bitset { "11" };

const char* ItemAttrValueS::Handler::m_key_list[2] = {
    u8"attr_type",
    u8"attr_value"
};
const char* ItemAttrValueS::Handler::m_alias_list[2] = {
    u8"attr_type",
    u8"attr_value"
};
const std::bitset<2> ItemAttrValueS::Handler::template_assigned_bitset { "11" };

const char* ItemAttrValueS64::Handler::m_key_list[2] = {
    u8"attr_key",
    u8"attr_value"
};
const char* ItemAttrValueS64::Handler::m_alias_list[2] = {
    u8"attr_key",
    u8"attr_value"
};
const std::bitset<2> ItemAttrValueS64::Handler::template_assigned_bitset { "11" };

const char* ItemAttrValueSS::Handler::m_key_list[2] = {
    u8"attr_key",
    u8"attr_value"
};
const char* ItemAttrValueSS::Handler::m_alias_list[2] = {
    u8"attr_key",
    u8"attr_value"
};
const std::bitset<2> ItemAttrValueSS::Handler::template_assigned_bitset { "11" };

const char* ItemAttrValueList::Handler::m_key_list[6] = {
    u8"item_guid",
    u8"item_values_64",
    u8"item_values_s",
    u8"item_values_s64",
    u8"item_values_ss",
    u8"dyn_attrs"
};
const char* ItemAttrValueList::Handler::m_alias_list[6] = {
    u8"item_guid",
    u8"item_values_64",
    u8"item_values_s",
    u8"item_values_s64",
    u8"item_values_ss",
    u8"dyn_attrs"
};
const std::bitset<6> ItemAttrValueList::Handler::template_assigned_bitset { "111111" };

const char* ChatMsg::Handler::m_key_list[4] = {
    u8"role_name",
    u8"role_id",
    u8"message",
    u8"vip"
};
const char* ChatMsg::Handler::m_alias_list[4] = {
    u8"role_name",
    u8"role_id",
    u8"message",
    u8"vip"
};
const std::bitset<4> ChatMsg::Handler::template_assigned_bitset { "1111" };

const char* ParamData::Handler::m_key_list[2] = {
    u8"type",
    u8"param"
};
const char* ParamData::Handler::m_alias_list[2] = {
    u8"type",
    u8"param"
};
const std::bitset<2> ParamData::Handler::template_assigned_bitset { "11" };

const char* FightPetData::Handler::m_key_list[2] = {
    u8"guid",
    u8"state"
};
const char* FightPetData::Handler::m_alias_list[2] = {
    u8"guid",
    u8"state"
};
const std::bitset<2> FightPetData::Handler::template_assigned_bitset { "11" };

const char* FightBuffData::Handler::m_key_list[2] = {
    u8"id",
    u8"stack"
};
const char* FightBuffData::Handler::m_alias_list[2] = {
    u8"id",
    u8"stack"
};
const std::bitset<2> FightBuffData::Handler::template_assigned_bitset { "11" };

const char* FighterData::Handler::m_key_list[13] = {
    u8"site",
    u8"fighter_id",
    u8"guid",
    u8"type",
    u8"template_id",
    u8"name",
    u8"attrs",
    u8"pets",
    u8"buffs",
    u8"model",
    u8"friend_instruction",
    u8"enemy_instruction",
    u8"customs"
};
const char* FighterData::Handler::m_alias_list[13] = {
    u8"site",
    u8"fighter_id",
    u8"guid",
    u8"type",
    u8"template_id",
    u8"name",
    u8"attrs",
    u8"pets",
    u8"buffs",
    u8"model",
    u8"friend_instruction",
    u8"enemy_instruction",
    u8"customs"
};
const std::bitset<13> FighterData::Handler::template_assigned_bitset { "1111111111111" };

const char* FightGroupData::Handler::m_key_list[4] = {
    u8"type",
    u8"lineup_id",
    u8"lineup_lv",
    u8"fighters"
};
const char* FightGroupData::Handler::m_alias_list[4] = {
    u8"type",
    u8"lineup_id",
    u8"lineup_lv",
    u8"fighters"
};
const std::bitset<4> FightGroupData::Handler::template_assigned_bitset { "1111" };

const char* FightOperateData::Handler::m_key_list[4] = {
    u8"actor",
    u8"operate",
    u8"target",
    u8"param"
};
const char* FightOperateData::Handler::m_alias_list[4] = {
    u8"actor",
    u8"operate",
    u8"target",
    u8"param"
};
const std::bitset<4> FightOperateData::Handler::template_assigned_bitset { "1111" };

const char* MemberData::Handler::m_key_list[5] = {
    u8"guid",
    u8"name",
    u8"temp_leave",
    u8"attrs",
    u8"customs"
};
const char* MemberData::Handler::m_alias_list[5] = {
    u8"guid",
    u8"name",
    u8"temp_leave",
    u8"attrs",
    u8"customs"
};
const std::bitset<5> MemberData::Handler::template_assigned_bitset { "11111" };

const char* KeyValueU32::Handler::m_key_list[2] = {
    u8"key",
    u8"value"
};
const char* KeyValueU32::Handler::m_alias_list[2] = {
    u8"key",
    u8"value"
};
const std::bitset<2> KeyValueU32::Handler::template_assigned_bitset { "11" };

const char* KeyValueU64::Handler::m_key_list[2] = {
    u8"key",
    u8"value"
};
const char* KeyValueU64::Handler::m_alias_list[2] = {
    u8"key",
    u8"value"
};
const std::bitset<2> KeyValueU64::Handler::template_assigned_bitset { "11" };

const char* KeyValueS::Handler::m_key_list[2] = {
    u8"key",
    u8"value"
};
const char* KeyValueS::Handler::m_alias_list[2] = {
    u8"key",
    u8"value"
};
const std::bitset<2> KeyValueS::Handler::template_assigned_bitset { "11" };

const char* OptionData::Handler::m_key_list[3] = {
    u8"id",
    u8"mark",
    u8"text"
};
const char* OptionData::Handler::m_alias_list[3] = {
    u8"id",
    u8"mark",
    u8"text"
};
const std::bitset<3> OptionData::Handler::template_assigned_bitset { "111" };

const char* ShopItemData::Handler::m_key_list[4] = {
    u8"id",
    u8"price_type",
    u8"price",
    u8"bind"
};
const char* ShopItemData::Handler::m_alias_list[4] = {
    u8"id",
    u8"price_type",
    u8"price",
    u8"bind"
};
const std::bitset<4> ShopItemData::Handler::template_assigned_bitset { "1111" };

const char* SellItemData::Handler::m_key_list[2] = {
    u8"guid",
    u8"amount"
};
const char* SellItemData::Handler::m_alias_list[2] = {
    u8"guid",
    u8"amount"
};
const std::bitset<2> SellItemData::Handler::template_assigned_bitset { "11" };

const char* Col1Data::Handler::m_key_list[1] = {
    u8"col1"
};
const char* Col1Data::Handler::m_alias_list[1] = {
    u8"col1"
};
const std::bitset<1> Col1Data::Handler::template_assigned_bitset { "1" };

const char* Col2Data::Handler::m_key_list[2] = {
    u8"col1",
    u8"col2"
};
const char* Col2Data::Handler::m_alias_list[2] = {
    u8"col1",
    u8"col2"
};
const std::bitset<2> Col2Data::Handler::template_assigned_bitset { "11" };

const char* Col3Data::Handler::m_key_list[3] = {
    u8"col1",
    u8"col2",
    u8"col3"
};
const char* Col3Data::Handler::m_alias_list[3] = {
    u8"col1",
    u8"col2",
    u8"col3"
};
const std::bitset<3> Col3Data::Handler::template_assigned_bitset { "111" };

const char* Col4Data::Handler::m_key_list[4] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4"
};
const char* Col4Data::Handler::m_alias_list[4] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4"
};
const std::bitset<4> Col4Data::Handler::template_assigned_bitset { "1111" };

const char* Col5Data::Handler::m_key_list[5] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5"
};
const char* Col5Data::Handler::m_alias_list[5] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5"
};
const std::bitset<5> Col5Data::Handler::template_assigned_bitset { "11111" };

const char* Col6Data::Handler::m_key_list[6] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6"
};
const char* Col6Data::Handler::m_alias_list[6] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6"
};
const std::bitset<6> Col6Data::Handler::template_assigned_bitset { "111111" };

const char* Col7Data::Handler::m_key_list[7] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6",
    u8"col7"
};
const char* Col7Data::Handler::m_alias_list[7] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6",
    u8"col7"
};
const std::bitset<7> Col7Data::Handler::template_assigned_bitset { "1111111" };

const char* Col8Data::Handler::m_key_list[8] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6",
    u8"col7",
    u8"col8"
};
const char* Col8Data::Handler::m_alias_list[8] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6",
    u8"col7",
    u8"col8"
};
const std::bitset<8> Col8Data::Handler::template_assigned_bitset { "11111111" };

const char* Col9Data::Handler::m_key_list[9] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6",
    u8"col7",
    u8"col8",
    u8"col9"
};
const char* Col9Data::Handler::m_alias_list[9] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6",
    u8"col7",
    u8"col8",
    u8"col9"
};
const std::bitset<9> Col9Data::Handler::template_assigned_bitset { "111111111" };

const char* Col10Data::Handler::m_key_list[10] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6",
    u8"col7",
    u8"col8",
    u8"col9",
    u8"col10"
};
const char* Col10Data::Handler::m_alias_list[10] = {
    u8"col1",
    u8"col2",
    u8"col3",
    u8"col4",
    u8"col5",
    u8"col6",
    u8"col7",
    u8"col8",
    u8"col9",
    u8"col10"
};
const std::bitset<10> Col10Data::Handler::template_assigned_bitset { "1111111111" };

const char* TalkNodeData::Handler::m_key_list[12] = {
    u8"node_type",
    u8"node_id",
    u8"display_text",
    u8"connect_text",
    u8"npc_id",
    u8"npc_emotion",
    u8"npc_talk",
    u8"npc_template",
    u8"func_type",
    u8"link_node",
    u8"belong_to",
    u8"player_talk"
};
const char* TalkNodeData::Handler::m_alias_list[12] = {
    u8"node_type",
    u8"node_id",
    u8"display_text",
    u8"connect_text",
    u8"npc_id",
    u8"npc_emotion",
    u8"npc_talk",
    u8"npc_template",
    u8"func_type",
    u8"link_node",
    u8"belong_to",
    u8"player_talk"
};
const std::bitset<12> TalkNodeData::Handler::template_assigned_bitset { "111111111111" };

const char* QuestClientData::Handler::m_key_list[29] = {
    u8"quest_id",
    u8"quest_type",
    u8"quest_type_name",
    u8"quest_name",
    u8"quest_color",
    u8"quest_state",
    u8"display_location",
    u8"display_accept_tip",
    u8"display_accomplish_tip",
    u8"can_track",
    u8"can_abandon",
    u8"accpet_time",
    u8"limit_time",
    u8"changed_time",
    u8"accept_npc",
    u8"accept_npc_info",
    u8"commit_npc",
    u8"commit_npc_info",
    u8"quest_goal_type",
    u8"quest_goal",
    u8"quest_award",
    u8"track_desc",
    u8"cant_accept_track_desc",
    u8"cant_accept_desc",
    u8"can_accept_desc",
    u8"accepted_desc",
    u8"is_display_cycle_num",
    u8"finish_cycle_num",
    u8"total_cycle_num"
};
const char* QuestClientData::Handler::m_alias_list[29] = {
    u8"quest_id",
    u8"quest_type",
    u8"quest_type_name",
    u8"quest_name",
    u8"quest_color",
    u8"quest_state",
    u8"display_location",
    u8"display_accept_tip",
    u8"display_accomplish_tip",
    u8"can_track",
    u8"can_abandon",
    u8"accpet_time",
    u8"limit_time",
    u8"changed_time",
    u8"accept_npc",
    u8"accept_npc_info",
    u8"commit_npc",
    u8"commit_npc_info",
    u8"quest_goal_type",
    u8"quest_goal",
    u8"quest_award",
    u8"track_desc",
    u8"cant_accept_track_desc",
    u8"cant_accept_desc",
    u8"can_accept_desc",
    u8"accepted_desc",
    u8"is_display_cycle_num",
    u8"finish_cycle_num",
    u8"total_cycle_num"
};
const std::bitset<29> QuestClientData::Handler::template_assigned_bitset { "11111111111111111111111111111" };

const char* QuestTalkData::Handler::m_key_list[4] = {
    u8"npc_id",
    u8"node_id",
    u8"talk_text",
    u8"options"
};
const char* QuestTalkData::Handler::m_alias_list[4] = {
    u8"npc_id",
    u8"node_id",
    u8"talk_text",
    u8"options"
};
const std::bitset<4> QuestTalkData::Handler::template_assigned_bitset { "1111" };

const char* IDCountData::Handler::m_key_list[2] = {
    u8"id",
    u8"amount"
};
const char* IDCountData::Handler::m_alias_list[2] = {
    u8"id",
    u8"amount"
};
const std::bitset<2> IDCountData::Handler::template_assigned_bitset { "11" };

const char* GUIDCountData::Handler::m_key_list[2] = {
    u8"guid",
    u8"amount"
};
const char* GUIDCountData::Handler::m_alias_list[2] = {
    u8"guid",
    u8"amount"
};
const std::bitset<2> GUIDCountData::Handler::template_assigned_bitset { "11" };

const char* SkillTipsData::Handler::m_key_list[3] = {
    u8"name",
    u8"attr",
    u8"value"
};
const char* SkillTipsData::Handler::m_alias_list[3] = {
    u8"name",
    u8"attr",
    u8"value"
};
const std::bitset<3> SkillTipsData::Handler::template_assigned_bitset { "111" };

const char* GoalVisitNpcData::Handler::m_key_list[1] = {
    u8"npc_id"
};
const char* GoalVisitNpcData::Handler::m_alias_list[1] = {
    u8"npc_id"
};
const std::bitset<1> GoalVisitNpcData::Handler::template_assigned_bitset { "1" };

const char* GoalKillBossData::Handler::m_key_list[5] = {
    u8"boss_id",
    u8"map_id",
    u8"loc_x",
    u8"loc_y",
    u8"kill_count"
};
const char* GoalKillBossData::Handler::m_alias_list[5] = {
    u8"boss_id",
    u8"map_id",
    u8"loc_x",
    u8"loc_y",
    u8"kill_count"
};
const std::bitset<5> GoalKillBossData::Handler::template_assigned_bitset { "11111" };

const char* GoalCommitItemData::Handler::m_key_list[2] = {
    u8"item_id",
    u8"item_count"
};
const char* GoalCommitItemData::Handler::m_alias_list[2] = {
    u8"item_id",
    u8"item_count"
};
const std::bitset<2> GoalCommitItemData::Handler::template_assigned_bitset { "11" };

const char* GoalCommitPetData::Handler::m_key_list[2] = {
    u8"pet_id",
    u8"pet_count"
};
const char* GoalCommitPetData::Handler::m_alias_list[2] = {
    u8"pet_id",
    u8"pet_count"
};
const std::bitset<2> GoalCommitPetData::Handler::template_assigned_bitset { "11" };

const char* GoalEscortNpcData::Handler::m_key_list[1] = {
    u8"npc_id"
};
const char* GoalEscortNpcData::Handler::m_alias_list[1] = {
    u8"npc_id"
};
const std::bitset<1> GoalEscortNpcData::Handler::template_assigned_bitset { "1" };

const char* GoalArriveAddrData::Handler::m_key_list[7] = {
    u8"map_id",
    u8"loc_x",
    u8"loc_y",
    u8"range",
    u8"item_id",
    u8"npc_id",
    u8"desc"
};
const char* GoalArriveAddrData::Handler::m_alias_list[7] = {
    u8"map_id",
    u8"loc_x",
    u8"loc_y",
    u8"range",
    u8"item_id",
    u8"npc_id",
    u8"desc"
};
const std::bitset<7> GoalArriveAddrData::Handler::template_assigned_bitset { "1111111" };

const char* GoalFightMapData::Handler::m_key_list[3] = {
    u8"map_id",
    u8"count",
    u8"count_required"
};
const char* GoalFightMapData::Handler::m_alias_list[3] = {
    u8"map_id",
    u8"count",
    u8"count_required"
};
const std::bitset<3> GoalFightMapData::Handler::template_assigned_bitset { "111" };

const char* GoalFightMapCountData::Handler::m_key_list[1] = {
    u8"fight_map"
};
const char* GoalFightMapCountData::Handler::m_alias_list[1] = {
    u8"fight_map"
};
const std::bitset<1> GoalFightMapCountData::Handler::template_assigned_bitset { "1" };

const char* GoalKillMonsterData::Handler::m_key_list[8] = {
    u8"monster_id",
    u8"count",
    u8"count_required",
    u8"map_id",
    u8"left",
    u8"top",
    u8"width",
    u8"height"
};
const char* GoalKillMonsterData::Handler::m_alias_list[8] = {
    u8"monster_id",
    u8"count",
    u8"count_required",
    u8"map_id",
    u8"left",
    u8"top",
    u8"width",
    u8"height"
};
const std::bitset<8> GoalKillMonsterData::Handler::template_assigned_bitset { "11111111" };

const char* GoalKillMonsterCountData::Handler::m_key_list[1] = {
    u8"kill_monster"
};
const char* GoalKillMonsterCountData::Handler::m_alias_list[1] = {
    u8"kill_monster"
};
const std::bitset<1> GoalKillMonsterCountData::Handler::template_assigned_bitset { "1" };

const char* GoalEncounterFightData::Handler::m_key_list[2] = {
    u8"cur_num",
    u8"max_num"
};
const char* GoalEncounterFightData::Handler::m_alias_list[2] = {
    u8"cur_num",
    u8"max_num"
};
const std::bitset<2> GoalEncounterFightData::Handler::template_assigned_bitset { "11" };

const char* GoalUseItemData::Handler::m_key_list[7] = {
    u8"map_id",
    u8"loc_x",
    u8"loc_y",
    u8"range",
    u8"item_id",
    u8"npc_id",
    u8"desc"
};
const char* GoalUseItemData::Handler::m_alias_list[7] = {
    u8"map_id",
    u8"loc_x",
    u8"loc_y",
    u8"range",
    u8"item_id",
    u8"npc_id",
    u8"desc"
};
const std::bitset<7> GoalUseItemData::Handler::template_assigned_bitset { "1111111" };

const char* GoalCapturePetData::Handler::m_key_list[3] = {
    u8"pet_id",
    u8"count",
    u8"count_required"
};
const char* GoalCapturePetData::Handler::m_alias_list[3] = {
    u8"pet_id",
    u8"count",
    u8"count_required"
};
const std::bitset<3> GoalCapturePetData::Handler::template_assigned_bitset { "111" };

const char* GoalPVEData::Handler::m_key_list[2] = {
    u8"win_cur",
    u8"win_required"
};
const char* GoalPVEData::Handler::m_alias_list[2] = {
    u8"win_cur",
    u8"win_required"
};
const std::bitset<2> GoalPVEData::Handler::template_assigned_bitset { "11" };

const char* IPConfig::Handler::m_key_list[5] = {
    u8"type",
    u8"port",
    u8"recv_buf",
    u8"send_buf",
    u8"ip"
};
const char* IPConfig::Handler::m_alias_list[5] = {
    u8"type",
    u8"port",
    u8"recv_buf",
    u8"send_buf",
    u8"ip"
};
const std::bitset<5> IPConfig::Handler::template_assigned_bitset { "11111" };

const char* ActivityData::Handler::m_key_list[4] = {
    u8"id",
    u8"state",
    u8"today",
    u8"custom"
};
const char* ActivityData::Handler::m_alias_list[4] = {
    u8"id",
    u8"state",
    u8"today",
    u8"custom"
};
const std::bitset<4> ActivityData::Handler::template_assigned_bitset { "1111" };

const char* StringData::Handler::m_key_list[1] = {
    u8"str"
};
const char* StringData::Handler::m_alias_list[1] = {
    u8"str"
};
const std::bitset<1> StringData::Handler::template_assigned_bitset { "1" };

const char* GUIDData::Handler::m_key_list[1] = {
    u8"guid"
};
const char* GUIDData::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> GUIDData::Handler::template_assigned_bitset { "1" };

const char* AuctionObjectData::Handler::m_key_list[18] = {
    u8"guid",
    u8"id",
    u8"type",
    u8"custom1",
    u8"custom2",
    u8"custom3",
    u8"level",
    u8"fight_value",
    u8"coin_type",
    u8"coin_value",
    u8"amount",
    u8"state",
    u8"puton_times",
    u8"pulloff_times",
    u8"seller_guid",
    u8"seller_name",
    u8"item_data",
    u8"pet_data"
};
const char* AuctionObjectData::Handler::m_alias_list[18] = {
    u8"guid",
    u8"id",
    u8"type",
    u8"custom1",
    u8"custom2",
    u8"custom3",
    u8"level",
    u8"fight_value",
    u8"coin_type",
    u8"coin_value",
    u8"amount",
    u8"state",
    u8"puton_times",
    u8"pulloff_times",
    u8"seller_guid",
    u8"seller_name",
    u8"item_data",
    u8"pet_data"
};
const std::bitset<18> AuctionObjectData::Handler::template_assigned_bitset { "111111111111111111" };

const char* AuctionCookieData::Handler::m_key_list[4] = {
    u8"guid",
    u8"coins",
    u8"favorites",
    u8"records"
};
const char* AuctionCookieData::Handler::m_alias_list[4] = {
    u8"guid",
    u8"coins",
    u8"favorites",
    u8"records"
};
const std::bitset<4> AuctionCookieData::Handler::template_assigned_bitset { "1111" };

const char* GuildMemberBriefData::Handler::m_key_list[14] = {
    u8"player_guid",
    u8"player_name",
    u8"level",
    u8"role",
    u8"guild_job",
    u8"total_contrb",
    u8"join_time",
    u8"last_login_time",
    u8"last_logout_time",
    u8"status",
    u8"vip",
    u8"reincarnation",
    u8"job",
    u8"customs"
};
const char* GuildMemberBriefData::Handler::m_alias_list[14] = {
    u8"player_guid",
    u8"player_name",
    u8"level",
    u8"role",
    u8"guild_job",
    u8"total_contrb",
    u8"join_time",
    u8"last_login_time",
    u8"last_logout_time",
    u8"status",
    u8"vip",
    u8"reincarnation",
    u8"job",
    u8"customs"
};
const std::bitset<14> GuildMemberBriefData::Handler::template_assigned_bitset { "11111111111111" };

const char* GuildMemberData::Handler::m_key_list[22] = {
    u8"player_guid",
    u8"player_name",
    u8"level",
    u8"role",
    u8"guild_job",
    u8"total_contrb",
    u8"surplus_contrb",
    u8"last_week_contrb",
    u8"this_week_contrb",
    u8"permission",
    u8"join_time",
    u8"donate_bind_gold",
    u8"last_donate_time",
    u8"last_dividend_time",
    u8"last_login_time",
    u8"last_logout_time",
    u8"forbid_end_time",
    u8"status",
    u8"vip",
    u8"reincarnation",
    u8"customs",
    u8"job"
};
const char* GuildMemberData::Handler::m_alias_list[22] = {
    u8"player_guid",
    u8"player_name",
    u8"level",
    u8"role",
    u8"guild_job",
    u8"total_contrb",
    u8"surplus_contrb",
    u8"last_week_contrb",
    u8"this_week_contrb",
    u8"permission",
    u8"join_time",
    u8"donate_bind_gold",
    u8"last_donate_time",
    u8"last_dividend_time",
    u8"last_login_time",
    u8"last_logout_time",
    u8"forbid_end_time",
    u8"status",
    u8"vip",
    u8"reincarnation",
    u8"customs",
    u8"job"
};
const std::bitset<22> GuildMemberData::Handler::template_assigned_bitset { "1111111111111111111111" };

const char* GuildApplicantData::Handler::m_key_list[10] = {
    u8"guid",
    u8"name",
    u8"level",
    u8"role",
    u8"sn",
    u8"guild_contrb",
    u8"last_apply_time",
    u8"vip",
    u8"reincarnation",
    u8"job"
};
const char* GuildApplicantData::Handler::m_alias_list[10] = {
    u8"guid",
    u8"name",
    u8"level",
    u8"role",
    u8"sn",
    u8"guild_contrb",
    u8"last_apply_time",
    u8"vip",
    u8"reincarnation",
    u8"job"
};
const std::bitset<10> GuildApplicantData::Handler::template_assigned_bitset { "1111111111" };

const char* GuildEventData::Handler::m_key_list[3] = {
    u8"event_type",
    u8"event_time",
    u8"event_msg"
};
const char* GuildEventData::Handler::m_alias_list[3] = {
    u8"event_type",
    u8"event_time",
    u8"event_msg"
};
const std::bitset<3> GuildEventData::Handler::template_assigned_bitset { "111" };

const char* GuildBriefData::Handler::m_key_list[13] = {
    u8"guid",
    u8"guild_id",
    u8"name",
    u8"leader_guid",
    u8"leader_name",
    u8"leader_level",
    u8"leader_role",
    u8"level",
    u8"member_count",
    u8"max_member_count",
    u8"declaration",
    u8"applyed",
    u8"leader_school"
};
const char* GuildBriefData::Handler::m_alias_list[13] = {
    u8"guid",
    u8"guild_id",
    u8"name",
    u8"leader_guid",
    u8"leader_name",
    u8"leader_level",
    u8"leader_role",
    u8"level",
    u8"member_count",
    u8"max_member_count",
    u8"declaration",
    u8"applyed",
    u8"leader_school"
};
const std::bitset<13> GuildBriefData::Handler::template_assigned_bitset { "1111111111111" };

const char* GuildData::Handler::m_key_list[28] = {
    u8"guid",
    u8"guild_id",
    u8"name",
    u8"leader_guid",
    u8"leader_name",
    u8"level",
    u8"base_level",
    u8"wing_room_level",
    u8"vault_level",
    u8"academy_level",
    u8"pharmacy_level",
    u8"upgrade_cooldown",
    u8"member_count",
    u8"max_member_count",
    u8"online_count",
    u8"fund",
    u8"liveness",
    u8"build_degree",
    u8"prestige",
    u8"fight_score",
    u8"config",
    u8"create_time",
    u8"dismissed_time",
    u8"impeach_time",
    u8"impeach_guid",
    u8"declaration",
    u8"events",
    u8"customs"
};
const char* GuildData::Handler::m_alias_list[28] = {
    u8"guid",
    u8"guild_id",
    u8"name",
    u8"leader_guid",
    u8"leader_name",
    u8"level",
    u8"base_level",
    u8"wing_room_level",
    u8"vault_level",
    u8"academy_level",
    u8"pharmacy_level",
    u8"upgrade_cooldown",
    u8"member_count",
    u8"max_member_count",
    u8"online_count",
    u8"fund",
    u8"liveness",
    u8"build_degree",
    u8"prestige",
    u8"fight_score",
    u8"config",
    u8"create_time",
    u8"dismissed_time",
    u8"impeach_time",
    u8"impeach_guid",
    u8"declaration",
    u8"events",
    u8"customs"
};
const std::bitset<28> GuildData::Handler::template_assigned_bitset { "1111111111111111111111111111" };

const char* NoticeData::Handler::m_key_list[2] = {
    u8"notice_time",
    u8"notice"
};
const char* NoticeData::Handler::m_alias_list[2] = {
    u8"notice_time",
    u8"notice"
};
const std::bitset<2> NoticeData::Handler::template_assigned_bitset { "11" };

const char* DBGuildData::Handler::m_key_list[3] = {
    u8"guild",
    u8"notice",
    u8"items"
};
const char* DBGuildData::Handler::m_alias_list[3] = {
    u8"guild",
    u8"notice",
    u8"items"
};
const std::bitset<3> DBGuildData::Handler::template_assigned_bitset { "111" };

const char* DBGuildMemberData::Handler::m_key_list[3] = {
    u8"member",
    u8"guild_guid",
    u8"total_fight_value"
};
const char* DBGuildMemberData::Handler::m_alias_list[3] = {
    u8"member",
    u8"guild_guid",
    u8"total_fight_value"
};
const std::bitset<3> DBGuildMemberData::Handler::template_assigned_bitset { "111" };

const char* DBGuildApplicantData::Handler::m_key_list[3] = {
    u8"applicant",
    u8"guild_guid",
    u8"total_fight_value"
};
const char* DBGuildApplicantData::Handler::m_alias_list[3] = {
    u8"applicant",
    u8"guild_guid",
    u8"total_fight_value"
};
const std::bitset<3> DBGuildApplicantData::Handler::template_assigned_bitset { "111" };

const char* ContactData::Handler::m_key_list[13] = {
    u8"guid",
    u8"contact_type",
    u8"name",
    u8"role",
    u8"job",
    u8"level",
    u8"friendship",
    u8"last_contact_time",
    u8"sn",
    u8"status",
    u8"vip",
    u8"reincarnation",
    u8"customs"
};
const char* ContactData::Handler::m_alias_list[13] = {
    u8"guid",
    u8"contact_type",
    u8"name",
    u8"role",
    u8"job",
    u8"level",
    u8"friendship",
    u8"last_contact_time",
    u8"sn",
    u8"status",
    u8"vip",
    u8"reincarnation",
    u8"customs"
};
const std::bitset<13> ContactData::Handler::template_assigned_bitset { "1111111111111" };

const char* DBContactData::Handler::m_key_list[6] = {
    u8"player_guid",
    u8"contact_guid",
    u8"contact_type",
    u8"friendship",
    u8"last_contact_time",
    u8"customs"
};
const char* DBContactData::Handler::m_alias_list[6] = {
    u8"player_guid",
    u8"contact_guid",
    u8"contact_type",
    u8"friendship",
    u8"last_contact_time",
    u8"customs"
};
const std::bitset<6> DBContactData::Handler::template_assigned_bitset { "111111" };

const char* MessageData::Handler::m_key_list[5] = {
    u8"message",
    u8"send_guid",
    u8"send_time",
    u8"recv_guid",
    u8"auto_reply"
};
const char* MessageData::Handler::m_alias_list[5] = {
    u8"message",
    u8"send_guid",
    u8"send_time",
    u8"recv_guid",
    u8"auto_reply"
};
const std::bitset<5> MessageData::Handler::template_assigned_bitset { "11111" };

const char* ContactInfoData::Handler::m_key_list[12] = {
    u8"guid",
    u8"name",
    u8"role",
    u8"job",
    u8"level",
    u8"sn",
    u8"status",
    u8"vip",
    u8"reincarnation",
    u8"last_login_time",
    u8"last_logout_time",
    u8"total_fight_value"
};
const char* ContactInfoData::Handler::m_alias_list[12] = {
    u8"guid",
    u8"name",
    u8"role",
    u8"job",
    u8"level",
    u8"sn",
    u8"status",
    u8"vip",
    u8"reincarnation",
    u8"last_login_time",
    u8"last_logout_time",
    u8"total_fight_value"
};
const std::bitset<12> ContactInfoData::Handler::template_assigned_bitset { "111111111111" };

const char* MailHeadData::Handler::m_key_list[7] = {
    u8"mail_guid",
    u8"mail_type",
    u8"title",
    u8"send_guid",
    u8"send_name",
    u8"send_time",
    u8"status"
};
const char* MailHeadData::Handler::m_alias_list[7] = {
    u8"mail_guid",
    u8"mail_type",
    u8"title",
    u8"send_guid",
    u8"send_name",
    u8"send_time",
    u8"status"
};
const std::bitset<7> MailHeadData::Handler::template_assigned_bitset { "1111111" };

const char* MailBodyData::Handler::m_key_list[4] = {
    u8"content",
    u8"attrs",
    u8"items",
    u8"pets"
};
const char* MailBodyData::Handler::m_alias_list[4] = {
    u8"content",
    u8"attrs",
    u8"items",
    u8"pets"
};
const std::bitset<4> MailBodyData::Handler::template_assigned_bitset { "1111" };

const char* MailData::Handler::m_key_list[2] = {
    u8"head",
    u8"body"
};
const char* MailData::Handler::m_alias_list[2] = {
    u8"head",
    u8"body"
};
const std::bitset<2> MailData::Handler::template_assigned_bitset { "11" };

const char* DBMailData::Handler::m_key_list[4] = {
    u8"recv_guid",
    u8"guids",
    u8"head",
    u8"body"
};
const char* DBMailData::Handler::m_alias_list[4] = {
    u8"recv_guid",
    u8"guids",
    u8"head",
    u8"body"
};
const std::bitset<4> DBMailData::Handler::template_assigned_bitset { "1111" };

const char* DBVarData::Handler::m_key_list[4] = {
    u8"key",
    u8"value",
    u8"value_type",
    u8"merge_type"
};
const char* DBVarData::Handler::m_alias_list[4] = {
    u8"key",
    u8"value",
    u8"value_type",
    u8"merge_type"
};
const std::bitset<4> DBVarData::Handler::template_assigned_bitset { "1111" };

const char* RanklistData::Handler::m_key_list[5] = {
    u8"name",
    u8"rank_info",
    u8"rank_data1",
    u8"rank_data2",
    u8"rank_data3"
};
const char* RanklistData::Handler::m_alias_list[5] = {
    u8"name",
    u8"rank_info",
    u8"rank_data1",
    u8"rank_data2",
    u8"rank_data3"
};
const std::bitset<5> RanklistData::Handler::template_assigned_bitset { "11111" };

const char* DBRanklistData::Handler::m_key_list[8] = {
    u8"name",
    u8"rank_info",
    u8"rank_data1",
    u8"rank_data2",
    u8"rank_data3",
    u8"rank_guid",
    u8"object_guid",
    u8"rank_type"
};
const char* DBRanklistData::Handler::m_alias_list[8] = {
    u8"name",
    u8"rank_info",
    u8"rank_data1",
    u8"rank_data2",
    u8"rank_data3",
    u8"rank_guid",
    u8"object_guid",
    u8"rank_type"
};
const std::bitset<8> DBRanklistData::Handler::template_assigned_bitset { "11111111" };

const char* TeamBriefData::Handler::m_key_list[10] = {
    u8"team_guid",
    u8"target",
    u8"min_require_level",
    u8"max_require_level",
    u8"member_count",
    u8"leader_data",
    u8"has_applicant",
    u8"min_require_reinc",
    u8"max_require_reinc",
    u8"customs"
};
const char* TeamBriefData::Handler::m_alias_list[10] = {
    u8"team_guid",
    u8"target",
    u8"min_require_level",
    u8"max_require_level",
    u8"member_count",
    u8"leader_data",
    u8"has_applicant",
    u8"min_require_reinc",
    u8"max_require_reinc",
    u8"customs"
};
const std::bitset<10> TeamBriefData::Handler::template_assigned_bitset { "1111111111" };

const char* TeamApplicantData::Handler::m_key_list[3] = {
    u8"guid",
    u8"name",
    u8"attrs"
};
const char* TeamApplicantData::Handler::m_alias_list[3] = {
    u8"guid",
    u8"name",
    u8"attrs"
};
const std::bitset<3> TeamApplicantData::Handler::template_assigned_bitset { "111" };

const char* ExchangeData::Handler::m_key_list[2] = {
    u8"day",
    u8"records"
};
const char* ExchangeData::Handler::m_alias_list[2] = {
    u8"day",
    u8"records"
};
const std::bitset<2> ExchangeData::Handler::template_assigned_bitset { "11" };

const char* ForbidData::Handler::m_key_list[7] = {
    u8"sid",
    u8"keyword",
    u8"keytype",
    u8"fbdtype",
    u8"begin",
    u8"end",
    u8"desc"
};
const char* ForbidData::Handler::m_alias_list[7] = {
    u8"sid",
    u8"keyword",
    u8"keytype",
    u8"fbdtype",
    u8"begin",
    u8"end",
    u8"desc"
};
const std::bitset<7> ForbidData::Handler::template_assigned_bitset { "1111111" };

const char* ForbidTalkData::Handler::m_key_list[4] = {
    u8"guid",
    u8"begin",
    u8"end",
    u8"desc"
};
const char* ForbidTalkData::Handler::m_alias_list[4] = {
    u8"guid",
    u8"begin",
    u8"end",
    u8"desc"
};
const std::bitset<4> ForbidTalkData::Handler::template_assigned_bitset { "1111" };

const char* PasturePetData::Handler::m_key_list[12] = {
    u8"owner_guid",
    u8"pet",
    u8"map",
    u8"x",
    u8"y",
    u8"placed_time",
    u8"placed_status",
    u8"replaced_time",
    u8"replaced_player_guid",
    u8"replaced_player_name",
    u8"replaced_pet_guid",
    u8"replaced_pet_name"
};
const char* PasturePetData::Handler::m_alias_list[12] = {
    u8"owner_guid",
    u8"pet",
    u8"map",
    u8"x",
    u8"y",
    u8"placed_time",
    u8"placed_status",
    u8"replaced_time",
    u8"replaced_player_guid",
    u8"replaced_player_name",
    u8"replaced_pet_guid",
    u8"replaced_pet_name"
};
const std::bitset<12> PasturePetData::Handler::template_assigned_bitset { "111111111111" };

const char* LadderData::Handler::m_key_list[3] = {
    u8"player_guid",
    u8"rank",
    u8"player"
};
const char* LadderData::Handler::m_alias_list[3] = {
    u8"player_guid",
    u8"rank",
    u8"player"
};
const std::bitset<3> LadderData::Handler::template_assigned_bitset { "111" };

const char* LadderRecordData::Handler::m_key_list[8] = {
    u8"action",
    u8"time",
    u8"player_guid",
    u8"player_name",
    u8"player_rank",
    u8"target_guid",
    u8"target_name",
    u8"target_rank"
};
const char* LadderRecordData::Handler::m_alias_list[8] = {
    u8"action",
    u8"time",
    u8"player_guid",
    u8"player_name",
    u8"player_rank",
    u8"target_guid",
    u8"target_name",
    u8"target_rank"
};
const std::bitset<8> LadderRecordData::Handler::template_assigned_bitset { "11111111" };

const char* PlayerDetail::Handler::m_key_list[15] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"unid",
    u8"name",
    u8"attrs",
    u8"map",
    u8"x",
    u8"y",
    u8"state",
    u8"createtime",
    u8"lastlogin",
    u8"destroytime",
    u8"customs",
    u8"equips"
};
const char* PlayerDetail::Handler::m_alias_list[15] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"unid",
    u8"name",
    u8"attrs",
    u8"map",
    u8"x",
    u8"y",
    u8"state",
    u8"createtime",
    u8"lastlogin",
    u8"destroytime",
    u8"customs",
    u8"equips"
};
const std::bitset<15> PlayerDetail::Handler::template_assigned_bitset { "111111111111111" };

const char* Pt::Handler::m_key_list[2] = {
    u8"x",
    u8"y"
};
const char* Pt::Handler::m_alias_list[2] = {
    u8"x",
    u8"y"
};
const std::bitset<2> Pt::Handler::template_assigned_bitset { "11" };

const char* ViolateData::Handler::m_key_list[2] = {
    u8"type",
    u8"violate"
};
const char* ViolateData::Handler::m_alias_list[2] = {
    u8"type",
    u8"violate"
};
const std::bitset<2> ViolateData::Handler::template_assigned_bitset { "11" };

const char* ChannelSwitchData::Handler::m_key_list[2] = {
    u8"type",
    u8"sw"
};
const char* ChannelSwitchData::Handler::m_alias_list[2] = {
    u8"type",
    u8"sw"
};
const std::bitset<2> ChannelSwitchData::Handler::template_assigned_bitset { "11" };

const char* VerifyData::Handler::m_key_list[2] = {
    u8"fbd_time",
    u8"accountstatus"
};
const char* VerifyData::Handler::m_alias_list[2] = {
    u8"fbd_time",
    u8"accountstatus"
};
const std::bitset<2> VerifyData::Handler::template_assigned_bitset { "00" };

const char* IPPort::Handler::m_key_list[2] = {
    u8"ip",
    u8"port"
};
const char* IPPort::Handler::m_alias_list[2] = {
    u8"ip",
    u8"port"
};
const std::bitset<2> IPPort::Handler::template_assigned_bitset { "11" };

const char* CrossIPPort::Handler::m_key_list[7] = {
    u8"gid",
    u8"gcp",
    u8"gsp",
    u8"innerip",
    u8"extip",
    u8"innerflag",
    u8"clientname"
};
const char* CrossIPPort::Handler::m_alias_list[7] = {
    u8"gid",
    u8"gcp",
    u8"gsp",
    u8"innerip",
    u8"extip",
    u8"innerflag",
    u8"clientname"
};
const std::bitset<7> CrossIPPort::Handler::template_assigned_bitset { "1111111" };

const char* TestJsonName::Handler::m_key_list[1] = {
    u8"name"
};
const char* TestJsonName::Handler::m_alias_list[1] = {
    u8"name"
};
const std::bitset<1> TestJsonName::Handler::template_assigned_bitset { "1" };

