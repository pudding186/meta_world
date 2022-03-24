#include <new>

#include "GSCS.hpp"

template<>
const unsigned short Protocol<LoginCSNtf>::module_id = 108;
template<>
const unsigned short Protocol<LoginCSNtf>::protocol_id = 1;

const char* LoginCSNtf::Handler::m_key_list[7] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"attrs",
    u8"name",
    u8"gate_index",
    u8"game_index"
};
const char* LoginCSNtf::Handler::m_alias_list[7] = {
    u8"uid",
    u8"sid",
    u8"guid",
    u8"attrs",
    u8"name",
    u8"gate_index",
    u8"game_index"
};
const std::bitset<7> LoginCSNtf::Handler::template_assigned_bitset { "1111111" };

template<>
const unsigned short Protocol<SyncPlayerU32Ntf>::module_id = 108;
template<>
const unsigned short Protocol<SyncPlayerU32Ntf>::protocol_id = 2;

const char* SyncPlayerU32Ntf::Handler::m_key_list[2] = {
    u8"guid",
    u8"pair"
};
const char* SyncPlayerU32Ntf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"pair"
};
const std::bitset<2> SyncPlayerU32Ntf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<SyncPlayerU64Ntf>::module_id = 108;
template<>
const unsigned short Protocol<SyncPlayerU64Ntf>::protocol_id = 3;

const char* SyncPlayerU64Ntf::Handler::m_key_list[2] = {
    u8"guid",
    u8"pair"
};
const char* SyncPlayerU64Ntf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"pair"
};
const std::bitset<2> SyncPlayerU64Ntf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<SyncPlayerSNtf>::module_id = 108;
template<>
const unsigned short Protocol<SyncPlayerSNtf>::protocol_id = 4;

const char* SyncPlayerSNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"pair"
};
const char* SyncPlayerSNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"pair"
};
const std::bitset<2> SyncPlayerSNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<GSCSChannelMsgNtf>::module_id = 108;
template<>
const unsigned short Protocol<GSCSChannelMsgNtf>::protocol_id = 5;

const char* GSCSChannelMsgNtf::Handler::m_key_list[3] = {
    u8"guid",
    u8"channel",
    u8"msg"
};
const char* GSCSChannelMsgNtf::Handler::m_alias_list[3] = {
    u8"guid",
    u8"channel",
    u8"msg"
};
const std::bitset<3> GSCSChannelMsgNtf::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<SubscribeChannelNtf>::module_id = 108;
template<>
const unsigned short Protocol<SubscribeChannelNtf>::protocol_id = 6;

const char* SubscribeChannelNtf::Handler::m_key_list[4] = {
    u8"guid",
    u8"channel",
    u8"channel_key",
    u8"subscribe"
};
const char* SubscribeChannelNtf::Handler::m_alias_list[4] = {
    u8"guid",
    u8"channel",
    u8"channel_key",
    u8"subscribe"
};
const std::bitset<4> SubscribeChannelNtf::Handler::template_assigned_bitset { "1111" };

template<>
const unsigned short Protocol<LogoutCSNtf>::module_id = 108;
template<>
const unsigned short Protocol<LogoutCSNtf>::protocol_id = 7;

const char* LogoutCSNtf::Handler::m_key_list[1] = {
    u8"guid"
};
const char* LogoutCSNtf::Handler::m_alias_list[1] = {
    u8"guid"
};
const std::bitset<1> LogoutCSNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<HeadMsgGSNtf>::module_id = 108;
template<>
const unsigned short Protocol<HeadMsgGSNtf>::protocol_id = 8;

const char* HeadMsgGSNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"msg"
};
const char* HeadMsgGSNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"msg"
};
const std::bitset<2> HeadMsgGSNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<SetChannelIntervalNtf>::module_id = 108;
template<>
const unsigned short Protocol<SetChannelIntervalNtf>::protocol_id = 9;

const char* SetChannelIntervalNtf::Handler::m_key_list[2] = {
    u8"channel",
    u8"interval"
};
const char* SetChannelIntervalNtf::Handler::m_alias_list[2] = {
    u8"channel",
    u8"interval"
};
const std::bitset<2> SetChannelIntervalNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<SetChannelLevelNtf>::module_id = 108;
template<>
const unsigned short Protocol<SetChannelLevelNtf>::protocol_id = 10;

const char* SetChannelLevelNtf::Handler::m_key_list[2] = {
    u8"channel",
    u8"level"
};
const char* SetChannelLevelNtf::Handler::m_alias_list[2] = {
    u8"channel",
    u8"level"
};
const std::bitset<2> SetChannelLevelNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<SyncPlayerAttrNtf>::module_id = 108;
template<>
const unsigned short Protocol<SyncPlayerAttrNtf>::protocol_id = 11;

const char* SyncPlayerAttrNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"attrs"
};
const char* SyncPlayerAttrNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"attrs"
};
const std::bitset<2> SyncPlayerAttrNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<SyncPlayerNameNtf>::module_id = 108;
template<>
const unsigned short Protocol<SyncPlayerNameNtf>::protocol_id = 12;

const char* SyncPlayerNameNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"name"
};
const char* SyncPlayerNameNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"name"
};
const std::bitset<2> SyncPlayerNameNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<WorldChannelMsgGSNtf>::module_id = 108;
template<>
const unsigned short Protocol<WorldChannelMsgGSNtf>::protocol_id = 13;

const char* WorldChannelMsgGSNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"msg"
};
const char* WorldChannelMsgGSNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"msg"
};
const std::bitset<2> WorldChannelMsgGSNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<WorldChannelMsgCSNtf>::module_id = 108;
template<>
const unsigned short Protocol<WorldChannelMsgCSNtf>::protocol_id = 14;

const char* WorldChannelMsgCSNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"msg"
};
const char* WorldChannelMsgCSNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"msg"
};
const std::bitset<2> WorldChannelMsgCSNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<NoticeMsgCSNtf>::module_id = 108;
template<>
const unsigned short Protocol<NoticeMsgCSNtf>::protocol_id = 15;

const char* NoticeMsgCSNtf::Handler::m_key_list[3] = {
    u8"type",
    u8"msg",
    u8"scroll"
};
const char* NoticeMsgCSNtf::Handler::m_alias_list[3] = {
    u8"type",
    u8"msg",
    u8"scroll"
};
const std::bitset<3> NoticeMsgCSNtf::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<ChannelSwitchCSNtf>::module_id = 108;
template<>
const unsigned short Protocol<ChannelSwitchCSNtf>::protocol_id = 16;

const char* ChannelSwitchCSNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"datas"
};
const char* ChannelSwitchCSNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"datas"
};
const std::bitset<2> ChannelSwitchCSNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<EmojiDataCSNtf>::module_id = 108;
template<>
const unsigned short Protocol<EmojiDataCSNtf>::protocol_id = 17;

const char* EmojiDataCSNtf::Handler::m_key_list[2] = {
    u8"guid",
    u8"emojis"
};
const char* EmojiDataCSNtf::Handler::m_alias_list[2] = {
    u8"guid",
    u8"emojis"
};
const std::bitset<2> EmojiDataCSNtf::Handler::template_assigned_bitset { "11" };

