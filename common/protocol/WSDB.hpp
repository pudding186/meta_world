#pragma once

#include "net_data.hpp"

//===============包含的其他协议文件===============
#include "StructDef.hpp"
 //===============宏定义开始===============
//===============宏定义结束===============

//===============数据定义开始===============
struct LoadUserReq:TProtocol<LoadUserReq>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 1;

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
	LoadUserReq& operator= (const LoadUserReq& src)
	{
		uid = src.uid;

		sid = src.sid;

		return *this;
	}
	bool operator==(const LoadUserReq& src) const
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
	bool operator != (const LoadUserReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadUserReq& data, JsonHandler* parent = nullptr)
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
        LoadUserReq& m_data;
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
        
        LoadUserReq::Handler h(*this, nullptr);
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

struct LoadUserAck:TProtocol<LoadUserAck>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 2;

	UserData                  userdata; //账号数据
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		userdata.EnCode(net_data);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		userdata.EnCode(net_data);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!userdata.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!userdata.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		userdata.Reset();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadUserAck& operator= (const LoadUserAck& src)
	{
		userdata = src.userdata;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadUserAck& src) const
	{
		if (userdata != src.userdata)
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
	bool operator != (const LoadUserAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadUserAck& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_userdata_handler(m_data.userdata, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_userdata_handler.ResetState();
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
            case 2:
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
                return &m_userdata_handler;
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

            if (!m_userdata_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoadUserAck& m_data;
        UserData::Handler m_userdata_handler;
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

		json_data.Writer().Key(u8"userdata", sizeof(u8"userdata") - 1);
		userdata.ToJson(json_data);

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
        
        LoadUserAck::Handler h(*this, nullptr);
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

struct UpdateUserNtf:TProtocol<UpdateUserNtf>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 3;

	UserData                  userdata; //账号数据
	bool EnCode(NetEnCode& net_data)
	{
		userdata.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		userdata.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!userdata.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!userdata.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		userdata.Reset();

	}
	UpdateUserNtf& operator= (const UpdateUserNtf& src)
	{
		userdata = src.userdata;

		return *this;
	}
	bool operator==(const UpdateUserNtf& src) const
	{
		if (userdata != src.userdata)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateUserNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateUserNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_userdata_handler(m_data.userdata, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_userdata_handler.ResetState();
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
                return &m_userdata_handler;
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

            if (!m_userdata_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        UpdateUserNtf& m_data;
        UserData::Handler m_userdata_handler;
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

		json_data.Writer().Key(u8"userdata", sizeof(u8"userdata") - 1);
		userdata.ToJson(json_data);

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
        
        UpdateUserNtf::Handler h(*this, nullptr);
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

struct PlayerBriefsReq:TProtocol<PlayerBriefsReq>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 4;

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
	PlayerBriefsReq& operator= (const PlayerBriefsReq& src)
	{
		uid = src.uid;

		sid = src.sid;

		return *this;
	}
	bool operator==(const PlayerBriefsReq& src) const
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
	bool operator != (const PlayerBriefsReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PlayerBriefsReq& data, JsonHandler* parent = nullptr)
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
        PlayerBriefsReq& m_data;
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
        
        PlayerBriefsReq::Handler h(*this, nullptr);
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

struct PlayerBriefsAck:TProtocol<PlayerBriefsAck>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 5;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	DataArray<PlayerBrief, unsigned char> briefs; //玩家简略列表
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddArray(briefs);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddArray(briefs);

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

		if(!net_data.DelArray(briefs))
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

		if(!net_data.DelArray(briefs))
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

		briefs.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	PlayerBriefsAck& operator= (const PlayerBriefsAck& src)
	{
		uid = src.uid;

		sid = src.sid;

		briefs = src.briefs;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const PlayerBriefsAck& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
		{
			return false;
		}

		if (briefs != src.briefs)
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
	bool operator != (const PlayerBriefsAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PlayerBriefsAck& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_briefs_handler(m_data.briefs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_briefs_handler.ResetState();
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
            switch (m_state)
            {
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_briefs_handler;
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

            if (!m_briefs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        PlayerBriefsAck& m_data;
        DataArrayHandler<PlayerBrief, unsigned char, PlayerBrief::Handler> m_briefs_handler;
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

		json_data.Writer().Key(u8"briefs", sizeof(u8"briefs") - 1);
		json_data.AddArray(briefs);

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
        
        PlayerBriefsAck::Handler h(*this, nullptr);
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

struct NewPlayerReq:TProtocol<NewPlayerReq>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 6;

	PlayerBrief               playerbrief; //玩家信息
	bool EnCode(NetEnCode& net_data)
	{
		playerbrief.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		playerbrief.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!playerbrief.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!playerbrief.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		playerbrief.Reset();

	}
	NewPlayerReq& operator= (const NewPlayerReq& src)
	{
		playerbrief = src.playerbrief;

		return *this;
	}
	bool operator==(const NewPlayerReq& src) const
	{
		if (playerbrief != src.playerbrief)
		{
			return false;
		}

		return true;
	}
	bool operator != (const NewPlayerReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(NewPlayerReq& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_playerbrief_handler(m_data.playerbrief, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_playerbrief_handler.ResetState();
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
                return &m_playerbrief_handler;
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

            if (!m_playerbrief_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        NewPlayerReq& m_data;
        PlayerBrief::Handler m_playerbrief_handler;
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

		json_data.Writer().Key(u8"playerbrief", sizeof(u8"playerbrief") - 1);
		playerbrief.ToJson(json_data);

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
        
        NewPlayerReq::Handler h(*this, nullptr);
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

struct NewPlayerAck:TProtocol<NewPlayerAck>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 7;

	PlayerBrief               playerbrief; //玩家信息
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		playerbrief.EnCode(net_data);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		playerbrief.EnCode(net_data);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!playerbrief.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!playerbrief.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		playerbrief.Reset();

		errcode = 0;

		errmsg[0] = 0;

	}
	NewPlayerAck& operator= (const NewPlayerAck& src)
	{
		playerbrief = src.playerbrief;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const NewPlayerAck& src) const
	{
		if (playerbrief != src.playerbrief)
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
	bool operator != (const NewPlayerAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(NewPlayerAck& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_playerbrief_handler(m_data.playerbrief, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_playerbrief_handler.ResetState();
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
            case 2:
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
                return &m_playerbrief_handler;
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

            if (!m_playerbrief_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        NewPlayerAck& m_data;
        PlayerBrief::Handler m_playerbrief_handler;
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

		json_data.Writer().Key(u8"playerbrief", sizeof(u8"playerbrief") - 1);
		playerbrief.ToJson(json_data);

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
        
        NewPlayerAck::Handler h(*this, nullptr);
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

struct UpdateBriefNtf:TProtocol<UpdateBriefNtf>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 8;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	PlayerBrief               brief; //玩家简介
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		brief.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		brief.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!brief.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!brief.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		brief.Reset();

	}
	UpdateBriefNtf& operator= (const UpdateBriefNtf& src)
	{
		uid = src.uid;

		sid = src.sid;

		brief = src.brief;

		return *this;
	}
	bool operator==(const UpdateBriefNtf& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
		{
			return false;
		}

		if (brief != src.brief)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateBriefNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateBriefNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_brief_handler(m_data.brief, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_brief_handler.ResetState();
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
            switch (m_state)
            {
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_brief_handler;
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

            if (!m_brief_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        UpdateBriefNtf& m_data;
        PlayerBrief::Handler m_brief_handler;
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

		json_data.Writer().Key(u8"brief", sizeof(u8"brief") - 1);
		brief.ToJson(json_data);

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
        
        UpdateBriefNtf::Handler h(*this, nullptr);
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

struct UpdatePlayerNtf:TProtocol<UpdatePlayerNtf>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 9;

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
	UpdatePlayerNtf& operator= (const UpdatePlayerNtf& src)
	{
		playerdata = src.playerdata;

		return *this;
	}
	bool operator==(const UpdatePlayerNtf& src) const
	{
		if (playerdata != src.playerdata)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdatePlayerNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdatePlayerNtf& data, JsonHandler* parent = nullptr)
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
        UpdatePlayerNtf& m_data;
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
        
        UpdatePlayerNtf::Handler h(*this, nullptr);
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

struct LoadPlayerReq:TProtocol<LoadPlayerReq>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 10;

	unsigned long long        guid; //玩家guid
	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		uid = 0;

		sid = 0;

	}
	LoadPlayerReq& operator= (const LoadPlayerReq& src)
	{
		guid = src.guid;

		uid = src.uid;

		sid = src.sid;

		return *this;
	}
	bool operator==(const LoadPlayerReq& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

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
	bool operator != (const LoadPlayerReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadPlayerReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.uid, str, length);
            }
            break;
            case 2:
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
        LoadPlayerReq& m_data;
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
        
        LoadPlayerReq::Handler h(*this, nullptr);
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

struct LoadPlayerAck:TProtocol<LoadPlayerAck>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 11;

	PlayerData                playerdata; //玩家数据
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		playerdata.EnCode(net_data);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		playerdata.EnCode(net_data);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!playerdata.DeCode(net_data))
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

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		playerdata.Reset();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadPlayerAck& operator= (const LoadPlayerAck& src)
	{
		playerdata = src.playerdata;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadPlayerAck& src) const
	{
		if (playerdata != src.playerdata)
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
	bool operator != (const LoadPlayerAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadPlayerAck& data, JsonHandler* parent = nullptr)
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
            case 2:
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
        LoadPlayerAck& m_data;
        PlayerData::Handler m_playerdata_handler;
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

		json_data.Writer().Key(u8"playerdata", sizeof(u8"playerdata") - 1);
		playerdata.ToJson(json_data);

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
        
        LoadPlayerAck::Handler h(*this, nullptr);
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

struct BillInNtf:TProtocol<BillInNtf>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 12;

	unsigned int              uid; //账号id
	unsigned char             sid; //子区id
	unsigned int              bill_id; //订单ID
	signed long long          billin_yb; //用户充值元宝数
	char                      desc[256]; //充值描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(bill_id);

		net_data.AddIntegral(billin_yb);

		net_data.AddString(desc, sizeof(desc));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

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

		bill_id = 0;

		billin_yb = 0;

		desc[0] = 0;

	}
	BillInNtf& operator= (const BillInNtf& src)
	{
		uid = src.uid;

		sid = src.sid;

		bill_id = src.bill_id;

		billin_yb = src.billin_yb;

		{
			size_t str_len = strnlen(src.desc, sizeof(desc) - 1);
			memcpy(desc, src.desc, str_len + 1);
			desc[sizeof(desc) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const BillInNtf& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
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
	bool operator != (const BillInNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(BillInNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.bill_id, str, length);
            }
            break;
            case 3:
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
            case 4:
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
        BillInNtf& m_data;
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
        
        BillInNtf::Handler h(*this, nullptr);
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

struct LoadForbidReq:TProtocol<LoadForbidReq>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 13;

	bool EnCode(NetEnCode& net_data)
	{
		(void)(net_data);
		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		(void)(net_data);
		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		(void)(net_data);
		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		(void)(net_data);
		return true;
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadForbidReq& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
        }

        bool Key(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)str;
            (void)length;
            (void)copy;

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
            return true;
        }
    private:
        LoadForbidReq& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

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
        
        LoadForbidReq::Handler h(*this, nullptr);
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

struct LoadForbidAck:TProtocol<LoadForbidAck>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 14;

	DataArray<ForbidData, unsigned int> forbids; //禁止数据
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(forbids);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddArray(forbids);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(forbids))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if(!net_data.DelArray(forbids))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		forbids.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadForbidAck& operator= (const LoadForbidAck& src)
	{
		forbids = src.forbids;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadForbidAck& src) const
	{
		if (forbids != src.forbids)
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
	bool operator != (const LoadForbidAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadForbidAck& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_forbids_handler(m_data.forbids, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_forbids_handler.ResetState();
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
            case 2:
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
            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                return &m_forbids_handler;
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

            if (!m_forbids_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoadForbidAck& m_data;
        DataArrayHandler<ForbidData, unsigned int, ForbidData::Handler> m_forbids_handler;
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

		json_data.Writer().Key(u8"forbids", sizeof(u8"forbids") - 1);
		json_data.AddArray(forbids);

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
        
        LoadForbidAck::Handler h(*this, nullptr);
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

struct AddForbidNtf:TProtocol<AddForbidNtf>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 15;

	ForbidData                data; //禁止数据
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
	AddForbidNtf& operator= (const AddForbidNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddForbidNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddForbidNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddForbidNtf& data, JsonHandler* parent = nullptr)
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
        AddForbidNtf& m_data;
        ForbidData::Handler m_data_handler;
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
        
        AddForbidNtf::Handler h(*this, nullptr);
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

struct DelForbidNtf:TProtocol<DelForbidNtf>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 16;

	ForbidData                data; //禁止数据
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
	DelForbidNtf& operator= (const DelForbidNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const DelForbidNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelForbidNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelForbidNtf& data, JsonHandler* parent = nullptr)
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
        DelForbidNtf& m_data;
        ForbidData::Handler m_data_handler;
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
        
        DelForbidNtf::Handler h(*this, nullptr);
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

struct RefreshPlayerCacheNtf:TProtocol<RefreshPlayerCacheNtf>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 17;

	unsigned long long        guid; //玩家的GUID
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
	RefreshPlayerCacheNtf& operator= (const RefreshPlayerCacheNtf& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const RefreshPlayerCacheNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const RefreshPlayerCacheNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(RefreshPlayerCacheNtf& data, JsonHandler* parent = nullptr)
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
        RefreshPlayerCacheNtf& m_data;
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
        
        RefreshPlayerCacheNtf::Handler h(*this, nullptr);
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

struct LoadForbidTalkReq:TProtocol<LoadForbidTalkReq>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 18;

	bool EnCode(NetEnCode& net_data)
	{
		(void)(net_data);
		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		(void)(net_data);
		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		(void)(net_data);
		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		(void)(net_data);
		return true;
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadForbidTalkReq& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
        }

        bool Key(const char* str, rapidjson::SizeType length, bool copy) override
        {
            (void)str;
            (void)length;
            (void)copy;

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
            return true;
        }
    private:
        LoadForbidTalkReq& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

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
        
        LoadForbidTalkReq::Handler h(*this, nullptr);
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

struct LoadForbidTalkAck:TProtocol<LoadForbidTalkAck>
{
	static constexpr unsigned short module_id = 106;
	static constexpr unsigned short protocol_id = 19;

	DataArray<ForbidTalkData, unsigned int> forbids; //禁止聊天数据
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(forbids);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddArray(forbids);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(forbids))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if(!net_data.DelArray(forbids))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		forbids.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadForbidTalkAck& operator= (const LoadForbidTalkAck& src)
	{
		forbids = src.forbids;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadForbidTalkAck& src) const
	{
		if (forbids != src.forbids)
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
	bool operator != (const LoadForbidTalkAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadForbidTalkAck& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_forbids_handler(m_data.forbids, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_forbids_handler.ResetState();
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
            case 2:
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
            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                return &m_forbids_handler;
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

            if (!m_forbids_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoadForbidTalkAck& m_data;
        DataArrayHandler<ForbidTalkData, unsigned int, ForbidTalkData::Handler> m_forbids_handler;
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

		json_data.Writer().Key(u8"forbids", sizeof(u8"forbids") - 1);
		json_data.AddArray(forbids);

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
        
        LoadForbidTalkAck::Handler h(*this, nullptr);
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
class CWSDB: public ProtocolModule
{
public:
	CWSDB()
	{
		size_t max_protocol_size = 0;
		if (sizeof(LoadUserReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadUserReq);

		if (sizeof(LoadUserAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadUserAck);

		if (sizeof(UpdateUserNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateUserNtf);

		if (sizeof(PlayerBriefsReq) > max_protocol_size)
			max_protocol_size = sizeof(PlayerBriefsReq);

		if (sizeof(PlayerBriefsAck) > max_protocol_size)
			max_protocol_size = sizeof(PlayerBriefsAck);

		if (sizeof(NewPlayerReq) > max_protocol_size)
			max_protocol_size = sizeof(NewPlayerReq);

		if (sizeof(NewPlayerAck) > max_protocol_size)
			max_protocol_size = sizeof(NewPlayerAck);

		if (sizeof(UpdateBriefNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateBriefNtf);

		if (sizeof(UpdatePlayerNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdatePlayerNtf);

		if (sizeof(LoadPlayerReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadPlayerReq);

		if (sizeof(LoadPlayerAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadPlayerAck);

		if (sizeof(BillInNtf) > max_protocol_size)
			max_protocol_size = sizeof(BillInNtf);

		if (sizeof(LoadForbidReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadForbidReq);

		if (sizeof(LoadForbidAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadForbidAck);

		if (sizeof(AddForbidNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddForbidNtf);

		if (sizeof(DelForbidNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelForbidNtf);

		if (sizeof(RefreshPlayerCacheNtf) > max_protocol_size)
			max_protocol_size = sizeof(RefreshPlayerCacheNtf);

		if (sizeof(LoadForbidTalkReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadForbidTalkReq);

		if (sizeof(LoadForbidTalkAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadForbidTalkAck);

		m_protocol_buffer = S_MALLOC(max_protocol_size);
	}

	virtual ~CWSDB()
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
		case LoadUserReq::protocol_id:
		{
			return LoadUserReq::Name();
		}
		break;
		case LoadUserAck::protocol_id:
		{
			return LoadUserAck::Name();
		}
		break;
		case UpdateUserNtf::protocol_id:
		{
			return UpdateUserNtf::Name();
		}
		break;
		case PlayerBriefsReq::protocol_id:
		{
			return PlayerBriefsReq::Name();
		}
		break;
		case PlayerBriefsAck::protocol_id:
		{
			return PlayerBriefsAck::Name();
		}
		break;
		case NewPlayerReq::protocol_id:
		{
			return NewPlayerReq::Name();
		}
		break;
		case NewPlayerAck::protocol_id:
		{
			return NewPlayerAck::Name();
		}
		break;
		case UpdateBriefNtf::protocol_id:
		{
			return UpdateBriefNtf::Name();
		}
		break;
		case UpdatePlayerNtf::protocol_id:
		{
			return UpdatePlayerNtf::Name();
		}
		break;
		case LoadPlayerReq::protocol_id:
		{
			return LoadPlayerReq::Name();
		}
		break;
		case LoadPlayerAck::protocol_id:
		{
			return LoadPlayerAck::Name();
		}
		break;
		case BillInNtf::protocol_id:
		{
			return BillInNtf::Name();
		}
		break;
		case LoadForbidReq::protocol_id:
		{
			return LoadForbidReq::Name();
		}
		break;
		case LoadForbidAck::protocol_id:
		{
			return LoadForbidAck::Name();
		}
		break;
		case AddForbidNtf::protocol_id:
		{
			return AddForbidNtf::Name();
		}
		break;
		case DelForbidNtf::protocol_id:
		{
			return DelForbidNtf::Name();
		}
		break;
		case RefreshPlayerCacheNtf::protocol_id:
		{
			return RefreshPlayerCacheNtf::Name();
		}
		break;
		case LoadForbidTalkReq::protocol_id:
		{
			return LoadForbidTalkReq::Name();
		}
		break;
		case LoadForbidTalkAck::protocol_id:
		{
			return LoadForbidTalkAck::Name();
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

		if (m_id != 106)
		{
			net_data.Reset(net_data_pos);
			return false;
		}

		switch(p_id)
		{
		case LoadUserReq::protocol_id:
		{
			LoadUserReq* proto = new(m_protocol_buffer) LoadUserReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadUserReq(*proto);
				proto->~LoadUserReq();
				return true;
			}
			else
			{
				proto->~LoadUserReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadUserAck::protocol_id:
		{
			LoadUserAck* proto = new(m_protocol_buffer) LoadUserAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadUserAck(*proto);
				proto->~LoadUserAck();
				return true;
			}
			else
			{
				proto->~LoadUserAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateUserNtf::protocol_id:
		{
			UpdateUserNtf* proto = new(m_protocol_buffer) UpdateUserNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateUserNtf(*proto);
				proto->~UpdateUserNtf();
				return true;
			}
			else
			{
				proto->~UpdateUserNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case PlayerBriefsReq::protocol_id:
		{
			PlayerBriefsReq* proto = new(m_protocol_buffer) PlayerBriefsReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_PlayerBriefsReq(*proto);
				proto->~PlayerBriefsReq();
				return true;
			}
			else
			{
				proto->~PlayerBriefsReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case PlayerBriefsAck::protocol_id:
		{
			PlayerBriefsAck* proto = new(m_protocol_buffer) PlayerBriefsAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_PlayerBriefsAck(*proto);
				proto->~PlayerBriefsAck();
				return true;
			}
			else
			{
				proto->~PlayerBriefsAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case NewPlayerReq::protocol_id:
		{
			NewPlayerReq* proto = new(m_protocol_buffer) NewPlayerReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_NewPlayerReq(*proto);
				proto->~NewPlayerReq();
				return true;
			}
			else
			{
				proto->~NewPlayerReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case NewPlayerAck::protocol_id:
		{
			NewPlayerAck* proto = new(m_protocol_buffer) NewPlayerAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_NewPlayerAck(*proto);
				proto->~NewPlayerAck();
				return true;
			}
			else
			{
				proto->~NewPlayerAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateBriefNtf::protocol_id:
		{
			UpdateBriefNtf* proto = new(m_protocol_buffer) UpdateBriefNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateBriefNtf(*proto);
				proto->~UpdateBriefNtf();
				return true;
			}
			else
			{
				proto->~UpdateBriefNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdatePlayerNtf::protocol_id:
		{
			UpdatePlayerNtf* proto = new(m_protocol_buffer) UpdatePlayerNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdatePlayerNtf(*proto);
				proto->~UpdatePlayerNtf();
				return true;
			}
			else
			{
				proto->~UpdatePlayerNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadPlayerReq::protocol_id:
		{
			LoadPlayerReq* proto = new(m_protocol_buffer) LoadPlayerReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadPlayerReq(*proto);
				proto->~LoadPlayerReq();
				return true;
			}
			else
			{
				proto->~LoadPlayerReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadPlayerAck::protocol_id:
		{
			LoadPlayerAck* proto = new(m_protocol_buffer) LoadPlayerAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadPlayerAck(*proto);
				proto->~LoadPlayerAck();
				return true;
			}
			else
			{
				proto->~LoadPlayerAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case BillInNtf::protocol_id:
		{
			BillInNtf* proto = new(m_protocol_buffer) BillInNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_BillInNtf(*proto);
				proto->~BillInNtf();
				return true;
			}
			else
			{
				proto->~BillInNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadForbidReq::protocol_id:
		{
			LoadForbidReq* proto = new(m_protocol_buffer) LoadForbidReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadForbidReq(*proto);
				proto->~LoadForbidReq();
				return true;
			}
			else
			{
				proto->~LoadForbidReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadForbidAck::protocol_id:
		{
			LoadForbidAck* proto = new(m_protocol_buffer) LoadForbidAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadForbidAck(*proto);
				proto->~LoadForbidAck();
				return true;
			}
			else
			{
				proto->~LoadForbidAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddForbidNtf::protocol_id:
		{
			AddForbidNtf* proto = new(m_protocol_buffer) AddForbidNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddForbidNtf(*proto);
				proto->~AddForbidNtf();
				return true;
			}
			else
			{
				proto->~AddForbidNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelForbidNtf::protocol_id:
		{
			DelForbidNtf* proto = new(m_protocol_buffer) DelForbidNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelForbidNtf(*proto);
				proto->~DelForbidNtf();
				return true;
			}
			else
			{
				proto->~DelForbidNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case RefreshPlayerCacheNtf::protocol_id:
		{
			RefreshPlayerCacheNtf* proto = new(m_protocol_buffer) RefreshPlayerCacheNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_RefreshPlayerCacheNtf(*proto);
				proto->~RefreshPlayerCacheNtf();
				return true;
			}
			else
			{
				proto->~RefreshPlayerCacheNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadForbidTalkReq::protocol_id:
		{
			LoadForbidTalkReq* proto = new(m_protocol_buffer) LoadForbidTalkReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadForbidTalkReq(*proto);
				proto->~LoadForbidTalkReq();
				return true;
			}
			else
			{
				proto->~LoadForbidTalkReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadForbidTalkAck::protocol_id:
		{
			LoadForbidTalkAck* proto = new(m_protocol_buffer) LoadForbidTalkAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadForbidTalkAck(*proto);
				proto->~LoadForbidTalkAck();
				return true;
			}
			else
			{
				proto->~LoadForbidTalkAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		default:
			return false;
		}

	}

	static inline unsigned short GetModuleID(void){ return 106; }

	static inline unsigned short GetProtocolNum(void){ return 19; }

	static const unsigned short module_id = 106;

	static const unsigned short protocol_num = 19;

	unsigned short ModuleId() override { return D::GetModuleID(); }
	unsigned short ProtocolNum() override { return D::GetProtocolNum(); }
	bool Handle(NetDeCode & net_data) override { return static_cast<D*>(this)->HandleProtocol(net_data); }
//===============以下协议回调函数需要使用者来实现===============
	void OnRecv_LoadUserReq(LoadUserReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadUserAck(LoadUserAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateUserNtf(UpdateUserNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_PlayerBriefsReq(PlayerBriefsReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_PlayerBriefsAck(PlayerBriefsAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_NewPlayerReq(NewPlayerReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_NewPlayerAck(NewPlayerAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateBriefNtf(UpdateBriefNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdatePlayerNtf(UpdatePlayerNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadPlayerReq(LoadPlayerReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadPlayerAck(LoadPlayerAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_BillInNtf(BillInNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadForbidReq(LoadForbidReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadForbidAck(LoadForbidAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddForbidNtf(AddForbidNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelForbidNtf(DelForbidNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_RefreshPlayerCacheNtf(RefreshPlayerCacheNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadForbidTalkReq(LoadForbidTalkReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadForbidTalkAck(LoadForbidTalkAck& rstProtocol){ (void)(rstProtocol); };
private:
	 void* m_protocol_buffer;

};
