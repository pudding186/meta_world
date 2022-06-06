#pragma once

#include "net_data.hpp"

//===============包含的其他协议文件===============
#include "StructDef.hpp"
 //===============宏定义开始===============
//===============宏定义结束===============

//===============数据定义开始===============
struct CanLoginGSReq:TProtocol<CanLoginGSReq>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 1;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	unsigned long long        guid; //玩家GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

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

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		guid = 0;

	}
	CanLoginGSReq& operator= (const CanLoginGSReq& src)
	{
		uid = src.uid;

		sid = src.sid;

		guid = src.guid;

		return *this;
	}
	bool operator==(const CanLoginGSReq& src) const
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

		return true;
	}
	bool operator != (const CanLoginGSReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CanLoginGSReq& data, JsonHandler* parent = nullptr)
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
        CanLoginGSReq& m_data;
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

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

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
        
        CanLoginGSReq::Handler h(*this, nullptr);
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

struct CanLoginGSAck:TProtocol<CanLoginGSAck>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 2;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	unsigned long long        guid; //玩家GUID=0代表该帐号下所有角色都能进入 GUID!=0代表仅该GUID代表的角色能进入
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

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

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
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

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		guid = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	CanLoginGSAck& operator= (const CanLoginGSAck& src)
	{
		uid = src.uid;

		sid = src.sid;

		guid = src.guid;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const CanLoginGSAck& src) const
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

		if (errcode != src.errcode)
		{
			return false;
		}

		if (strncmp(errmsg, src.errmsg, sizeof(errmsg)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const CanLoginGSAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CanLoginGSAck& data, JsonHandler* parent = nullptr)
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
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.errcode, str, length);
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
                JsonDelString(m_data.errmsg, str, length);
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
        CanLoginGSAck& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<5> m_assigned_bitset;

        static const std::bitset<5> template_assigned_bitset;
        static const char* m_key_list[5];
        static const char* m_alias_list[5];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"errcode", errcode);

		json_data.AddString(u8"errmsg", errmsg, sizeof(errmsg));

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
        
        CanLoginGSAck::Handler h(*this, nullptr);
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

