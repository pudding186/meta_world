#include <new>

#include "ClientCS.hpp"

const char* ChannelMsgReq::Handler::m_key_list[2] = {
    u8"channel",
    u8"msg"
};
const char* ChannelMsgReq::Handler::m_alias_list[2] = {
    u8"channel",
    u8"msg"
};
const std::bitset<2> ChannelMsgReq::Handler::template_assigned_bitset { "11" };

const char* ChannelMsgNtf::Handler::m_key_list[2] = {
    u8"channel",
    u8"chat_msg"
};
const char* ChannelMsgNtf::Handler::m_alias_list[2] = {
    u8"channel",
    u8"chat_msg"
};
const std::bitset<2> ChannelMsgNtf::Handler::template_assigned_bitset { "11" };

const char* TipsMsgCSNtf::Handler::m_key_list[1] = {
    u8"msg"
};
const char* TipsMsgCSNtf::Handler::m_alias_list[1] = {
    u8"msg"
};
const std::bitset<1> TipsMsgCSNtf::Handler::template_assigned_bitset { "1" };

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

const char* EmojiDataNtf::Handler::m_key_list[1] = {
    u8"emojis"
};
const char* EmojiDataNtf::Handler::m_alias_list[1] = {
    u8"emojis"
};
const std::bitset<1> EmojiDataNtf::Handler::template_assigned_bitset { "1" };

