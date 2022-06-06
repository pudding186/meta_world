#pragma once

#include "net_data.hpp"

//===============包含的其他协议文件===============
#include "StructDef.hpp"
 //===============宏定义开始===============
//===============宏定义结束===============

//===============数据定义开始===============
struct LoginCSNtf:TProtocol<LoginCSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 1;

	unsigned int              uid; //账号ID
	unsigned char             sid; //子区ID
	unsigned long long        guid; //玩家GUID
	DataArray<AttrData, unsigned short> attrs; //属性数据
	char                      name[32]; //玩家名字
	unsigned int              gate_index; //gate服务器索引
	unsigned int              game_index; //game服务器索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddArray(attrs);

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(gate_index);

		net_data.AddIntegral(game_index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddArray(attrs);

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(gate_index);

		net_data.AddIntegral(game_index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(gate_index))
			return false;

		if (!net_data.DelIntegral(game_index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(gate_index))
			return false;

		if (!net_data.DelIntegral(game_index))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		guid = 0;

		attrs.clear();

		name[0] = 0;

		gate_index = 0;

		game_index = 0;

	}
	LoginCSNtf& operator= (const LoginCSNtf& src)
	{
		uid = src.uid;

		sid = src.sid;

		guid = src.guid;

		attrs = src.attrs;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		gate_index = src.gate_index;

		game_index = src.game_index;

		return *this;
	}
	bool operator==(const LoginCSNtf& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
		{
			return false;
		}

		if (guid != src.guid)
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (gate_index != src.gate_index)
		{
			return false;
		}

		if (game_index != src.game_index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoginCSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoginCSNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_attrs_handler(m_data.attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_attrs_handler.ResetState();
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
                JsonDelIntegral(m_data.uid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.sid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.guid, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gate_index, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.game_index, str, length);
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
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.name, str, length);
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
            switch (m_state)
            {
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_attrs_handler;
            }
            break;
            default:
            {
                m_default_handler.Reset(this);
                return &m_default_handler;
            }
            }

        }

        bool EndArray(JsonHandler* childen, rapidjson::SizeType elementCount) override
        {
            (void)elementCount;

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
        bool IsAllMemberSet(void)
        {
            if ((m_assigned_bitset ^ template_assigned_bitset).any())
            {
                return false;
            }

            if (!m_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoginCSNtf& m_data;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<7> m_assigned_bitset;

        static const std::bitset<7> template_assigned_bitset;
        static const char* m_key_list[7];
        static const char* m_alias_list[7];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"guid", guid);

		json_data.Writer().Key(u8"attrs", sizeof(u8"attrs") - 1);
		json_data.AddArray(attrs);

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddIntegral(u8"gate_index", gate_index);

		json_data.AddIntegral(u8"game_index", game_index);

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
        
        LoginCSNtf::Handler h(*this, nullptr);
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

struct SyncPlayerU32Ntf:TProtocol<SyncPlayerU32Ntf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 2;

	unsigned long long        guid; //玩家GUID
	KeyValueU32               pair; //信息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		pair.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		pair.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!pair.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!pair.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		pair.Reset();

	}
	SyncPlayerU32Ntf& operator= (const SyncPlayerU32Ntf& src)
	{
		guid = src.guid;

		pair = src.pair;

		return *this;
	}
	bool operator==(const SyncPlayerU32Ntf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (pair != src.pair)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SyncPlayerU32Ntf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SyncPlayerU32Ntf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_pair_handler(m_data.pair, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_pair_handler.ResetState();
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
                JsonDelIntegral(m_data.guid, str, length);
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
                return &m_pair_handler;
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

            if (!m_pair_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        SyncPlayerU32Ntf& m_data;
        KeyValueU32::Handler m_pair_handler;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.Writer().Key(u8"pair", sizeof(u8"pair") - 1);
		pair.ToJson(json_data);

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
        
        SyncPlayerU32Ntf::Handler h(*this, nullptr);
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

struct SyncPlayerU64Ntf:TProtocol<SyncPlayerU64Ntf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 3;

	unsigned long long        guid; //玩家GUID
	KeyValueU64               pair; //信息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		pair.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		pair.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!pair.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!pair.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		pair.Reset();

	}
	SyncPlayerU64Ntf& operator= (const SyncPlayerU64Ntf& src)
	{
		guid = src.guid;

		pair = src.pair;

		return *this;
	}
	bool operator==(const SyncPlayerU64Ntf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (pair != src.pair)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SyncPlayerU64Ntf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SyncPlayerU64Ntf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_pair_handler(m_data.pair, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_pair_handler.ResetState();
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
                JsonDelIntegral(m_data.guid, str, length);
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
                return &m_pair_handler;
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

            if (!m_pair_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        SyncPlayerU64Ntf& m_data;
        KeyValueU64::Handler m_pair_handler;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.Writer().Key(u8"pair", sizeof(u8"pair") - 1);
		pair.ToJson(json_data);

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
        
        SyncPlayerU64Ntf::Handler h(*this, nullptr);
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

struct SyncPlayerSNtf:TProtocol<SyncPlayerSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 4;

	unsigned long long        guid; //玩家GUID
	KeyValueS                 pair; //信息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		pair.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		pair.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!pair.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!pair.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		pair.Reset();

	}
	SyncPlayerSNtf& operator= (const SyncPlayerSNtf& src)
	{
		guid = src.guid;

		pair = src.pair;

		return *this;
	}
	bool operator==(const SyncPlayerSNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (pair != src.pair)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SyncPlayerSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SyncPlayerSNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_pair_handler(m_data.pair, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_pair_handler.ResetState();
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
                JsonDelIntegral(m_data.guid, str, length);
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
                return &m_pair_handler;
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

            if (!m_pair_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        SyncPlayerSNtf& m_data;
        KeyValueS::Handler m_pair_handler;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.Writer().Key(u8"pair", sizeof(u8"pair") - 1);
		pair.ToJson(json_data);

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
        
        SyncPlayerSNtf::Handler h(*this, nullptr);
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

struct GSCSChannelMsgNtf:TProtocol<GSCSChannelMsgNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 5;

	unsigned long long        guid; //玩家GUID
	unsigned char             channel; //1：当前 2：队伍 3：帮派 4：地图 5：p2p 6：私聊 7：喇叭 8：招募 9：门派 10：世界
	DataArray<unsigned char, unsigned short> msg; //消息内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(channel);

		net_data.AddArray(msg);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(channel);

		net_data.AddArray(msg);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(channel))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(channel))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		channel = 0;

		msg.clear();

	}
	GSCSChannelMsgNtf& operator= (const GSCSChannelMsgNtf& src)
	{
		guid = src.guid;

		channel = src.channel;

		msg = src.msg;

		return *this;
	}
	bool operator==(const GSCSChannelMsgNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

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
	bool operator != (const GSCSChannelMsgNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GSCSChannelMsgNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guid, str, length);
            }
            break;
            case 1:
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
            case 2:
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
        GSCSChannelMsgNtf& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

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
        
        GSCSChannelMsgNtf::Handler h(*this, nullptr);
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

struct SubscribeChannelNtf:TProtocol<SubscribeChannelNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 6;

	unsigned long long        guid; //玩家GUID
	unsigned char             channel; //1：当前 2：队伍 3：帮派 4：地图 5：p2p 6：私聊 7：喇叭 8：招募 9：门派 10：世界
	unsigned long long        channel_key; //频道KEY
	unsigned char             subscribe; //1: 订阅; 0: 取消订阅
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(channel);

		net_data.AddIntegral(channel_key);

		net_data.AddIntegral(subscribe);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(channel);

		net_data.AddIntegral(channel_key);

		net_data.AddIntegral(subscribe);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(channel))
			return false;

		if (!net_data.DelIntegral(channel_key))
			return false;

		if (!net_data.DelIntegral(subscribe))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(channel))
			return false;

		if (!net_data.DelIntegral(channel_key))
			return false;

		if (!net_data.DelIntegral(subscribe))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		channel = 0;

		channel_key = 0;

		subscribe = 0;

	}
	SubscribeChannelNtf& operator= (const SubscribeChannelNtf& src)
	{
		guid = src.guid;

		channel = src.channel;

		channel_key = src.channel_key;

		subscribe = src.subscribe;

		return *this;
	}
	bool operator==(const SubscribeChannelNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (channel != src.channel)
		{
			return false;
		}

		if (channel_key != src.channel_key)
		{
			return false;
		}

		if (subscribe != src.subscribe)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SubscribeChannelNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SubscribeChannelNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.channel, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.channel_key, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.subscribe, str, length);
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
        SubscribeChannelNtf& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<4> m_assigned_bitset;

        static const std::bitset<4> template_assigned_bitset;
        static const char* m_key_list[4];
        static const char* m_alias_list[4];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"channel", channel);

		json_data.AddIntegral(u8"channel_key", channel_key);

		json_data.AddIntegral(u8"subscribe", subscribe);

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
        
        SubscribeChannelNtf::Handler h(*this, nullptr);
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

struct LogoutCSNtf:TProtocol<LogoutCSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 7;

	unsigned long long        guid; //玩家GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

	}
	LogoutCSNtf& operator= (const LogoutCSNtf& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const LogoutCSNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LogoutCSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LogoutCSNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guid, str, length);
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
        LogoutCSNtf& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

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
        
        LogoutCSNtf::Handler h(*this, nullptr);
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

struct HeadMsgGSNtf:TProtocol<HeadMsgGSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 8;

	unsigned long long        guid; //对象GUID
	DataArray<unsigned char, unsigned short> msg; //消息内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(msg);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(msg);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		msg.clear();

	}
	HeadMsgGSNtf& operator= (const HeadMsgGSNtf& src)
	{
		guid = src.guid;

		msg = src.msg;

		return *this;
	}
	bool operator==(const HeadMsgGSNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (msg != src.msg)
		{
			return false;
		}

		return true;
	}
	bool operator != (const HeadMsgGSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(HeadMsgGSNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guid, str, length);
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
        HeadMsgGSNtf& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

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
        
        HeadMsgGSNtf::Handler h(*this, nullptr);
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

struct SetChannelIntervalNtf:TProtocol<SetChannelIntervalNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 9;

	unsigned char             channel; //1：当前 2：队伍 3：帮派 4：地图 5：p2p 6：私聊 7：喇叭 8：招募 9：门派 10：世界
	unsigned int              interval; //通道间隔
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(channel);

		net_data.AddIntegral(interval);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(channel);

		net_data.AddIntegral(interval);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(channel))
			return false;

		if (!net_data.DelIntegral(interval))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(channel))
			return false;

		if (!net_data.DelIntegral(interval))
			return false;

		return true;
	}
	void Reset(void)
	{
		channel = 0;

		interval = 0;

	}
	SetChannelIntervalNtf& operator= (const SetChannelIntervalNtf& src)
	{
		channel = src.channel;

		interval = src.interval;

		return *this;
	}
	bool operator==(const SetChannelIntervalNtf& src) const
	{
		if (channel != src.channel)
		{
			return false;
		}

		if (interval != src.interval)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SetChannelIntervalNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SetChannelIntervalNtf& data, JsonHandler* parent = nullptr)
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
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.interval, str, length);
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
        SetChannelIntervalNtf& m_data;
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

		json_data.AddIntegral(u8"interval", interval);

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
        
        SetChannelIntervalNtf::Handler h(*this, nullptr);
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