struct LoginGSReq:TProtocol<LoginGSReq>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 3;

	unsigned int              gate; //网关索引
	PlayerData                playerdata; //玩家数据
	char                      longitude[64]; //经度(不存档)
	char                      latitude[64]; //纬度(不存档)
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(gate);

		playerdata.EnCode(net_data);

		net_data.AddString(longitude, sizeof(longitude));

		net_data.AddString(latitude, sizeof(latitude));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(gate);

		playerdata.EnCode(net_data);

		net_data.AddString(longitude, sizeof(longitude));

		net_data.AddString(latitude, sizeof(latitude));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(gate))
			return false;

		if (!playerdata.DeCode(net_data))
			return false;

		if (!net_data.DelString(longitude, sizeof(longitude)))
			return false;

		if (!net_data.DelString(latitude, sizeof(latitude)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(gate))
			return false;

		if (!playerdata.DeCode(net_data))
			return false;

		if (!net_data.DelString(longitude, sizeof(longitude)))
			return false;

		if (!net_data.DelString(latitude, sizeof(latitude)))
			return false;

		return true;
	}
	void Reset(void)
	{
		gate = 0;

		playerdata.Reset();

		longitude[0] = 0;

		latitude[0] = 0;

	}
	LoginGSReq& operator= (const LoginGSReq& src)
	{
		gate = src.gate;

		playerdata = src.playerdata;

		{
			size_t str_len = strnlen(src.longitude, sizeof(longitude) - 1);
			memcpy(longitude, src.longitude, str_len + 1);
			longitude[sizeof(longitude) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.latitude, sizeof(latitude) - 1);
			memcpy(latitude, src.latitude, str_len + 1);
			latitude[sizeof(latitude) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoginGSReq& src) const
	{
		if (gate != src.gate)
		{
			return false;
		}

		if (playerdata != src.playerdata)
		{
			return false;
		}

		if (strncmp(longitude, src.longitude, sizeof(longitude)-1))
		{
			return false;
		}

		if (strncmp(latitude, src.latitude, sizeof(latitude)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoginGSReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoginGSReq& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_playerdata_handler(m_data.playerdata, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_playerdata_handler.ResetState();
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
                JsonDelIntegral(m_data.gate, str, length);
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
                JsonDelString(m_data.longitude, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.latitude, str, length);
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
            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_playerdata_handler;
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

            if (!m_playerdata_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoginGSReq& m_data;
        PlayerData::Handler m_playerdata_handler;
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

		json_data.AddIntegral(u8"gate", gate);

		json_data.Writer().Key(u8"playerdata", sizeof(u8"playerdata") - 1);
		playerdata.ToJson(json_data);

		json_data.AddString(u8"longitude", longitude, sizeof(longitude));

		json_data.AddString(u8"latitude", latitude, sizeof(latitude));

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
        
        LoginGSReq::Handler h(*this, nullptr);
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

struct LoginGSAck:TProtocol<LoginGSAck>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 4;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	unsigned long long        guid; //玩家GUID
	unsigned int              arrayid; //错误码
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(arrayid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(arrayid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

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

		if (!net_data.DelIntegral(arrayid))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
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

		if (!net_data.DelIntegral(arrayid))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		guid = 0;

		arrayid = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	LoginGSAck& operator= (const LoginGSAck& src)
	{
		uid = src.uid;

		sid = src.sid;

		guid = src.guid;

		arrayid = src.arrayid;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoginGSAck& src) const
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

		if (arrayid != src.arrayid)
		{
			return false;
		}

		if (errcode != src.errcode)
		{
			return false;
		}

		if (strncmp(errmsg, src.errmsg, sizeof(errmsg)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoginGSAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoginGSAck& data, JsonHandler* parent = nullptr)
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
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.arrayid, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.errcode, str, length);
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
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.errmsg, str, length);
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
        LoginGSAck& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<6> m_assigned_bitset;

        static const std::bitset<6> template_assigned_bitset;
        static const char* m_key_list[6];
        static const char* m_alias_list[6];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"arrayid", arrayid);

		json_data.AddIntegral(u8"errcode", errcode);

		json_data.AddString(u8"errmsg", errmsg, sizeof(errmsg));

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
        
        LoginGSAck::Handler h(*this, nullptr);
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

struct ReloginGSNtf:TProtocol<ReloginGSNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 5;

	unsigned int              gate; //网关索引
	unsigned long long        guid; //玩家GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(gate);

		net_data.AddIntegral(guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(gate);

		net_data.AddIntegral(guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(gate))
			return false;

		if (!net_data.DelIntegral(guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(gate))
			return false;

		if (!net_data.DelIntegral(guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		gate = 0;

		guid = 0;

	}
	ReloginGSNtf& operator= (const ReloginGSNtf& src)
	{
		gate = src.gate;

		guid = src.guid;

		return *this;
	}
	bool operator==(const ReloginGSNtf& src) const
	{
		if (gate != src.gate)
		{
			return false;
		}

		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ReloginGSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ReloginGSNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.gate, str, length);
            }
            break;
            case 1:
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
        ReloginGSNtf& m_data;
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

		json_data.AddIntegral(u8"gate", gate);

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
        
        ReloginGSNtf::Handler h(*this, nullptr);
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

struct LogoutGSReq:TProtocol<LogoutGSReq>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 6;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	unsigned char             type; //1:回到选角界面 2:退出游戏 3:重复登陆踢回选角界面 4:断线踢出游戏
	unsigned long long        guid; //玩家GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(type);

		net_data.AddIntegral(guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(type);

		net_data.AddIntegral(guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		type = 0;

		guid = 0;

	}
	LogoutGSReq& operator= (const LogoutGSReq& src)
	{
		uid = src.uid;

		sid = src.sid;

		type = src.type;

		guid = src.guid;

		return *this;
	}
	bool operator==(const LogoutGSReq& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
		{
			return false;
		}

		if (type != src.type)
		{
			return false;
		}

		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LogoutGSReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LogoutGSReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.type, str, length);
            }
            break;
            case 3:
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
        LogoutGSReq& m_data;
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

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"type", type);

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
        
        LogoutGSReq::Handler h(*this, nullptr);
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

struct LogoutGSAck:TProtocol<LogoutGSAck>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 7;

	PlayerData                playerdata; //玩家数据
	unsigned char             type; //1:回到选角界面 2:退出游戏 3:重复登陆踢回选角界面 4:断线踢出游戏
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		playerdata.EnCode(net_data);

		net_data.AddIntegral(type);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		playerdata.EnCode(net_data);

		net_data.AddIntegral(type);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!playerdata.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!playerdata.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		playerdata.Reset();

		type = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	LogoutGSAck& operator= (const LogoutGSAck& src)
	{
		playerdata = src.playerdata;

		type = src.type;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LogoutGSAck& src) const
	{
		if (playerdata != src.playerdata)
		{
			return false;
		}

		if (type != src.type)
		{
			return false;
		}

		if (errcode != src.errcode)
		{
			return false;
		}

		if (strncmp(errmsg, src.errmsg, sizeof(errmsg)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const LogoutGSAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LogoutGSAck& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_playerdata_handler(m_data.playerdata, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_playerdata_handler.ResetState();
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
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.type, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.errcode, str, length);
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
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.errmsg, str, length);
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
            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                return &m_playerdata_handler;
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

            if (!m_playerdata_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LogoutGSAck& m_data;
        PlayerData::Handler m_playerdata_handler;
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

		json_data.Writer().Key(u8"playerdata", sizeof(u8"playerdata") - 1);
		playerdata.ToJson(json_data);

		json_data.AddIntegral(u8"type", type);

		json_data.AddIntegral(u8"errcode", errcode);

		json_data.AddString(u8"errmsg", errmsg, sizeof(errmsg));

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
        
        LogoutGSAck::Handler h(*this, nullptr);
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

struct OfflineNtf:TProtocol<OfflineNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 8;

	unsigned long long        guid; //玩家guid
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
	OfflineNtf& operator= (const OfflineNtf& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const OfflineNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const OfflineNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(OfflineNtf& data, JsonHandler* parent = nullptr)
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
        OfflineNtf& m_data;
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
        
        OfflineNtf::Handler h(*this, nullptr);
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

struct WrapperPkg:TProtocol<WrapperPkg>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 9;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	DataArray<unsigned char, unsigned int> wrapper; //包装数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddArray(wrapper);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddArray(wrapper);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if(!net_data.DelArray(wrapper))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if(!net_data.DelArray(wrapper))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		wrapper.clear();

	}
	WrapperPkg& operator= (const WrapperPkg& src)
	{
		uid = src.uid;

		sid = src.sid;

		wrapper = src.wrapper;

		return *this;
	}
	bool operator==(const WrapperPkg& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
		{
			return false;
		}

		if (wrapper != src.wrapper)
		{
			return false;
		}

		return true;
	}
	bool operator != (const WrapperPkg& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(WrapperPkg& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.uid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.sid, str, length);
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
                DataArrayHandler<unsigned char, unsigned int> handler(m_data.wrapper, this);
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
        WrapperPkg& m_data;
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

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.Writer().Key(u8"wrapper", sizeof(u8"wrapper") - 1);
		json_data.AddArray(wrapper);

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
        
        WrapperPkg::Handler h(*this, nullptr);
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

struct SavePlayerNtf:TProtocol<SavePlayerNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 10;

	PlayerData                playerdata; //玩家详细
	bool EnCode(NetEnCode& net_data)
	{
		playerdata.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		playerdata.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!playerdata.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!playerdata.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		playerdata.Reset();

	}
	SavePlayerNtf& operator= (const SavePlayerNtf& src)
	{
		playerdata = src.playerdata;

		return *this;
	}
	bool operator==(const SavePlayerNtf& src) const
	{
		if (playerdata != src.playerdata)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SavePlayerNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SavePlayerNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_playerdata_handler(m_data.playerdata, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_playerdata_handler.ResetState();
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
                return &m_playerdata_handler;
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

            if (!m_playerdata_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        SavePlayerNtf& m_data;
        PlayerData::Handler m_playerdata_handler;
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

		json_data.Writer().Key(u8"playerdata", sizeof(u8"playerdata") - 1);
		playerdata.ToJson(json_data);

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
        
        SavePlayerNtf::Handler h(*this, nullptr);
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

struct BillInGSNtf:TProtocol<BillInGSNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 11;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	unsigned long long        guid; //玩家GUID
	unsigned int              bill_id; //订单ID
	signed long long          billin_yb; //用户充值元宝数
	char                      desc[256]; //充值描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(bill_id);

		net_data.AddIntegral(billin_yb);

		net_data.AddString(desc, sizeof(desc));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(bill_id);

		net_data.AddIntegral(billin_yb);

		net_data.AddString(desc, sizeof(desc));

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

		if (!net_data.DelIntegral(bill_id))
			return false;

		if (!net_data.DelIntegral(billin_yb))
			return false;

		if (!net_data.DelString(desc, sizeof(desc)))
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

		if (!net_data.DelIntegral(bill_id))
			return false;

		if (!net_data.DelIntegral(billin_yb))
			return false;

		if (!net_data.DelString(desc, sizeof(desc)))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		guid = 0;

		bill_id = 0;

		billin_yb = 0;

		desc[0] = 0;

	}
	BillInGSNtf& operator= (const BillInGSNtf& src)
	{
		uid = src.uid;

		sid = src.sid;

		guid = src.guid;

		bill_id = src.bill_id;

		billin_yb = src.billin_yb;

		{
			size_t str_len = strnlen(src.desc, sizeof(desc) - 1);
			memcpy(desc, src.desc, str_len + 1);
			desc[sizeof(desc) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const BillInGSNtf& src) const
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

		if (bill_id != src.bill_id)
		{
			return false;
		}

		if (billin_yb != src.billin_yb)
		{
			return false;
		}

		if (strncmp(desc, src.desc, sizeof(desc)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const BillInGSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(BillInGSNtf& data, JsonHandler* parent = nullptr)
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
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.bill_id, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.billin_yb, str, length);
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
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.desc, str, length);
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
        BillInGSNtf& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<6> m_assigned_bitset;

        static const std::bitset<6> template_assigned_bitset;
        static const char* m_key_list[6];
        static const char* m_alias_list[6];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"bill_id", bill_id);

		json_data.AddIntegral(u8"billin_yb", billin_yb);

		json_data.AddString(u8"desc", desc, sizeof(desc));

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
        
        BillInGSNtf::Handler h(*this, nullptr);
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

struct ForceKickNtf:TProtocol<ForceKickNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 12;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

	}
	ForceKickNtf& operator= (const ForceKickNtf& src)
	{
		uid = src.uid;

		sid = src.sid;

		return *this;
	}
	bool operator==(const ForceKickNtf& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ForceKickNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ForceKickNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.uid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.sid, str, length);
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
        ForceKickNtf& m_data;
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

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

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
        
        ForceKickNtf::Handler h(*this, nullptr);
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

struct GameInNtf:TProtocol<GameInNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 13;

	unsigned int              uid; //账号ID
	unsigned char             sid; //子区ID
	signed long long          gameinyb; //累计游戏内产出元宝
	signed long long          gameoutyb; //累计游戏内消耗元宝
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(gameinyb);

		net_data.AddIntegral(gameoutyb);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(gameinyb);

		net_data.AddIntegral(gameoutyb);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelIntegral(gameinyb))
			return false;

		if (!net_data.DelIntegral(gameoutyb))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelIntegral(gameinyb))
			return false;

		if (!net_data.DelIntegral(gameoutyb))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		gameinyb = 0;

		gameoutyb = 0;

	}
	GameInNtf& operator= (const GameInNtf& src)
	{
		uid = src.uid;

		sid = src.sid;

		gameinyb = src.gameinyb;

		gameoutyb = src.gameoutyb;

		return *this;
	}
	bool operator==(const GameInNtf& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
		{
			return false;
		}

		if (gameinyb != src.gameinyb)
		{
			return false;
		}

		if (gameoutyb != src.gameoutyb)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GameInNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GameInNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.gameinyb, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gameoutyb, str, length);
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
        GameInNtf& m_data;
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

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"gameinyb", gameinyb);

		json_data.AddIntegral(u8"gameoutyb", gameoutyb);

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
        
        GameInNtf::Handler h(*this, nullptr);
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

struct ForbidNtf:TProtocol<ForbidNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 14;

	unsigned char             forbid; //0:解禁 1:禁
	ForbidData                data; //封禁数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(forbid);

		data.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(forbid);

		data.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(forbid))
			return false;

		if (!data.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(forbid))
			return false;

		if (!data.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		forbid = 0;

		data.Reset();

	}
	ForbidNtf& operator= (const ForbidNtf& src)
	{
		forbid = src.forbid;

		data = src.data;

		return *this;
	}
	bool operator==(const ForbidNtf& src) const
	{
		if (forbid != src.forbid)
		{
			return false;
		}

		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ForbidNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ForbidNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_data_handler(m_data.data, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_data_handler.ResetState();
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
                JsonDelIntegral(m_data.forbid, str, length);
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
                return &m_data_handler;
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

            if (!m_data_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ForbidNtf& m_data;
        ForbidData::Handler m_data_handler;
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

		json_data.AddIntegral(u8"forbid", forbid);

		json_data.Writer().Key(u8"data", sizeof(u8"data") - 1);
		data.ToJson(json_data);

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
        
        ForbidNtf::Handler h(*this, nullptr);
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

struct SetCustomVarNtf:TProtocol<SetCustomVarNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 15;

	unsigned short            type; //类型
	unsigned short            scope; //广播范围
	char                      key[256]; //key值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(scope);

		net_data.AddString(key, sizeof(key));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(scope);

		net_data.AddString(key, sizeof(key));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(scope))
			return false;

		if (!net_data.DelString(key, sizeof(key)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(scope))
			return false;

		if (!net_data.DelString(key, sizeof(key)))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		scope = 0;

		key[0] = 0;

	}
	SetCustomVarNtf& operator= (const SetCustomVarNtf& src)
	{
		type = src.type;

		scope = src.scope;

		{
			size_t str_len = strnlen(src.key, sizeof(key) - 1);
			memcpy(key, src.key, str_len + 1);
			key[sizeof(key) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const SetCustomVarNtf& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (scope != src.scope)
		{
			return false;
		}

		if (strncmp(key, src.key, sizeof(key)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const SetCustomVarNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SetCustomVarNtf& data, JsonHandler* parent = nullptr)
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
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.scope, str, length);
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
                JsonDelString(m_data.key, str, length);
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
        SetCustomVarNtf& m_data;
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

		json_data.AddIntegral(u8"scope", scope);

		json_data.AddString(u8"key", key, sizeof(key));

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
        
        SetCustomVarNtf::Handler h(*this, nullptr);
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

struct NewContactInfoDataNtf:TProtocol<NewContactInfoDataNtf>
{
	static constexpr unsigned short module_id = 107;
	static constexpr unsigned short protocol_id = 16;

	ContactInfoData           data; //联系人信息
	bool EnCode(NetEnCode& net_data)
	{
		data.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		data.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!data.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!data.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		data.Reset();

	}
	NewContactInfoDataNtf& operator= (const NewContactInfoDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const NewContactInfoDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const NewContactInfoDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(NewContactInfoDataNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_data_handler(m_data.data, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_data_handler.ResetState();
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
                return &m_data_handler;
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

            if (!m_data_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        NewContactInfoDataNtf& m_data;
        ContactInfoData::Handler m_data_handler;
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

		json_data.Writer().Key(u8"data", sizeof(u8"data") - 1);
		data.ToJson(json_data);

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
        
        NewContactInfoDataNtf::Handler h(*this, nullptr);
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
class CWSGS: public ProtocolModule
{
public:
	CWSGS()
	{
		size_t max_protocol_size = 0;
		if (sizeof(CanLoginGSReq) > max_protocol_size)
			max_protocol_size = sizeof(CanLoginGSReq);

		if (sizeof(CanLoginGSAck) > max_protocol_size)
			max_protocol_size = sizeof(CanLoginGSAck);

		if (sizeof(LoginGSReq) > max_protocol_size)
			max_protocol_size = sizeof(LoginGSReq);

		if (sizeof(LoginGSAck) > max_protocol_size)
			max_protocol_size = sizeof(LoginGSAck);

		if (sizeof(ReloginGSNtf) > max_protocol_size)
			max_protocol_size = sizeof(ReloginGSNtf);

		if (sizeof(LogoutGSReq) > max_protocol_size)
			max_protocol_size = sizeof(LogoutGSReq);

		if (sizeof(LogoutGSAck) > max_protocol_size)
			max_protocol_size = sizeof(LogoutGSAck);

		if (sizeof(OfflineNtf) > max_protocol_size)
			max_protocol_size = sizeof(OfflineNtf);

		if (sizeof(WrapperPkg) > max_protocol_size)
			max_protocol_size = sizeof(WrapperPkg);

		if (sizeof(SavePlayerNtf) > max_protocol_size)
			max_protocol_size = sizeof(SavePlayerNtf);

		if (sizeof(BillInGSNtf) > max_protocol_size)
			max_protocol_size = sizeof(BillInGSNtf);

		if (sizeof(ForceKickNtf) > max_protocol_size)
			max_protocol_size = sizeof(ForceKickNtf);

		if (sizeof(GameInNtf) > max_protocol_size)
			max_protocol_size = sizeof(GameInNtf);

		if (sizeof(ForbidNtf) > max_protocol_size)
			max_protocol_size = sizeof(ForbidNtf);

		if (sizeof(SetCustomVarNtf) > max_protocol_size)
			max_protocol_size = sizeof(SetCustomVarNtf);

		if (sizeof(NewContactInfoDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(NewContactInfoDataNtf);

		m_protocol_buffer = S_MALLOC(max_protocol_size);
	}

	virtual ~CWSGS()
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
		case CanLoginGSReq::protocol_id:
		{
			return CanLoginGSReq::Name();
		}
		break;
		case CanLoginGSAck::protocol_id:
		{
			return CanLoginGSAck::Name();
		}
		break;
		case LoginGSReq::protocol_id:
		{
			return LoginGSReq::Name();
		}
		break;
		case LoginGSAck::protocol_id:
		{
			return LoginGSAck::Name();
		}
		break;
		case ReloginGSNtf::protocol_id:
		{
			return ReloginGSNtf::Name();
		}
		break;
		case LogoutGSReq::protocol_id:
		{
			return LogoutGSReq::Name();
		}
		break;
		case LogoutGSAck::protocol_id:
		{
			return LogoutGSAck::Name();
		}
		break;
		case OfflineNtf::protocol_id:
		{
			return OfflineNtf::Name();
		}
		break;
		case WrapperPkg::protocol_id:
		{
			return WrapperPkg::Name();
		}
		break;
		case SavePlayerNtf::protocol_id:
		{
			return SavePlayerNtf::Name();
		}
		break;
		case BillInGSNtf::protocol_id:
		{
			return BillInGSNtf::Name();
		}
		break;
		case ForceKickNtf::protocol_id:
		{
			return ForceKickNtf::Name();
		}
		break;
		case GameInNtf::protocol_id:
		{
			return GameInNtf::Name();
		}
		break;
		case ForbidNtf::protocol_id:
		{
			return ForbidNtf::Name();
		}
		break;
		case SetCustomVarNtf::protocol_id:
		{
			return SetCustomVarNtf::Name();
		}
		break;
		case NewContactInfoDataNtf::protocol_id:
		{
			return NewContactInfoDataNtf::Name();
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

		if (m_id != 107)
		{
			net_data.Reset(net_data_pos);
			return false;
		}

		switch(p_id)
		{
		case CanLoginGSReq::protocol_id:
		{
			CanLoginGSReq* proto = new(m_protocol_buffer) CanLoginGSReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_CanLoginGSReq(*proto);
				proto->~CanLoginGSReq();
				return true;
			}
			else
			{
				proto->~CanLoginGSReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case CanLoginGSAck::protocol_id:
		{
			CanLoginGSAck* proto = new(m_protocol_buffer) CanLoginGSAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_CanLoginGSAck(*proto);
				proto->~CanLoginGSAck();
				return true;
			}
			else
			{
				proto->~CanLoginGSAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoginGSReq::protocol_id:
		{
			LoginGSReq* proto = new(m_protocol_buffer) LoginGSReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoginGSReq(*proto);
				proto->~LoginGSReq();
				return true;
			}
			else
			{
				proto->~LoginGSReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoginGSAck::protocol_id:
		{
			LoginGSAck* proto = new(m_protocol_buffer) LoginGSAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoginGSAck(*proto);
				proto->~LoginGSAck();
				return true;
			}
			else
			{
				proto->~LoginGSAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ReloginGSNtf::protocol_id:
		{
			ReloginGSNtf* proto = new(m_protocol_buffer) ReloginGSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ReloginGSNtf(*proto);
				proto->~ReloginGSNtf();
				return true;
			}
			else
			{
				proto->~ReloginGSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LogoutGSReq::protocol_id:
		{
			LogoutGSReq* proto = new(m_protocol_buffer) LogoutGSReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LogoutGSReq(*proto);
				proto->~LogoutGSReq();
				return true;
			}
			else
			{
				proto->~LogoutGSReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LogoutGSAck::protocol_id:
		{
			LogoutGSAck* proto = new(m_protocol_buffer) LogoutGSAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LogoutGSAck(*proto);
				proto->~LogoutGSAck();
				return true;
			}
			else
			{
				proto->~LogoutGSAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case OfflineNtf::protocol_id:
		{
			OfflineNtf* proto = new(m_protocol_buffer) OfflineNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_OfflineNtf(*proto);
				proto->~OfflineNtf();
				return true;
			}
			else
			{
				proto->~OfflineNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case WrapperPkg::protocol_id:
		{
			WrapperPkg* proto = new(m_protocol_buffer) WrapperPkg();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_WrapperPkg(*proto);
				proto->~WrapperPkg();
				return true;
			}
			else
			{
				proto->~WrapperPkg();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SavePlayerNtf::protocol_id:
		{
			SavePlayerNtf* proto = new(m_protocol_buffer) SavePlayerNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SavePlayerNtf(*proto);
				proto->~SavePlayerNtf();
				return true;
			}
			else
			{
				proto->~SavePlayerNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case BillInGSNtf::protocol_id:
		{
			BillInGSNtf* proto = new(m_protocol_buffer) BillInGSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_BillInGSNtf(*proto);
				proto->~BillInGSNtf();
				return true;
			}
			else
			{
				proto->~BillInGSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ForceKickNtf::protocol_id:
		{
			ForceKickNtf* proto = new(m_protocol_buffer) ForceKickNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ForceKickNtf(*proto);
				proto->~ForceKickNtf();
				return true;
			}
			else
			{
				proto->~ForceKickNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case GameInNtf::protocol_id:
		{
			GameInNtf* proto = new(m_protocol_buffer) GameInNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_GameInNtf(*proto);
				proto->~GameInNtf();
				return true;
			}
			else
			{
				proto->~GameInNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ForbidNtf::protocol_id:
		{
			ForbidNtf* proto = new(m_protocol_buffer) ForbidNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ForbidNtf(*proto);
				proto->~ForbidNtf();
				return true;
			}
			else
			{
				proto->~ForbidNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SetCustomVarNtf::protocol_id:
		{
			SetCustomVarNtf* proto = new(m_protocol_buffer) SetCustomVarNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SetCustomVarNtf(*proto);
				proto->~SetCustomVarNtf();
				return true;
			}
			else
			{
				proto->~SetCustomVarNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case NewContactInfoDataNtf::protocol_id:
		{
			NewContactInfoDataNtf* proto = new(m_protocol_buffer) NewContactInfoDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_NewContactInfoDataNtf(*proto);
				proto->~NewContactInfoDataNtf();
				return true;
			}
			else
			{
				proto->~NewContactInfoDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		default:
			return false;
		}

	}

	static inline unsigned short GetModuleID(void){ return 107; }

	static inline unsigned short GetProtocolNum(void){ return 16; }

	static const unsigned short module_id = 107;

	static const unsigned short protocol_num = 16;

	unsigned short ModuleId() override { return D::GetModuleID(); }
	unsigned short ProtocolNum() override { return D::GetProtocolNum(); }
	bool Handle(NetDeCode & net_data) override { return static_cast<D*>(this)->HandleProtocol(net_data); }
//===============以下协议回调函数需要使用者来实现===============
	void OnRecv_CanLoginGSReq(CanLoginGSReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_CanLoginGSAck(CanLoginGSAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoginGSReq(LoginGSReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoginGSAck(LoginGSAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ReloginGSNtf(ReloginGSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LogoutGSReq(LogoutGSReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LogoutGSAck(LogoutGSAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_OfflineNtf(OfflineNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_WrapperPkg(WrapperPkg& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SavePlayerNtf(SavePlayerNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_BillInGSNtf(BillInGSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ForceKickNtf(ForceKickNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_GameInNtf(GameInNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ForbidNtf(ForbidNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SetCustomVarNtf(SetCustomVarNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_NewContactInfoDataNtf(NewContactInfoDataNtf& rstProtocol){ (void)(rstProtocol); };
private:
	 void* m_protocol_buffer;

};
