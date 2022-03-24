#include <new>

#include "ClientCS.hpp"

template<>
const unsigned short Protocol<ChannelMsgReq>::module_id = 104;
template<>
const unsigned short Protocol<ChannelMsgReq>::protocol_id = 1;

const char* ChannelMsgReq::Handler::m_key_list[2] = {
    u8"channel",
    u8"msg"
};
const char* ChannelMsgReq::Handler::m_alias_list[2] = {
    u8"channel",
    u8"msg"
};
const std::bitset<2> ChannelMsgReq::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<ChannelMsgNtf>::module_id = 104;
template<>
const unsigned short Protocol<ChannelMsgNtf>::protocol_id = 2;

const char* ChannelMsgNtf::Handler::m_key_list[2] = {
    u8"channel",
    u8"chat_msg"
};
const char* ChannelMsgNtf::Handler::m_alias_list[2] = {
    u8"channel",
    u8"chat_msg"
};
const std::bitset<2> ChannelMsgNtf::Handler::template_assigned_bitset { "11" };

template<>
const unsigned short Protocol<TipsMsgCSNtf>::module_id = 104;
template<>
const unsigned short Protocol<TipsMsgCSNtf>::protocol_id = 3;

const char* TipsMsgCSNtf::Handler::m_key_list[1] = {
    u8"msg"
};
const char* TipsMsgCSNtf::Handler::m_alias_list[1] = {
    u8"msg"
};
const std::bitset<1> TipsMsgCSNtf::Handler::template_assigned_bitset { "1" };

template<>
const unsigned short Protocol<NoticeMsgNtf>::module_id = 104;
template<>
const unsigned short Protocol<NoticeMsgNtf>::protocol_id = 4;

const char* NoticeMsgNtf::Handler::m_key_list[3] = {
    u8"type",
    u8"msg",
    u8"scroll"
};
const char* NoticeMsgNtf::Handler::m_alias_list[3] = {
    u8"type",
    u8"msg",
    u8"scroll"
};
const std::bitset<3> NoticeMsgNtf::Handler::template_assigned_bitset { "111" };

template<>
const unsigned short Protocol<EmojiDataNtf>::module_id = 104;
template<>
const unsigned short Protocol<EmojiDataNtf>::protocol_id = 5;

const char* EmojiDataNtf::Handler::m_key_list[1] = {
    u8"emojis"
};
const char* EmojiDataNtf::Handler::m_alias_list[1] = {
    u8"emojis"
};
const std::bitset<1> EmojiDataNtf::Handler::template_assigned_bitset { "1" };