struct SetChannelLevelNtf:TProtocol<SetChannelLevelNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 10;

	unsigned char             channel; //1：当前 2：队伍 3：帮派 4：地图 5：p2p 6：私聊 7：喇叭 8：招募 9：门派 10：世界
	unsigned int              level; //通道间隔
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(channel);

		net_data.AddIntegral(level);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(channel);

		net_data.AddIntegral(level);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(channel))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(channel))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		return true;
	}
	void Reset(void)
	{
		channel = 0;

		level = 0;

	}
	SetChannelLevelNtf& operator= (const SetChannelLevelNtf& src)
	{
		channel = src.channel;

		level = src.level;

		return *this;
	}
	bool operator==(const SetChannelLevelNtf& src) const
	{
		if (channel != src.channel)
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SetChannelLevelNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SetChannelLevelNtf& data, JsonHandler* parent = nullptr)
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
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
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
        SetChannelLevelNtf& m_data;
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

		json_data.AddIntegral(u8"level", level);

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
        
        SetChannelLevelNtf::Handler h(*this, nullptr);
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

struct SyncPlayerAttrNtf:TProtocol<SyncPlayerAttrNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 11;

	unsigned long long        guid; //玩家GUID
	DataArray<AttrData, unsigned short> attrs; //属性数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(attrs);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(attrs);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		attrs.clear();

	}
	SyncPlayerAttrNtf& operator= (const SyncPlayerAttrNtf& src)
	{
		guid = src.guid;

		attrs = src.attrs;

		return *this;
	}
	bool operator==(const SyncPlayerAttrNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SyncPlayerAttrNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SyncPlayerAttrNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_attrs_handler(m_data.attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_attrs_handler.ResetState();
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
                JsonDelIntegral(m_data.guid, str, length);
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
            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_attrs_handler;
            }
            break;
            default:
            {
                m_default_handler.Reset(this);
                return &m_default_handler;
            }
            }

        }

        bool EndArray(JsonHandler* childen, rapidjson::SizeType elementCount) override
        {
            (void)elementCount;

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
        bool IsAllMemberSet(void)
        {
            if ((m_assigned_bitset ^ template_assigned_bitset).any())
            {
                return false;
            }

            if (!m_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        SyncPlayerAttrNtf& m_data;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.Writer().Key(u8"attrs", sizeof(u8"attrs") - 1);
		json_data.AddArray(attrs);

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
        
        SyncPlayerAttrNtf::Handler h(*this, nullptr);
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

struct SyncPlayerNameNtf:TProtocol<SyncPlayerNameNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 12;

	unsigned long long        guid; //玩家GUID
	char                      name[32]; //玩家名字
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddString(name, sizeof(name));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddString(name, sizeof(name));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		name[0] = 0;

	}
	SyncPlayerNameNtf& operator= (const SyncPlayerNameNtf& src)
	{
		guid = src.guid;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const SyncPlayerNameNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const SyncPlayerNameNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SyncPlayerNameNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guid, str, length);
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
                JsonDelString(m_data.name, str, length);
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
        SyncPlayerNameNtf& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddString(u8"name", name, sizeof(name));

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
        
        SyncPlayerNameNtf::Handler h(*this, nullptr);
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

struct WorldChannelMsgGSNtf:TProtocol<WorldChannelMsgGSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 13;

	unsigned long long        guid; //玩家GUID
	DataArray<unsigned char, unsigned short> msg; //消息内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(msg);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(msg);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		msg.clear();

	}
	WorldChannelMsgGSNtf& operator= (const WorldChannelMsgGSNtf& src)
	{
		guid = src.guid;

		msg = src.msg;

		return *this;
	}
	bool operator==(const WorldChannelMsgGSNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (msg != src.msg)
		{
			return false;
		}

		return true;
	}
	bool operator != (const WorldChannelMsgGSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(WorldChannelMsgGSNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guid, str, length);
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
        WorldChannelMsgGSNtf& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

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
        
        WorldChannelMsgGSNtf::Handler h(*this, nullptr);
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

struct WorldChannelMsgCSNtf:TProtocol<WorldChannelMsgCSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 14;

	unsigned long long        guid; //玩家GUID
	DataArray<unsigned char, unsigned short> msg; //消息内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(msg);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(msg);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(msg))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		msg.clear();

	}
	WorldChannelMsgCSNtf& operator= (const WorldChannelMsgCSNtf& src)
	{
		guid = src.guid;

		msg = src.msg;

		return *this;
	}
	bool operator==(const WorldChannelMsgCSNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (msg != src.msg)
		{
			return false;
		}

		return true;
	}
	bool operator != (const WorldChannelMsgCSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(WorldChannelMsgCSNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guid, str, length);
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
        WorldChannelMsgCSNtf& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

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
        
        WorldChannelMsgCSNtf::Handler h(*this, nullptr);
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

struct NoticeMsgCSNtf:TProtocol<NoticeMsgCSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 15;

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
	NoticeMsgCSNtf& operator= (const NoticeMsgCSNtf& src)
	{
		type = src.type;

		msg = src.msg;

		scroll = src.scroll;

		return *this;
	}
	bool operator==(const NoticeMsgCSNtf& src) const
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
	bool operator != (const NoticeMsgCSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(NoticeMsgCSNtf& data, JsonHandler* parent = nullptr)
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
        NoticeMsgCSNtf& m_data;
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
        
        NoticeMsgCSNtf::Handler h(*this, nullptr);
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

struct ChannelSwitchCSNtf:TProtocol<ChannelSwitchCSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 16;

	unsigned long long        guid; //玩家GUID
	DataArray<ChannelSwitchData, unsigned char> datas; //频道开关数组
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(datas);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(datas);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		datas.clear();

	}
	ChannelSwitchCSNtf& operator= (const ChannelSwitchCSNtf& src)
	{
		guid = src.guid;

		datas = src.datas;

		return *this;
	}
	bool operator==(const ChannelSwitchCSNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (datas != src.datas)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ChannelSwitchCSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ChannelSwitchCSNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_datas_handler(m_data.datas, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_datas_handler.ResetState();
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
                JsonDelIntegral(m_data.guid, str, length);
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
            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_datas_handler;
            }
            break;
            default:
            {
                m_default_handler.Reset(this);
                return &m_default_handler;
            }
            }

        }

        bool EndArray(JsonHandler* childen, rapidjson::SizeType elementCount) override
        {
            (void)elementCount;

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
        bool IsAllMemberSet(void)
        {
            if ((m_assigned_bitset ^ template_assigned_bitset).any())
            {
                return false;
            }

            if (!m_datas_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ChannelSwitchCSNtf& m_data;
        DataArrayHandler<ChannelSwitchData, unsigned char, ChannelSwitchData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        ChannelSwitchCSNtf::Handler h(*this, nullptr);
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

struct EmojiDataCSNtf:TProtocol<EmojiDataCSNtf>
{
	static constexpr unsigned short module_id = 108;
	static constexpr unsigned short protocol_id = 17;

	unsigned long long        guid; //玩家GUID
	EmojiData                 emojis; //频道开关数组
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		emojis.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		emojis.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!emojis.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!emojis.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		emojis.Reset();

	}
	EmojiDataCSNtf& operator= (const EmojiDataCSNtf& src)
	{
		guid = src.guid;

		emojis = src.emojis;

		return *this;
	}
	bool operator==(const EmojiDataCSNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (emojis != src.emojis)
		{
			return false;
		}

		return true;
	}
	bool operator != (const EmojiDataCSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(EmojiDataCSNtf& data, JsonHandler* parent = nullptr)
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
            (void)copy;

            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.guid, str, length);
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
        EmojiDataCSNtf& m_data;
        EmojiData::Handler m_emojis_handler;
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

		json_data.AddIntegral(u8"guid", guid);

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
        
        EmojiDataCSNtf::Handler h(*this, nullptr);
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
class CGSCS: public ProtocolModule
{
public:
	CGSCS()
	{
		size_t max_protocol_size = 0;
		if (sizeof(LoginCSNtf) > max_protocol_size)
			max_protocol_size = sizeof(LoginCSNtf);

		if (sizeof(SyncPlayerU32Ntf) > max_protocol_size)
			max_protocol_size = sizeof(SyncPlayerU32Ntf);

		if (sizeof(SyncPlayerU64Ntf) > max_protocol_size)
			max_protocol_size = sizeof(SyncPlayerU64Ntf);

		if (sizeof(SyncPlayerSNtf) > max_protocol_size)
			max_protocol_size = sizeof(SyncPlayerSNtf);

		if (sizeof(GSCSChannelMsgNtf) > max_protocol_size)
			max_protocol_size = sizeof(GSCSChannelMsgNtf);

		if (sizeof(SubscribeChannelNtf) > max_protocol_size)
			max_protocol_size = sizeof(SubscribeChannelNtf);

		if (sizeof(LogoutCSNtf) > max_protocol_size)
			max_protocol_size = sizeof(LogoutCSNtf);

		if (sizeof(HeadMsgGSNtf) > max_protocol_size)
			max_protocol_size = sizeof(HeadMsgGSNtf);

		if (sizeof(SetChannelIntervalNtf) > max_protocol_size)
			max_protocol_size = sizeof(SetChannelIntervalNtf);

		if (sizeof(SetChannelLevelNtf) > max_protocol_size)
			max_protocol_size = sizeof(SetChannelLevelNtf);

		if (sizeof(SyncPlayerAttrNtf) > max_protocol_size)
			max_protocol_size = sizeof(SyncPlayerAttrNtf);

		if (sizeof(SyncPlayerNameNtf) > max_protocol_size)
			max_protocol_size = sizeof(SyncPlayerNameNtf);

		if (sizeof(WorldChannelMsgGSNtf) > max_protocol_size)
			max_protocol_size = sizeof(WorldChannelMsgGSNtf);

		if (sizeof(WorldChannelMsgCSNtf) > max_protocol_size)
			max_protocol_size = sizeof(WorldChannelMsgCSNtf);

		if (sizeof(NoticeMsgCSNtf) > max_protocol_size)
			max_protocol_size = sizeof(NoticeMsgCSNtf);

		if (sizeof(ChannelSwitchCSNtf) > max_protocol_size)
			max_protocol_size = sizeof(ChannelSwitchCSNtf);

		if (sizeof(EmojiDataCSNtf) > max_protocol_size)
			max_protocol_size = sizeof(EmojiDataCSNtf);

		m_protocol_buffer = S_MALLOC(max_protocol_size);
	}

	virtual ~CGSCS()
	{
		if (m_protocol_buffer)
		{
			S_FREE(m_protocol_buffer);
			m_protocol_buffer = 0;
		}
	}

	const char* ProtocolName(unsigned short protocol_id) const
	{
		static char unknow_protocol[32];

		switch (protocol_id)
		{
		case LoginCSNtf::protocol_id:
		{
			return LoginCSNtf::Name();
		}
		break;
		case SyncPlayerU32Ntf::protocol_id:
		{
			return SyncPlayerU32Ntf::Name();
		}
		break;
		case SyncPlayerU64Ntf::protocol_id:
		{
			return SyncPlayerU64Ntf::Name();
		}
		break;
		case SyncPlayerSNtf::protocol_id:
		{
			return SyncPlayerSNtf::Name();
		}
		break;
		case GSCSChannelMsgNtf::protocol_id:
		{
			return GSCSChannelMsgNtf::Name();
		}
		break;
		case SubscribeChannelNtf::protocol_id:
		{
			return SubscribeChannelNtf::Name();
		}
		break;
		case LogoutCSNtf::protocol_id:
		{
			return LogoutCSNtf::Name();
		}
		break;
		case HeadMsgGSNtf::protocol_id:
		{
			return HeadMsgGSNtf::Name();
		}
		break;
		case SetChannelIntervalNtf::protocol_id:
		{
			return SetChannelIntervalNtf::Name();
		}
		break;
		case SetChannelLevelNtf::protocol_id:
		{
			return SetChannelLevelNtf::Name();
		}
		break;
		case SyncPlayerAttrNtf::protocol_id:
		{
			return SyncPlayerAttrNtf::Name();
		}
		break;
		case SyncPlayerNameNtf::protocol_id:
		{
			return SyncPlayerNameNtf::Name();
		}
		break;
		case WorldChannelMsgGSNtf::protocol_id:
		{
			return WorldChannelMsgGSNtf::Name();
		}
		break;
		case WorldChannelMsgCSNtf::protocol_id:
		{
			return WorldChannelMsgCSNtf::Name();
		}
		break;
		case NoticeMsgCSNtf::protocol_id:
		{
			return NoticeMsgCSNtf::Name();
		}
		break;
		case ChannelSwitchCSNtf::protocol_id:
		{
			return ChannelSwitchCSNtf::Name();
		}
		break;
		case EmojiDataCSNtf::protocol_id:
		{
			return EmojiDataCSNtf::Name();
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

		if (m_id != 108)
		{
			net_data.Reset(net_data_pos);
			return false;
		}

		switch(p_id)
		{
		case LoginCSNtf::protocol_id:
		{
			LoginCSNtf* proto = new(m_protocol_buffer) LoginCSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoginCSNtf(*proto);
				proto->~LoginCSNtf();
				return true;
			}
			else
			{
				proto->~LoginCSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SyncPlayerU32Ntf::protocol_id:
		{
			SyncPlayerU32Ntf* proto = new(m_protocol_buffer) SyncPlayerU32Ntf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SyncPlayerU32Ntf(*proto);
				proto->~SyncPlayerU32Ntf();
				return true;
			}
			else
			{
				proto->~SyncPlayerU32Ntf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SyncPlayerU64Ntf::protocol_id:
		{
			SyncPlayerU64Ntf* proto = new(m_protocol_buffer) SyncPlayerU64Ntf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SyncPlayerU64Ntf(*proto);
				proto->~SyncPlayerU64Ntf();
				return true;
			}
			else
			{
				proto->~SyncPlayerU64Ntf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SyncPlayerSNtf::protocol_id:
		{
			SyncPlayerSNtf* proto = new(m_protocol_buffer) SyncPlayerSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SyncPlayerSNtf(*proto);
				proto->~SyncPlayerSNtf();
				return true;
			}
			else
			{
				proto->~SyncPlayerSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case GSCSChannelMsgNtf::protocol_id:
		{
			GSCSChannelMsgNtf* proto = new(m_protocol_buffer) GSCSChannelMsgNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_GSCSChannelMsgNtf(*proto);
				proto->~GSCSChannelMsgNtf();
				return true;
			}
			else
			{
				proto->~GSCSChannelMsgNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SubscribeChannelNtf::protocol_id:
		{
			SubscribeChannelNtf* proto = new(m_protocol_buffer) SubscribeChannelNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SubscribeChannelNtf(*proto);
				proto->~SubscribeChannelNtf();
				return true;
			}
			else
			{
				proto->~SubscribeChannelNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LogoutCSNtf::protocol_id:
		{
			LogoutCSNtf* proto = new(m_protocol_buffer) LogoutCSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LogoutCSNtf(*proto);
				proto->~LogoutCSNtf();
				return true;
			}
			else
			{
				proto->~LogoutCSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case HeadMsgGSNtf::protocol_id:
		{
			HeadMsgGSNtf* proto = new(m_protocol_buffer) HeadMsgGSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_HeadMsgGSNtf(*proto);
				proto->~HeadMsgGSNtf();
				return true;
			}
			else
			{
				proto->~HeadMsgGSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SetChannelIntervalNtf::protocol_id:
		{
			SetChannelIntervalNtf* proto = new(m_protocol_buffer) SetChannelIntervalNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SetChannelIntervalNtf(*proto);
				proto->~SetChannelIntervalNtf();
				return true;
			}
			else
			{
				proto->~SetChannelIntervalNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SetChannelLevelNtf::protocol_id:
		{
			SetChannelLevelNtf* proto = new(m_protocol_buffer) SetChannelLevelNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SetChannelLevelNtf(*proto);
				proto->~SetChannelLevelNtf();
				return true;
			}
			else
			{
				proto->~SetChannelLevelNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SyncPlayerAttrNtf::protocol_id:
		{
			SyncPlayerAttrNtf* proto = new(m_protocol_buffer) SyncPlayerAttrNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SyncPlayerAttrNtf(*proto);
				proto->~SyncPlayerAttrNtf();
				return true;
			}
			else
			{
				proto->~SyncPlayerAttrNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SyncPlayerNameNtf::protocol_id:
		{
			SyncPlayerNameNtf* proto = new(m_protocol_buffer) SyncPlayerNameNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SyncPlayerNameNtf(*proto);
				proto->~SyncPlayerNameNtf();
				return true;
			}
			else
			{
				proto->~SyncPlayerNameNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case WorldChannelMsgGSNtf::protocol_id:
		{
			WorldChannelMsgGSNtf* proto = new(m_protocol_buffer) WorldChannelMsgGSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_WorldChannelMsgGSNtf(*proto);
				proto->~WorldChannelMsgGSNtf();
				return true;
			}
			else
			{
				proto->~WorldChannelMsgGSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case WorldChannelMsgCSNtf::protocol_id:
		{
			WorldChannelMsgCSNtf* proto = new(m_protocol_buffer) WorldChannelMsgCSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_WorldChannelMsgCSNtf(*proto);
				proto->~WorldChannelMsgCSNtf();
				return true;
			}
			else
			{
				proto->~WorldChannelMsgCSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case NoticeMsgCSNtf::protocol_id:
		{
			NoticeMsgCSNtf* proto = new(m_protocol_buffer) NoticeMsgCSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_NoticeMsgCSNtf(*proto);
				proto->~NoticeMsgCSNtf();
				return true;
			}
			else
			{
				proto->~NoticeMsgCSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ChannelSwitchCSNtf::protocol_id:
		{
			ChannelSwitchCSNtf* proto = new(m_protocol_buffer) ChannelSwitchCSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ChannelSwitchCSNtf(*proto);
				proto->~ChannelSwitchCSNtf();
				return true;
			}
			else
			{
				proto->~ChannelSwitchCSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case EmojiDataCSNtf::protocol_id:
		{
			EmojiDataCSNtf* proto = new(m_protocol_buffer) EmojiDataCSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_EmojiDataCSNtf(*proto);
				proto->~EmojiDataCSNtf();
				return true;
			}
			else
			{
				proto->~EmojiDataCSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		default:
			return false;
		}

	}

	static inline unsigned short GetModuleID(void){ return 108; }

	static inline unsigned short GetProtocolNum(void){ return 17; }

	static const unsigned short module_id = 108;

	static const unsigned short protocol_num = 17;

	unsigned short ModuleId() override { return D::GetModuleID(); }
	unsigned short ProtocolNum() override { return D::GetProtocolNum(); }
	bool Handle(NetDeCode & net_data) override { return static_cast<D*>(this)->HandleProtocol(net_data); }
//===============以下协议回调函数需要使用者来实现===============
	void OnRecv_LoginCSNtf(LoginCSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SyncPlayerU32Ntf(SyncPlayerU32Ntf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SyncPlayerU64Ntf(SyncPlayerU64Ntf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SyncPlayerSNtf(SyncPlayerSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_GSCSChannelMsgNtf(GSCSChannelMsgNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SubscribeChannelNtf(SubscribeChannelNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LogoutCSNtf(LogoutCSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_HeadMsgGSNtf(HeadMsgGSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SetChannelIntervalNtf(SetChannelIntervalNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SetChannelLevelNtf(SetChannelLevelNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SyncPlayerAttrNtf(SyncPlayerAttrNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SyncPlayerNameNtf(SyncPlayerNameNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_WorldChannelMsgGSNtf(WorldChannelMsgGSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_WorldChannelMsgCSNtf(WorldChannelMsgCSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_NoticeMsgCSNtf(NoticeMsgCSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ChannelSwitchCSNtf(ChannelSwitchCSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_EmojiDataCSNtf(EmojiDataCSNtf& rstProtocol){ (void)(rstProtocol); };
private:
	 void* m_protocol_buffer;

};
