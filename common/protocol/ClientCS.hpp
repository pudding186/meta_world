#pragma once

#include "net_data.hpp"

//===============包含的其他协议文件===============
#include "StructDef.hpp"
 //===============宏定义开始===============
//===============宏定义结束===============

//===============数据定义开始===============
struct ChannelMsgReq:Protocol<ChannelMsgReq>
{
	unsigned char             channel; //1：当前 2：队伍 3：帮派 4：地图 5：p2p 6：私聊 7：喇叭 8：招募 9：门派 10：世界
	DataArray<unsigned char, unsigned short> msg; //消息内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(channel);

		net_data.AddArray(msg);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(channel);

		net_data.AddArray(msg);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(channel))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(channel))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	void Reset(void)
	{
		channel = 0;

		msg.clear();

	}
	ChannelMsgReq& operator= (const ChannelMsgReq& src)
	{
		channel = src.channel;

		msg = src.msg;

		return *this;
	}
	bool operator==(const ChannelMsgReq& src) const
	{
		if (channel != src.channel)
		{
			return false;
		}

		if (msg != src.msg)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ChannelMsgReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ChannelMsgReq& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
        }

        bool Key(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            if (m_state >= static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0])))
            {
                m_state = 0;
            }

            if (strncmp(str, m_alias_list[m_state], length) && 
                strncmp(str, m_key_list[m_state], length))
            {
                for (size_t i = 0; i < (sizeof(m_key_list)/sizeof(m_key_list[0])); i++)
                {
                    if (!strncmp(str, m_alias_list[i], length) ||
                        !strncmp(str, m_key_list[i], length))
                    {
                        m_state = static_cast<int>(i);
                        return true;
                    }
                }

                m_state = static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0]));
                return true;
            }

            return true;
        }

        bool RawNumber(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.channel, str, length);
            }
            break;
            default:
            {
                return true;
            }
            }

            m_state++;
            return true;
        }

        bool String(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.msg, this);
                handler.String(str, length, copy);
            }
            break;
            default:
            {
                m_state = 0;
                return true;
            }
            }

            m_state++;
            return true;
        }

        JsonHandler* StartObject() override
        {
            m_default_handler.Reset(this);
            return &m_default_handler;
        }

        bool EndObject(JsonHandler* childen, rapidjson::SizeType memberCount) override
        {
            (void)childen;
            (void)memberCount;
            return true;
        }
        JsonHandler* StartArray() override
        {
            m_default_handler.Reset(this);
            return &m_default_handler;
        }

        bool EndArray(JsonHandler* childen, rapidjson::SizeType elementCount) override
        {
            (void)childen;
            (void)elementCount;
            return true;
        }
        bool IsAllMemberSet(void)
        {
            if ((m_assigned_bitset ^ template_assigned_bitset).any())
            {
                return false;
            }

            return true;
        }
    private:
        ChannelMsgReq& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<2> m_assigned_bitset;

        static const std::bitset<2> template_assigned_bitset;
        static const char* m_key_list[2];
        static const char* m_alias_list[2];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"channel", channel);

		json_data.Writer().Key(u8"msg", sizeof(u8"msg") - 1);
		json_data.AddArray(msg);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ChannelMsgReq::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ChannelMsgNtf:Protocol<ChannelMsgNtf>
{
	unsigned char             channel; //1：当前 2：队伍 3：帮派 4：地图 5：p2p 6：私聊 7：喇叭 8：招募 9：门派 10：世界
	ChatMsg                   chat_msg; //消息内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(channel);

		chat_msg.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(channel);

		chat_msg.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(channel))
			return false;

		if (!chat_msg.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(channel))
			return false;

		if (!chat_msg.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		channel = 0;

		chat_msg.Reset();

	}
	ChannelMsgNtf& operator= (const ChannelMsgNtf& src)
	{
		channel = src.channel;

		chat_msg = src.chat_msg;

		return *this;
	}
	bool operator==(const ChannelMsgNtf& src) const
	{
		if (channel != src.channel)
		{
			return false;
		}

		if (chat_msg != src.chat_msg)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ChannelMsgNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ChannelMsgNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_chat_msg_handler(m_data.chat_msg, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_chat_msg_handler.ResetState();
        }

        bool Key(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            if (m_state >= static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0])))
            {
                m_state = 0;
            }

            if (strncmp(str, m_alias_list[m_state], length) && 
                strncmp(str, m_key_list[m_state], length))
            {
                for (size_t i = 0; i < (sizeof(m_key_list)/sizeof(m_key_list[0])); i++)
                {
                    if (!strncmp(str, m_alias_list[i], length) ||
                        !strncmp(str, m_key_list[i], length))
                    {
                        m_state = static_cast<int>(i);
                        return true;
                    }
                }

                m_state = static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0]));
                return true;
            }

            return true;
        }

        bool RawNumber(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.channel, str, length);
            }
            break;
            default:
            {
                return true;
            }
            }

            m_state++;
            return true;
        }

        bool String(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)str;
            (void)length;
            (void)copy;

            return true;
        }

        JsonHandler* StartObject() override
        {
            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_chat_msg_handler;
            }
            break;
            default:
            {
                m_default_handler.Reset(this);
                return &m_default_handler;
            }
            }

        }

        bool EndObject(JsonHandler* childen, rapidjson::SizeType memberCount) override
        {
            (void)memberCount;

            if (childen != &m_default_handler)
            {
                m_state++;
            }
            else
            {
                m_state = 0;
            }

            return true;
        }
        JsonHandler* StartArray() override
        {
            m_default_handler.Reset(this);
            return &m_default_handler;
        }

        bool EndArray(JsonHandler* childen, rapidjson::SizeType elementCount) override
        {
            (void)childen;
            (void)elementCount;
            return true;
        }
        bool IsAllMemberSet(void)
        {
            if ((m_assigned_bitset ^ template_assigned_bitset).any())
            {
                return false;
            }

            if (!m_chat_msg_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ChannelMsgNtf& m_data;
        ChatMsg::Handler m_chat_msg_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<2> m_assigned_bitset;

        static const std::bitset<2> template_assigned_bitset;
        static const char* m_key_list[2];
        static const char* m_alias_list[2];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"channel", channel);

		json_data.Writer().Key(u8"chat_msg", sizeof(u8"chat_msg") - 1);
		chat_msg.ToJson(json_data);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ChannelMsgNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct TipsMsgCSNtf:Protocol<TipsMsgCSNtf>
{
	DataArray<unsigned char, unsigned short> msg; //消息内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(msg);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddArray(msg);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	void Reset(void)
	{
		msg.clear();

	}
	TipsMsgCSNtf& operator= (const TipsMsgCSNtf& src)
	{
		msg = src.msg;

		return *this;
	}
	bool operator==(const TipsMsgCSNtf& src) const
	{
		if (msg != src.msg)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TipsMsgCSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TipsMsgCSNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
        }

        bool Key(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            if (m_state >= static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0])))
            {
                m_state = 0;
            }

            if (strncmp(str, m_alias_list[m_state], length) && 
                strncmp(str, m_key_list[m_state], length))
            {
                for (size_t i = 0; i < (sizeof(m_key_list)/sizeof(m_key_list[0])); i++)
                {
                    if (!strncmp(str, m_alias_list[i], length) ||
                        !strncmp(str, m_key_list[i], length))
                    {
                        m_state = static_cast<int>(i);
                        return true;
                    }
                }

                m_state = static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0]));
                return true;
            }

            return true;
        }

        bool RawNumber(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)str;
            (void)length;
            (void)copy;

            return true;
        }

        bool String(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.msg, this);
                handler.String(str, length, copy);
            }
            break;
            default:
            {
                m_state = 0;
                return true;
            }
            }

            m_state++;
            return true;
        }

        JsonHandler* StartObject() override
        {
            m_default_handler.Reset(this);
            return &m_default_handler;
        }

        bool EndObject(JsonHandler* childen, rapidjson::SizeType memberCount) override
        {
            (void)childen;
            (void)memberCount;
            return true;
        }
        JsonHandler* StartArray() override
        {
            m_default_handler.Reset(this);
            return &m_default_handler;
        }

        bool EndArray(JsonHandler* childen, rapidjson::SizeType elementCount) override
        {
            (void)childen;
            (void)elementCount;
            return true;
        }
        bool IsAllMemberSet(void)
        {
            if ((m_assigned_bitset ^ template_assigned_bitset).any())
            {
                return false;
            }

            return true;
        }
    private:
        TipsMsgCSNtf& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<1> m_assigned_bitset;

        static const std::bitset<1> template_assigned_bitset;
        static const char* m_key_list[1];
        static const char* m_alias_list[1];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.Writer().Key(u8"msg", sizeof(u8"msg") - 1);
		json_data.AddArray(msg);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        TipsMsgCSNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct NoticeMsgNtf:Protocol<NoticeMsgNtf>
{
	unsigned char             type; //类型由客户端自定义,对服务器无意义
	DataArray<unsigned char, unsigned short> msg; //消息内容
	unsigned char             scroll; //消息滚动次数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddArray(msg);

		net_data.AddIntegral(scroll);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(type);

		net_data.AddArray(msg);

		net_data.AddIntegral(scroll);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		if (!net_data.DelIntegral(scroll))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(type))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		if (!net_data.DelIntegral(scroll))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		msg.clear();

		scroll = 0;

	}
	NoticeMsgNtf& operator= (const NoticeMsgNtf& src)
	{
		type = src.type;

		msg = src.msg;

		scroll = src.scroll;

		return *this;
	}
	bool operator==(const NoticeMsgNtf& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (msg != src.msg)
		{
			return false;
		}

		if (scroll != src.scroll)
		{
			return false;
		}

		return true;
	}
	bool operator != (const NoticeMsgNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(NoticeMsgNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
        }

        bool Key(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            if (m_state >= static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0])))
            {
                m_state = 0;
            }

            if (strncmp(str, m_alias_list[m_state], length) && 
                strncmp(str, m_key_list[m_state], length))
            {
                for (size_t i = 0; i < (sizeof(m_key_list)/sizeof(m_key_list[0])); i++)
                {
                    if (!strncmp(str, m_alias_list[i], length) ||
                        !strncmp(str, m_key_list[i], length))
                    {
                        m_state = static_cast<int>(i);
                        return true;
                    }
                }

                m_state = static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0]));
                return true;
            }

            return true;
        }

        bool RawNumber(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.type, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.scroll, str, length);
            }
            break;
            default:
            {
                return true;
            }
            }

            m_state++;
            return true;
        }

        bool String(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.msg, this);
                handler.String(str, length, copy);
            }
            break;
            default:
            {
                m_state = 0;
                return true;
            }
            }

            m_state++;
            return true;
        }

        JsonHandler* StartObject() override
        {
            m_default_handler.Reset(this);
            return &m_default_handler;
        }

        bool EndObject(JsonHandler* childen, rapidjson::SizeType memberCount) override
        {
            (void)childen;
            (void)memberCount;
            return true;
        }
        JsonHandler* StartArray() override
        {
            m_default_handler.Reset(this);
            return &m_default_handler;
        }

        bool EndArray(JsonHandler* childen, rapidjson::SizeType elementCount) override
        {
            (void)childen;
            (void)elementCount;
            return true;
        }
        bool IsAllMemberSet(void)
        {
            if ((m_assigned_bitset ^ template_assigned_bitset).any())
            {
                return false;
            }

            return true;
        }
    private:
        NoticeMsgNtf& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<3> m_assigned_bitset;

        static const std::bitset<3> template_assigned_bitset;
        static const char* m_key_list[3];
        static const char* m_alias_list[3];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"type", type);

		json_data.Writer().Key(u8"msg", sizeof(u8"msg") - 1);
		json_data.AddArray(msg);

		json_data.AddIntegral(u8"scroll", scroll);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        NoticeMsgNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct EmojiDataNtf:Protocol<EmojiDataNtf>
{
	EmojiData                 emojis; //频道开关数组
	bool EnCode(NetEnCode& net_data)
	{
		emojis.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		emojis.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!emojis.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!emojis.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		emojis.Reset();

	}
	EmojiDataNtf& operator= (const EmojiDataNtf& src)
	{
		emojis = src.emojis;

		return *this;
	}
	bool operator==(const EmojiDataNtf& src) const
	{
		if (emojis != src.emojis)
		{
			return false;
		}

		return true;
	}
	bool operator != (const EmojiDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(EmojiDataNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_emojis_handler(m_data.emojis, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_emojis_handler.ResetState();
        }

        bool Key(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)copy;

            if (m_state >= static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0])))
            {
                m_state = 0;
            }

            if (strncmp(str, m_alias_list[m_state], length) && 
                strncmp(str, m_key_list[m_state], length))
            {
                for (size_t i = 0; i < (sizeof(m_key_list)/sizeof(m_key_list[0])); i++)
                {
                    if (!strncmp(str, m_alias_list[i], length) ||
                        !strncmp(str, m_key_list[i], length))
                    {
                        m_state = static_cast<int>(i);
                        return true;
                    }
                }

                m_state = static_cast<unsigned int>(sizeof(m_key_list) / sizeof(m_key_list[0]));
                return true;
            }

            return true;
        }

        bool RawNumber(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)str;
            (void)length;
            (void)copy;

            return true;
        }

        bool String(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)str;
            (void)length;
            (void)copy;

            return true;
        }

        JsonHandler* StartObject() override
        {
            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                return &m_emojis_handler;
            }
            break;
            default:
            {
                m_default_handler.Reset(this);
                return &m_default_handler;
            }
            }

        }

        bool EndObject(JsonHandler* childen, rapidjson::SizeType memberCount) override
        {
            (void)memberCount;

            if (childen != &m_default_handler)
            {
                m_state++;
            }
            else
            {
                m_state = 0;
            }

            return true;
        }
        JsonHandler* StartArray() override
        {
            m_default_handler.Reset(this);
            return &m_default_handler;
        }

        bool EndArray(JsonHandler* childen, rapidjson::SizeType elementCount) override
        {
            (void)childen;
            (void)elementCount;
            return true;
        }
        bool IsAllMemberSet(void)
        {
            if ((m_assigned_bitset ^ template_assigned_bitset).any())
            {
                return false;
            }

            if (!m_emojis_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        EmojiDataNtf& m_data;
        EmojiData::Handler m_emojis_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<1> m_assigned_bitset;

        static const std::bitset<1> template_assigned_bitset;
        static const char* m_key_list[1];
        static const char* m_alias_list[1];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.Writer().Key(u8"emojis", sizeof(u8"emojis") - 1);
		emojis.ToJson(json_data);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        EmojiDataNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

//===============数据定义结束===============
template<typename D>
class CClientCS
{
public:
	CClientCS()
	{
		size_t max_protocol_size = 0;
		if (sizeof(ChannelMsgReq) > max_protocol_size)
			max_protocol_size = sizeof(ChannelMsgReq);

		if (sizeof(ChannelMsgNtf) > max_protocol_size)
			max_protocol_size = sizeof(ChannelMsgNtf);

		if (sizeof(TipsMsgCSNtf) > max_protocol_size)
			max_protocol_size = sizeof(TipsMsgCSNtf);

		if (sizeof(NoticeMsgNtf) > max_protocol_size)
			max_protocol_size = sizeof(NoticeMsgNtf);

		if (sizeof(EmojiDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(EmojiDataNtf);

		m_protocol_buffer = S_MALLOC(max_protocol_size);
	}

	virtual ~CClientCS()
	{
		if (m_protocol_buffer)
		{
			S_FREE(m_protocol_buffer);
			m_protocol_buffer = 0;
		}
	}

	template<typename T>
	bool BuildProtocol(Protocol<T>& proto, NetEnCode& net_data)
	{
		if (proto.module_id != 104)
			return false;

		net_data.AddIntegral(proto.module_id);
		net_data.AddIntegral(proto.protocol_id);

		return static_cast<T&>(proto).EnCode(net_data);
	}

	bool BuildProtocol(protocol_base* proto, NetEnCode& net_data)
	{
		if (proto->ModuleId() != 104)
			return false;

		net_data.AddIntegral(proto->ModuleId());
		net_data.AddIntegral(proto->ProtocolId());

		return proto->EnCodeEx(net_data);
	}

	const char* ProtocolName(unsigned short protocol_id) const
	{
		static char unknow_protocol[32];

		switch (protocol_id)
		{
		case 1:
		{
			return ChannelMsgReq::SName();
		}
		break;
		case 2:
		{
			return ChannelMsgNtf::SName();
		}
		break;
		case 3:
		{
			return TipsMsgCSNtf::SName();
		}
		break;
		case 4:
		{
			return NoticeMsgNtf::SName();
		}
		break;
		case 5:
		{
			return EmojiDataNtf::SName();
		}
		break;
		default:
		{
			snprintf(unknow_protocol, sizeof(unknow_protocol), u8"Unknow Protocol Id = %d", protocol_id);
			return unknow_protocol;
		}
		}
	}

	bool HandleProtocol(NetDeCode& net_data)
	{
		unsigned short m_id = 0;
		unsigned short p_id = 0;
		size_t net_data_pos = net_data.CurPos();

		if (!net_data.DelIntegral(m_id) || !net_data.DelIntegral(p_id))
		{
			net_data.Reset(net_data_pos);
			return false;
		}

		if (m_id != 104)
		{
			net_data.Reset(net_data_pos);
			return false;
		}

		switch(p_id)
		{
		case 1:
		{
			ChannelMsgReq* proto = new(m_protocol_buffer) ChannelMsgReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ChannelMsgReq(*proto);
				proto->~ChannelMsgReq();
				return true;
			}
			else
			{
				proto->~ChannelMsgReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 2:
		{
			ChannelMsgNtf* proto = new(m_protocol_buffer) ChannelMsgNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ChannelMsgNtf(*proto);
				proto->~ChannelMsgNtf();
				return true;
			}
			else
			{
				proto->~ChannelMsgNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 3:
		{
			TipsMsgCSNtf* proto = new(m_protocol_buffer) TipsMsgCSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_TipsMsgCSNtf(*proto);
				proto->~TipsMsgCSNtf();
				return true;
			}
			else
			{
				proto->~TipsMsgCSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 4:
		{
			NoticeMsgNtf* proto = new(m_protocol_buffer) NoticeMsgNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_NoticeMsgNtf(*proto);
				proto->~NoticeMsgNtf();
				return true;
			}
			else
			{
				proto->~NoticeMsgNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 5:
		{
			EmojiDataNtf* proto = new(m_protocol_buffer) EmojiDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_EmojiDataNtf(*proto);
				proto->~EmojiDataNtf();
				return true;
			}
			else
			{
				proto->~EmojiDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		default:
			return false;
		}

	}

	static inline unsigned short GetModuleID(void){ return 104; }

	static inline unsigned short GetProtocolNum(void){ return 5; }

	static const unsigned short module_id = 104;

	static const unsigned short protocol_num = 5;

//===============以下协议回调函数需要使用者来实现===============
	void OnRecv_ChannelMsgReq(ChannelMsgReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ChannelMsgNtf(ChannelMsgNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_TipsMsgCSNtf(TipsMsgCSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_NoticeMsgNtf(NoticeMsgNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_EmojiDataNtf(EmojiDataNtf& rstProtocol){ (void)(rstProtocol); };
private:
	 void* m_protocol_buffer;

};
