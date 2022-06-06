#pragma once

#include "net_data.hpp"

//===============包含的其他协议文件===============
#include "StructDef.hpp"
 //===============宏定义开始===============
//===============宏定义结束===============

//===============数据定义开始===============
struct RandKeyNtf:TProtocol<RandKeyNtf>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 1;

	DataArray<unsigned char, unsigned char> code_content; //随机码
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(code_content);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddArray(code_content);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(code_content))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if(!net_data.DelArray(code_content))
			return false;

		return true;
	}
	void Reset(void)
	{
		code_content.clear();

	}
	RandKeyNtf& operator= (const RandKeyNtf& src)
	{
		code_content = src.code_content;

		return *this;
	}
	bool operator==(const RandKeyNtf& src) const
	{
		if (code_content != src.code_content)
		{
			return false;
		}

		return true;
	}
	bool operator != (const RandKeyNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(RandKeyNtf& data, JsonHandler* parent = nullptr)
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
                DataArrayHandler<unsigned char, unsigned char> handler(m_data.code_content, this);
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
        RandKeyNtf& m_data;
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

		json_data.Writer().Key(u8"code_content", sizeof(u8"code_content") - 1);
		json_data.AddArray(code_content);

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
        
        RandKeyNtf::Handler h(*this, nullptr);
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

struct LoginReq:TProtocol<LoginReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 2;

	unsigned char             sid; //子区ID
	char                      username[256]; //账号名
	unsigned int              ip; //登陆ip地址
	DataArray<unsigned char, unsigned char> pwd_content; //密码密文
	char                      hwid[256]; //硬件码
	unsigned char             client_type; //客户端类型
	char                      client_version[256]; //客户端版本号
	char                      client_protocol_md5[256]; //客户端协议MD5
	char                      longitude[64]; //经度
	char                      latitude[64]; //纬度
	unsigned long long        session; //会话ID(由网关填写)
	unsigned long long        suid; //会话ID(由网关填写)
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(sid);

		net_data.AddString(username, sizeof(username));

		net_data.AddIntegral(ip);

		net_data.AddArray(pwd_content);

		net_data.AddString(hwid, sizeof(hwid));

		net_data.AddIntegral(client_type);

		net_data.AddString(client_version, sizeof(client_version));

		net_data.AddString(client_protocol_md5, sizeof(client_protocol_md5));

		net_data.AddString(longitude, sizeof(longitude));

		net_data.AddString(latitude, sizeof(latitude));

		net_data.AddIntegral(session);

		net_data.AddIntegral(suid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(sid);

		net_data.AddString(username, sizeof(username));

		net_data.AddIntegral(ip);

		net_data.AddArray(pwd_content);

		net_data.AddString(hwid, sizeof(hwid));

		net_data.AddIntegral(client_type);

		net_data.AddString(client_version, sizeof(client_version));

		net_data.AddString(client_protocol_md5, sizeof(client_protocol_md5));

		net_data.AddString(longitude, sizeof(longitude));

		net_data.AddString(latitude, sizeof(latitude));

		net_data.AddIntegral(session);

		net_data.AddIntegral(suid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelString(username, sizeof(username)))
			return false;

		if (!net_data.DelIntegral(ip))
			return false;

		if(!net_data.DelArray(pwd_content))
			return false;

		if (!net_data.DelString(hwid, sizeof(hwid)))
			return false;

		if (!net_data.DelIntegral(client_type))
			return false;

		if (!net_data.DelString(client_version, sizeof(client_version)))
			return false;

		if (!net_data.DelString(client_protocol_md5, sizeof(client_protocol_md5)))
			return false;

		if (!net_data.DelString(longitude, sizeof(longitude)))
			return false;

		if (!net_data.DelString(latitude, sizeof(latitude)))
			return false;

		if (!net_data.DelIntegral(session))
			return false;

		if (!net_data.DelIntegral(suid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelString(username, sizeof(username)))
			return false;

		if (!net_data.DelIntegral(ip))
			return false;

		if(!net_data.DelArray(pwd_content))
			return false;

		if (!net_data.DelString(hwid, sizeof(hwid)))
			return false;

		if (!net_data.DelIntegral(client_type))
			return false;

		if (!net_data.DelString(client_version, sizeof(client_version)))
			return false;

		if (!net_data.DelString(client_protocol_md5, sizeof(client_protocol_md5)))
			return false;

		if (!net_data.DelString(longitude, sizeof(longitude)))
			return false;

		if (!net_data.DelString(latitude, sizeof(latitude)))
			return false;

		if (!net_data.DelIntegral(session))
			return false;

		if (!net_data.DelIntegral(suid))
			return false;

		return true;
	}
	void Reset(void)
	{
		sid = 0;

		username[0] = 0;

		ip = 0;

		pwd_content.clear();

		hwid[0] = 0;

		client_type = 0;

		client_version[0] = 0;

		client_protocol_md5[0] = 0;

		longitude[0] = 0;

		latitude[0] = 0;

		session = 0;

		suid = 0;

	}
	LoginReq& operator= (const LoginReq& src)
	{
		sid = src.sid;

		{
			size_t str_len = strnlen(src.username, sizeof(username) - 1);
			memcpy(username, src.username, str_len + 1);
			username[sizeof(username) - 1] = 0;
		}

		ip = src.ip;

		pwd_content = src.pwd_content;

		{
			size_t str_len = strnlen(src.hwid, sizeof(hwid) - 1);
			memcpy(hwid, src.hwid, str_len + 1);
			hwid[sizeof(hwid) - 1] = 0;
		}

		client_type = src.client_type;

		{
			size_t str_len = strnlen(src.client_version, sizeof(client_version) - 1);
			memcpy(client_version, src.client_version, str_len + 1);
			client_version[sizeof(client_version) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.client_protocol_md5, sizeof(client_protocol_md5) - 1);
			memcpy(client_protocol_md5, src.client_protocol_md5, str_len + 1);
			client_protocol_md5[sizeof(client_protocol_md5) - 1] = 0;
		}

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

		session = src.session;

		suid = src.suid;

		return *this;
	}
	bool operator==(const LoginReq& src) const
	{
		if (sid != src.sid)
		{
			return false;
		}

		if (strncmp(username, src.username, sizeof(username)-1))
		{
			return false;
		}

		if (ip != src.ip)
		{
			return false;
		}

		if (pwd_content != src.pwd_content)
		{
			return false;
		}

		if (strncmp(hwid, src.hwid, sizeof(hwid)-1))
		{
			return false;
		}

		if (client_type != src.client_type)
		{
			return false;
		}

		if (strncmp(client_version, src.client_version, sizeof(client_version)-1))
		{
			return false;
		}

		if (strncmp(client_protocol_md5, src.client_protocol_md5, sizeof(client_protocol_md5)-1))
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

		if (session != src.session)
		{
			return false;
		}

		if (suid != src.suid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoginReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoginReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.sid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.ip, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.client_type, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.session, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.suid, str, length);
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
                JsonDelString(m_data.username, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned char> handler(m_data.pwd_content, this);
                handler.String(str, length, copy);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.hwid, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.client_version, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.client_protocol_md5, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.longitude, str, length);
            }
            break;
            case 9:
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
        LoginReq& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<12> m_assigned_bitset;

        static const std::bitset<12> template_assigned_bitset;
        static const char* m_key_list[12];
        static const char* m_alias_list[12];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddString(u8"username", username, sizeof(username));

		json_data.AddIntegral(u8"ip", ip);

		json_data.Writer().Key(u8"pwd_content", sizeof(u8"pwd_content") - 1);
		json_data.AddArray(pwd_content);

		json_data.AddString(u8"hwid", hwid, sizeof(hwid));

		json_data.AddIntegral(u8"client_type", client_type);

		json_data.AddString(u8"client_version", client_version, sizeof(client_version));

		json_data.AddString(u8"client_protocol_md5", client_protocol_md5, sizeof(client_protocol_md5));

		json_data.AddString(u8"longitude", longitude, sizeof(longitude));

		json_data.AddString(u8"latitude", latitude, sizeof(latitude));

		json_data.AddIntegral(u8"session", session);

		json_data.AddIntegral(u8"suid", suid);

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
        
        LoginReq::Handler h(*this, nullptr);
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

struct LoginAck:TProtocol<LoginAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 3;

	unsigned int              uid; //账号ID
	unsigned long long        guid; //正在GS中(战斗)的玩家GUID
	unsigned long long        suid; //会话ID
	unsigned long long        rid; //运行时ID(服务器重启后变化)
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(suid);

		net_data.AddIntegral(rid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(suid);

		net_data.AddIntegral(rid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(suid))
			return false;

		if (!net_data.DelIntegral(rid))
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

		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(suid))
			return false;

		if (!net_data.DelIntegral(rid))
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

		guid = 0;

		suid = 0;

		rid = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	LoginAck& operator= (const LoginAck& src)
	{
		uid = src.uid;

		guid = src.guid;

		suid = src.suid;

		rid = src.rid;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoginAck& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (guid != src.guid)
		{
			return false;
		}

		if (suid != src.suid)
		{
			return false;
		}

		if (rid != src.rid)
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
	bool operator != (const LoginAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoginAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.suid, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rid, str, length);
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
        LoginAck& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"suid", suid);

		json_data.AddIntegral(u8"rid", rid);

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
        
        LoginAck::Handler h(*this, nullptr);
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

struct ReloginReq:TProtocol<ReloginReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 4;

	unsigned char             sid; //子区ID
	unsigned int              uid; //账号ID
	unsigned int              ip; //登陆ip地址
	char                      hwid[128]; //硬件码
	unsigned char             client_type; //客户端类型
	char                      client_version[256]; //客户端版本号
	char                      client_protocol_md5[256]; //客户端协议MD5
	char                      longitude[64]; //经度
	char                      latitude[64]; //纬度
	unsigned long long        session; //会话ID(由网关填写)
	unsigned long long        suid; //会话ID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(sid);

		net_data.AddIntegral(uid);

		net_data.AddIntegral(ip);

		net_data.AddString(hwid, sizeof(hwid));

		net_data.AddIntegral(client_type);

		net_data.AddString(client_version, sizeof(client_version));

		net_data.AddString(client_protocol_md5, sizeof(client_protocol_md5));

		net_data.AddString(longitude, sizeof(longitude));

		net_data.AddString(latitude, sizeof(latitude));

		net_data.AddIntegral(session);

		net_data.AddIntegral(suid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(sid);

		net_data.AddIntegral(uid);

		net_data.AddIntegral(ip);

		net_data.AddString(hwid, sizeof(hwid));

		net_data.AddIntegral(client_type);

		net_data.AddString(client_version, sizeof(client_version));

		net_data.AddString(client_protocol_md5, sizeof(client_protocol_md5));

		net_data.AddString(longitude, sizeof(longitude));

		net_data.AddString(latitude, sizeof(latitude));

		net_data.AddIntegral(session);

		net_data.AddIntegral(suid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(ip))
			return false;

		if (!net_data.DelString(hwid, sizeof(hwid)))
			return false;

		if (!net_data.DelIntegral(client_type))
			return false;

		if (!net_data.DelString(client_version, sizeof(client_version)))
			return false;

		if (!net_data.DelString(client_protocol_md5, sizeof(client_protocol_md5)))
			return false;

		if (!net_data.DelString(longitude, sizeof(longitude)))
			return false;

		if (!net_data.DelString(latitude, sizeof(latitude)))
			return false;

		if (!net_data.DelIntegral(session))
			return false;

		if (!net_data.DelIntegral(suid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(ip))
			return false;

		if (!net_data.DelString(hwid, sizeof(hwid)))
			return false;

		if (!net_data.DelIntegral(client_type))
			return false;

		if (!net_data.DelString(client_version, sizeof(client_version)))
			return false;

		if (!net_data.DelString(client_protocol_md5, sizeof(client_protocol_md5)))
			return false;

		if (!net_data.DelString(longitude, sizeof(longitude)))
			return false;

		if (!net_data.DelString(latitude, sizeof(latitude)))
			return false;

		if (!net_data.DelIntegral(session))
			return false;

		if (!net_data.DelIntegral(suid))
			return false;

		return true;
	}
	void Reset(void)
	{
		sid = 0;

		uid = 0;

		ip = 0;

		hwid[0] = 0;

		client_type = 0;

		client_version[0] = 0;

		client_protocol_md5[0] = 0;

		longitude[0] = 0;

		latitude[0] = 0;

		session = 0;

		suid = 0;

	}
	ReloginReq& operator= (const ReloginReq& src)
	{
		sid = src.sid;

		uid = src.uid;

		ip = src.ip;

		{
			size_t str_len = strnlen(src.hwid, sizeof(hwid) - 1);
			memcpy(hwid, src.hwid, str_len + 1);
			hwid[sizeof(hwid) - 1] = 0;
		}

		client_type = src.client_type;

		{
			size_t str_len = strnlen(src.client_version, sizeof(client_version) - 1);
			memcpy(client_version, src.client_version, str_len + 1);
			client_version[sizeof(client_version) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.client_protocol_md5, sizeof(client_protocol_md5) - 1);
			memcpy(client_protocol_md5, src.client_protocol_md5, str_len + 1);
			client_protocol_md5[sizeof(client_protocol_md5) - 1] = 0;
		}

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

		session = src.session;

		suid = src.suid;

		return *this;
	}
	bool operator==(const ReloginReq& src) const
	{
		if (sid != src.sid)
		{
			return false;
		}

		if (uid != src.uid)
		{
			return false;
		}

		if (ip != src.ip)
		{
			return false;
		}

		if (strncmp(hwid, src.hwid, sizeof(hwid)-1))
		{
			return false;
		}

		if (client_type != src.client_type)
		{
			return false;
		}

		if (strncmp(client_version, src.client_version, sizeof(client_version)-1))
		{
			return false;
		}

		if (strncmp(client_protocol_md5, src.client_protocol_md5, sizeof(client_protocol_md5)-1))
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

		if (session != src.session)
		{
			return false;
		}

		if (suid != src.suid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ReloginReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ReloginReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.sid, str, length);
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
                JsonDelIntegral(m_data.ip, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.client_type, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.session, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.suid, str, length);
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
                JsonDelString(m_data.hwid, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.client_version, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.client_protocol_md5, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.longitude, str, length);
            }
            break;
            case 8:
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
        ReloginReq& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<11> m_assigned_bitset;

        static const std::bitset<11> template_assigned_bitset;
        static const char* m_key_list[11];
        static const char* m_alias_list[11];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"ip", ip);

		json_data.AddString(u8"hwid", hwid, sizeof(hwid));

		json_data.AddIntegral(u8"client_type", client_type);

		json_data.AddString(u8"client_version", client_version, sizeof(client_version));

		json_data.AddString(u8"client_protocol_md5", client_protocol_md5, sizeof(client_protocol_md5));

		json_data.AddString(u8"longitude", longitude, sizeof(longitude));

		json_data.AddString(u8"latitude", latitude, sizeof(latitude));

		json_data.AddIntegral(u8"session", session);

		json_data.AddIntegral(u8"suid", suid);

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
        
        ReloginReq::Handler h(*this, nullptr);
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

struct ReloginAck:TProtocol<ReloginAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 5;

	unsigned long long        guid; //在线玩家GUID
	unsigned long long        rid; //运行时ID(服务器重启后变化)
	unsigned int              gsindex; //GS索引
	unsigned int              arrayid; //GS上玩家对象数组下标
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(rid);

		net_data.AddIntegral(gsindex);

		net_data.AddIntegral(arrayid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(rid);

		net_data.AddIntegral(gsindex);

		net_data.AddIntegral(arrayid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(rid))
			return false;

		if (!net_data.DelIntegral(gsindex))
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
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(rid))
			return false;

		if (!net_data.DelIntegral(gsindex))
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
		guid = 0;

		rid = 0;

		gsindex = 0;

		arrayid = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	ReloginAck& operator= (const ReloginAck& src)
	{
		guid = src.guid;

		rid = src.rid;

		gsindex = src.gsindex;

		arrayid = src.arrayid;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const ReloginAck& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (rid != src.rid)
		{
			return false;
		}

		if (gsindex != src.gsindex)
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
	bool operator != (const ReloginAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ReloginAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.rid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gsindex, str, length);
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
        ReloginAck& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"rid", rid);

		json_data.AddIntegral(u8"gsindex", gsindex);

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
        
        ReloginAck::Handler h(*this, nullptr);
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

struct LogoutReq:TProtocol<LogoutReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 6;

	unsigned char             type; //1:回到选角界面 2:退出游戏
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(type);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(type))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

	}
	LogoutReq& operator= (const LogoutReq& src)
	{
		type = src.type;

		return *this;
	}
	bool operator==(const LogoutReq& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LogoutReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LogoutReq& data, JsonHandler* parent = nullptr)
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
        LogoutReq& m_data;
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

		json_data.AddIntegral(u8"type", type);

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
        
        LogoutReq::Handler h(*this, nullptr);
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

struct LogoutAck:TProtocol<LogoutAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 7;

	unsigned char             type; //1:回到选角界面 2:退出游戏
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
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
		type = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	LogoutAck& operator= (const LogoutAck& src)
	{
		type = src.type;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LogoutAck& src) const
	{
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
	bool operator != (const LogoutAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LogoutAck& data, JsonHandler* parent = nullptr)
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
        LogoutAck& m_data;
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
        
        LogoutAck::Handler h(*this, nullptr);
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

struct KickNtf:TProtocol<KickNtf>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 8;

	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		errcode = 0;

		errmsg[0] = 0;

	}
	KickNtf& operator= (const KickNtf& src)
	{
		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const KickNtf& src) const
	{
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
	bool operator != (const KickNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(KickNtf& data, JsonHandler* parent = nullptr)
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
            case 1:
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
        KickNtf& m_data;
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
        
        KickNtf::Handler h(*this, nullptr);
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

struct PlayerListReq:TProtocol<PlayerListReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 9;

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
        Handler(PlayerListReq& data, JsonHandler* parent = nullptr)
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
        PlayerListReq& m_data;
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
        
        PlayerListReq::Handler h(*this, nullptr);
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

struct PlayerListAck:TProtocol<PlayerListAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 10;

	unsigned long long        lastplayer; //最后一次登陆玩家
	DataArray<PlayerBrief, unsigned char> briefs; //玩家简略列表
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(lastplayer);

		net_data.AddArray(briefs);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(lastplayer);

		net_data.AddArray(briefs);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(lastplayer))
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
		if (!net_data.DelIntegral(lastplayer))
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
		lastplayer = 0;

		briefs.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	PlayerListAck& operator= (const PlayerListAck& src)
	{
		lastplayer = src.lastplayer;

		briefs = src.briefs;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const PlayerListAck& src) const
	{
		if (lastplayer != src.lastplayer)
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
	bool operator != (const PlayerListAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PlayerListAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.lastplayer, str, length);
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
        PlayerListAck& m_data;
        DataArrayHandler<PlayerBrief, unsigned char, PlayerBrief::Handler> m_briefs_handler;
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

		json_data.AddIntegral(u8"lastplayer", lastplayer);

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
        
        PlayerListAck::Handler h(*this, nullptr);
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

struct CreatePlayerReq:TProtocol<CreatePlayerReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 11;

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
	CreatePlayerReq& operator= (const CreatePlayerReq& src)
	{
		playerbrief = src.playerbrief;

		return *this;
	}
	bool operator==(const CreatePlayerReq& src) const
	{
		if (playerbrief != src.playerbrief)
		{
			return false;
		}

		return true;
	}
	bool operator != (const CreatePlayerReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CreatePlayerReq& data, JsonHandler* parent = nullptr)
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
        CreatePlayerReq& m_data;
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
        
        CreatePlayerReq::Handler h(*this, nullptr);
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

struct CreatePlayerAck:TProtocol<CreatePlayerAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 12;

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
	CreatePlayerAck& operator= (const CreatePlayerAck& src)
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
	bool operator==(const CreatePlayerAck& src) const
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
	bool operator != (const CreatePlayerAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CreatePlayerAck& data, JsonHandler* parent = nullptr)
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
        CreatePlayerAck& m_data;
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
        
        CreatePlayerAck::Handler h(*this, nullptr);
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

struct DestroyPlayerReq:TProtocol<DestroyPlayerReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 13;

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
	DestroyPlayerReq& operator= (const DestroyPlayerReq& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const DestroyPlayerReq& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DestroyPlayerReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DestroyPlayerReq& data, JsonHandler* parent = nullptr)
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
        DestroyPlayerReq& m_data;
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
        
        DestroyPlayerReq::Handler h(*this, nullptr);
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

struct DestroyPlayerAck:TProtocol<DestroyPlayerAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 14;

	unsigned long long        guid; //玩家GUID
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
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
		guid = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	DestroyPlayerAck& operator= (const DestroyPlayerAck& src)
	{
		guid = src.guid;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const DestroyPlayerAck& src) const
	{
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
	bool operator != (const DestroyPlayerAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DestroyPlayerAck& data, JsonHandler* parent = nullptr)
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
        DestroyPlayerAck& m_data;
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
        
        DestroyPlayerAck::Handler h(*this, nullptr);
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

struct RestorePlayerReq:TProtocol<RestorePlayerReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 15;

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
	RestorePlayerReq& operator= (const RestorePlayerReq& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const RestorePlayerReq& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const RestorePlayerReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(RestorePlayerReq& data, JsonHandler* parent = nullptr)
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
        RestorePlayerReq& m_data;
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
        
        RestorePlayerReq::Handler h(*this, nullptr);
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

struct RestorePlayerAck:TProtocol<RestorePlayerAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 16;

	unsigned long long        guid; //玩家GUID
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
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
		guid = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	RestorePlayerAck& operator= (const RestorePlayerAck& src)
	{
		guid = src.guid;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const RestorePlayerAck& src) const
	{
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
	bool operator != (const RestorePlayerAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(RestorePlayerAck& data, JsonHandler* parent = nullptr)
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
        RestorePlayerAck& m_data;
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
        
        RestorePlayerAck::Handler h(*this, nullptr);
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

struct EnterGSReq:TProtocol<EnterGSReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 17;

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
	EnterGSReq& operator= (const EnterGSReq& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const EnterGSReq& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const EnterGSReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(EnterGSReq& data, JsonHandler* parent = nullptr)
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
        EnterGSReq& m_data;
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
        
        EnterGSReq::Handler h(*this, nullptr);
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

struct EnterGSAck:TProtocol<EnterGSAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 18;

	unsigned long long        guid; //玩家GUID
	unsigned int              gsindex; //GS索引
	unsigned int              arrayid; //GS上玩家对象数组下标
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(gsindex);

		net_data.AddIntegral(arrayid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(gsindex);

		net_data.AddIntegral(arrayid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(gsindex))
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
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(gsindex))
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
		guid = 0;

		gsindex = 0;

		arrayid = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	EnterGSAck& operator= (const EnterGSAck& src)
	{
		guid = src.guid;

		gsindex = src.gsindex;

		arrayid = src.arrayid;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const EnterGSAck& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (gsindex != src.gsindex)
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
	bool operator != (const EnterGSAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(EnterGSAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.gsindex, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.arrayid, str, length);
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
        EnterGSAck& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"gsindex", gsindex);

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
        
        EnterGSAck::Handler h(*this, nullptr);
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

struct TrdLoginReq:TProtocol<TrdLoginReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 19;

	unsigned char             sid; //子区ID
	char                      username[256]; //账号名
	char                      platform[256]; //平台名称
	DataArray<unsigned char, unsigned short> token; //token
	char                      hwid[256]; //硬件码
	unsigned int              ip; //登陆ip地址
	unsigned char             client_type; //客户端类型
	char                      client_version[256]; //客户端版本号
	char                      client_protocol_md5[256]; //客户端协议MD5
	char                      longitude[64]; //经度
	char                      latitude[64]; //纬度
	unsigned long long        session; //会话ID(由网关填写)
	unsigned long long        suid; //会话ID(由网关填写)
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(sid);

		net_data.AddString(username, sizeof(username));

		net_data.AddString(platform, sizeof(platform));

		net_data.AddArray(token);

		net_data.AddString(hwid, sizeof(hwid));

		net_data.AddIntegral(ip);

		net_data.AddIntegral(client_type);

		net_data.AddString(client_version, sizeof(client_version));

		net_data.AddString(client_protocol_md5, sizeof(client_protocol_md5));

		net_data.AddString(longitude, sizeof(longitude));

		net_data.AddString(latitude, sizeof(latitude));

		net_data.AddIntegral(session);

		net_data.AddIntegral(suid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(sid);

		net_data.AddString(username, sizeof(username));

		net_data.AddString(platform, sizeof(platform));

		net_data.AddArray(token);

		net_data.AddString(hwid, sizeof(hwid));

		net_data.AddIntegral(ip);

		net_data.AddIntegral(client_type);

		net_data.AddString(client_version, sizeof(client_version));

		net_data.AddString(client_protocol_md5, sizeof(client_protocol_md5));

		net_data.AddString(longitude, sizeof(longitude));

		net_data.AddString(latitude, sizeof(latitude));

		net_data.AddIntegral(session);

		net_data.AddIntegral(suid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelString(username, sizeof(username)))
			return false;

		if (!net_data.DelString(platform, sizeof(platform)))
			return false;

		if(!net_data.DelArray(token))
			return false;

		if (!net_data.DelString(hwid, sizeof(hwid)))
			return false;

		if (!net_data.DelIntegral(ip))
			return false;

		if (!net_data.DelIntegral(client_type))
			return false;

		if (!net_data.DelString(client_version, sizeof(client_version)))
			return false;

		if (!net_data.DelString(client_protocol_md5, sizeof(client_protocol_md5)))
			return false;

		if (!net_data.DelString(longitude, sizeof(longitude)))
			return false;

		if (!net_data.DelString(latitude, sizeof(latitude)))
			return false;

		if (!net_data.DelIntegral(session))
			return false;

		if (!net_data.DelIntegral(suid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelString(username, sizeof(username)))
			return false;

		if (!net_data.DelString(platform, sizeof(platform)))
			return false;

		if(!net_data.DelArray(token))
			return false;

		if (!net_data.DelString(hwid, sizeof(hwid)))
			return false;

		if (!net_data.DelIntegral(ip))
			return false;

		if (!net_data.DelIntegral(client_type))
			return false;

		if (!net_data.DelString(client_version, sizeof(client_version)))
			return false;

		if (!net_data.DelString(client_protocol_md5, sizeof(client_protocol_md5)))
			return false;

		if (!net_data.DelString(longitude, sizeof(longitude)))
			return false;

		if (!net_data.DelString(latitude, sizeof(latitude)))
			return false;

		if (!net_data.DelIntegral(session))
			return false;

		if (!net_data.DelIntegral(suid))
			return false;

		return true;
	}
	void Reset(void)
	{
		sid = 0;

		username[0] = 0;

		platform[0] = 0;

		token.clear();

		hwid[0] = 0;

		ip = 0;

		client_type = 0;

		client_version[0] = 0;

		client_protocol_md5[0] = 0;

		longitude[0] = 0;

		latitude[0] = 0;

		session = 0;

		suid = 0;

	}
	TrdLoginReq& operator= (const TrdLoginReq& src)
	{
		sid = src.sid;

		{
			size_t str_len = strnlen(src.username, sizeof(username) - 1);
			memcpy(username, src.username, str_len + 1);
			username[sizeof(username) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.platform, sizeof(platform) - 1);
			memcpy(platform, src.platform, str_len + 1);
			platform[sizeof(platform) - 1] = 0;
		}

		token = src.token;

		{
			size_t str_len = strnlen(src.hwid, sizeof(hwid) - 1);
			memcpy(hwid, src.hwid, str_len + 1);
			hwid[sizeof(hwid) - 1] = 0;
		}

		ip = src.ip;

		client_type = src.client_type;

		{
			size_t str_len = strnlen(src.client_version, sizeof(client_version) - 1);
			memcpy(client_version, src.client_version, str_len + 1);
			client_version[sizeof(client_version) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.client_protocol_md5, sizeof(client_protocol_md5) - 1);
			memcpy(client_protocol_md5, src.client_protocol_md5, str_len + 1);
			client_protocol_md5[sizeof(client_protocol_md5) - 1] = 0;
		}

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

		session = src.session;

		suid = src.suid;

		return *this;
	}
	bool operator==(const TrdLoginReq& src) const
	{
		if (sid != src.sid)
		{
			return false;
		}

		if (strncmp(username, src.username, sizeof(username)-1))
		{
			return false;
		}

		if (strncmp(platform, src.platform, sizeof(platform)-1))
		{
			return false;
		}

		if (token != src.token)
		{
			return false;
		}

		if (strncmp(hwid, src.hwid, sizeof(hwid)-1))
		{
			return false;
		}

		if (ip != src.ip)
		{
			return false;
		}

		if (client_type != src.client_type)
		{
			return false;
		}

		if (strncmp(client_version, src.client_version, sizeof(client_version)-1))
		{
			return false;
		}

		if (strncmp(client_protocol_md5, src.client_protocol_md5, sizeof(client_protocol_md5)-1))
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

		if (session != src.session)
		{
			return false;
		}

		if (suid != src.suid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TrdLoginReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TrdLoginReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.sid, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.ip, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.client_type, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.session, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.suid, str, length);
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
                JsonDelString(m_data.username, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.platform, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.token, this);
                handler.String(str, length, copy);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.hwid, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.client_version, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.client_protocol_md5, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.longitude, str, length);
            }
            break;
            case 10:
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
        TrdLoginReq& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<13> m_assigned_bitset;

        static const std::bitset<13> template_assigned_bitset;
        static const char* m_key_list[13];
        static const char* m_alias_list[13];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddString(u8"username", username, sizeof(username));

		json_data.AddString(u8"platform", platform, sizeof(platform));

		json_data.Writer().Key(u8"token", sizeof(u8"token") - 1);
		json_data.AddArray(token);

		json_data.AddString(u8"hwid", hwid, sizeof(hwid));

		json_data.AddIntegral(u8"ip", ip);

		json_data.AddIntegral(u8"client_type", client_type);

		json_data.AddString(u8"client_version", client_version, sizeof(client_version));

		json_data.AddString(u8"client_protocol_md5", client_protocol_md5, sizeof(client_protocol_md5));

		json_data.AddString(u8"longitude", longitude, sizeof(longitude));

		json_data.AddString(u8"latitude", latitude, sizeof(latitude));

		json_data.AddIntegral(u8"session", session);

		json_data.AddIntegral(u8"suid", suid);

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
        
        TrdLoginReq::Handler h(*this, nullptr);
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

struct GetWSTimestampReq:TProtocol<GetWSTimestampReq>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 20;

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
        Handler(GetWSTimestampReq& data, JsonHandler* parent = nullptr)
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
        GetWSTimestampReq& m_data;
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
        
        GetWSTimestampReq::Handler h(*this, nullptr);
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

struct GetWSTimestampAck:TProtocol<GetWSTimestampAck>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 21;

	unsigned int              now; //当前服务器本地时间
	int                       zone; //当前服务器本地时间减去标准UTC时间的差值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(now);

		net_data.AddIntegral(zone);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(now);

		net_data.AddIntegral(zone);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(now))
			return false;

		if (!net_data.DelIntegral(zone))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(now))
			return false;

		if (!net_data.DelIntegral(zone))
			return false;

		return true;
	}
	void Reset(void)
	{
		now = 0;

		zone = 0;

	}
	GetWSTimestampAck& operator= (const GetWSTimestampAck& src)
	{
		now = src.now;

		zone = src.zone;

		return *this;
	}
	bool operator==(const GetWSTimestampAck& src) const
	{
		if (now != src.now)
		{
			return false;
		}

		if (zone != src.zone)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GetWSTimestampAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GetWSTimestampAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.now, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.zone, str, length);
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
        GetWSTimestampAck& m_data;
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

		json_data.AddIntegral(u8"now", now);

		json_data.AddIntegral(u8"zone", zone);

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
        
        GetWSTimestampAck::Handler h(*this, nullptr);
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

struct RealnameInfoNtf:TProtocol<RealnameInfoNtf>
{
	static constexpr unsigned short module_id = 101;
	static constexpr unsigned short protocol_id = 22;

	unsigned int              realname_status; //实名认证状态
	char                      auth[64]; //授权信息
	char                      realname_token[64]; //token
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(realname_status);

		net_data.AddString(auth, sizeof(auth));

		net_data.AddString(realname_token, sizeof(realname_token));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(realname_status);

		net_data.AddString(auth, sizeof(auth));

		net_data.AddString(realname_token, sizeof(realname_token));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(realname_status))
			return false;

		if (!net_data.DelString(auth, sizeof(auth)))
			return false;

		if (!net_data.DelString(realname_token, sizeof(realname_token)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(realname_status))
			return false;

		if (!net_data.DelString(auth, sizeof(auth)))
			return false;

		if (!net_data.DelString(realname_token, sizeof(realname_token)))
			return false;

		return true;
	}
	void Reset(void)
	{
		realname_status = 0;

		auth[0] = 0;

		realname_token[0] = 0;

	}
	RealnameInfoNtf& operator= (const RealnameInfoNtf& src)
	{
		realname_status = src.realname_status;

		{
			size_t str_len = strnlen(src.auth, sizeof(auth) - 1);
			memcpy(auth, src.auth, str_len + 1);
			auth[sizeof(auth) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.realname_token, sizeof(realname_token) - 1);
			memcpy(realname_token, src.realname_token, str_len + 1);
			realname_token[sizeof(realname_token) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const RealnameInfoNtf& src) const
	{
		if (realname_status != src.realname_status)
		{
			return false;
		}

		if (strncmp(auth, src.auth, sizeof(auth)-1))
		{
			return false;
		}

		if (strncmp(realname_token, src.realname_token, sizeof(realname_token)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const RealnameInfoNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(RealnameInfoNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.realname_status, str, length);
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
                JsonDelString(m_data.auth, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.realname_token, str, length);
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
        RealnameInfoNtf& m_data;
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

		json_data.AddIntegral(u8"realname_status", realname_status);

		json_data.AddString(u8"auth", auth, sizeof(auth));

		json_data.AddString(u8"realname_token", realname_token, sizeof(realname_token));

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
        
        RealnameInfoNtf::Handler h(*this, nullptr);
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
class CClientWS: public ProtocolModule
{
public:
	CClientWS()
	{
		size_t max_protocol_size = 0;
		if (sizeof(RandKeyNtf) > max_protocol_size)
			max_protocol_size = sizeof(RandKeyNtf);

		if (sizeof(LoginReq) > max_protocol_size)
			max_protocol_size = sizeof(LoginReq);

		if (sizeof(LoginAck) > max_protocol_size)
			max_protocol_size = sizeof(LoginAck);

		if (sizeof(ReloginReq) > max_protocol_size)
			max_protocol_size = sizeof(ReloginReq);

		if (sizeof(ReloginAck) > max_protocol_size)
			max_protocol_size = sizeof(ReloginAck);

		if (sizeof(LogoutReq) > max_protocol_size)
			max_protocol_size = sizeof(LogoutReq);

		if (sizeof(LogoutAck) > max_protocol_size)
			max_protocol_size = sizeof(LogoutAck);

		if (sizeof(KickNtf) > max_protocol_size)
			max_protocol_size = sizeof(KickNtf);

		if (sizeof(PlayerListReq) > max_protocol_size)
			max_protocol_size = sizeof(PlayerListReq);

		if (sizeof(PlayerListAck) > max_protocol_size)
			max_protocol_size = sizeof(PlayerListAck);

		if (sizeof(CreatePlayerReq) > max_protocol_size)
			max_protocol_size = sizeof(CreatePlayerReq);

		if (sizeof(CreatePlayerAck) > max_protocol_size)
			max_protocol_size = sizeof(CreatePlayerAck);

		if (sizeof(DestroyPlayerReq) > max_protocol_size)
			max_protocol_size = sizeof(DestroyPlayerReq);

		if (sizeof(DestroyPlayerAck) > max_protocol_size)
			max_protocol_size = sizeof(DestroyPlayerAck);

		if (sizeof(RestorePlayerReq) > max_protocol_size)
			max_protocol_size = sizeof(RestorePlayerReq);

		if (sizeof(RestorePlayerAck) > max_protocol_size)
			max_protocol_size = sizeof(RestorePlayerAck);

		if (sizeof(EnterGSReq) > max_protocol_size)
			max_protocol_size = sizeof(EnterGSReq);

		if (sizeof(EnterGSAck) > max_protocol_size)
			max_protocol_size = sizeof(EnterGSAck);

		if (sizeof(TrdLoginReq) > max_protocol_size)
			max_protocol_size = sizeof(TrdLoginReq);

		if (sizeof(GetWSTimestampReq) > max_protocol_size)
			max_protocol_size = sizeof(GetWSTimestampReq);

		if (sizeof(GetWSTimestampAck) > max_protocol_size)
			max_protocol_size = sizeof(GetWSTimestampAck);

		if (sizeof(RealnameInfoNtf) > max_protocol_size)
			max_protocol_size = sizeof(RealnameInfoNtf);

		m_protocol_buffer = S_MALLOC(max_protocol_size);
	}

	virtual ~CClientWS()
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
		case RandKeyNtf::protocol_id:
		{
			return RandKeyNtf::Name();
		}
		break;
		case LoginReq::protocol_id:
		{
			return LoginReq::Name();
		}
		break;
		case LoginAck::protocol_id:
		{
			return LoginAck::Name();
		}
		break;
		case ReloginReq::protocol_id:
		{
			return ReloginReq::Name();
		}
		break;
		case ReloginAck::protocol_id:
		{
			return ReloginAck::Name();
		}
		break;
		case LogoutReq::protocol_id:
		{
			return LogoutReq::Name();
		}
		break;
		case LogoutAck::protocol_id:
		{
			return LogoutAck::Name();
		}
		break;
		case KickNtf::protocol_id:
		{
			return KickNtf::Name();
		}
		break;
		case PlayerListReq::protocol_id:
		{
			return PlayerListReq::Name();
		}
		break;
		case PlayerListAck::protocol_id:
		{
			return PlayerListAck::Name();
		}
		break;
		case CreatePlayerReq::protocol_id:
		{
			return CreatePlayerReq::Name();
		}
		break;
		case CreatePlayerAck::protocol_id:
		{
			return CreatePlayerAck::Name();
		}
		break;
		case DestroyPlayerReq::protocol_id:
		{
			return DestroyPlayerReq::Name();
		}
		break;
		case DestroyPlayerAck::protocol_id:
		{
			return DestroyPlayerAck::Name();
		}
		break;
		case RestorePlayerReq::protocol_id:
		{
			return RestorePlayerReq::Name();
		}
		break;
		case RestorePlayerAck::protocol_id:
		{
			return RestorePlayerAck::Name();
		}
		break;
		case EnterGSReq::protocol_id:
		{
			return EnterGSReq::Name();
		}
		break;
		case EnterGSAck::protocol_id:
		{
			return EnterGSAck::Name();
		}
		break;
		case TrdLoginReq::protocol_id:
		{
			return TrdLoginReq::Name();
		}
		break;
		case GetWSTimestampReq::protocol_id:
		{
			return GetWSTimestampReq::Name();
		}
		break;
		case GetWSTimestampAck::protocol_id:
		{
			return GetWSTimestampAck::Name();
		}
		break;
		case RealnameInfoNtf::protocol_id:
		{
			return RealnameInfoNtf::Name();
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

		if (m_id != 101)
		{
			net_data.Reset(net_data_pos);
			return false;
		}

		switch(p_id)
		{
		case RandKeyNtf::protocol_id:
		{
			RandKeyNtf* proto = new(m_protocol_buffer) RandKeyNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_RandKeyNtf(*proto);
				proto->~RandKeyNtf();
				return true;
			}
			else
			{
				proto->~RandKeyNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoginReq::protocol_id:
		{
			LoginReq* proto = new(m_protocol_buffer) LoginReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoginReq(*proto);
				proto->~LoginReq();
				return true;
			}
			else
			{
				proto->~LoginReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoginAck::protocol_id:
		{
			LoginAck* proto = new(m_protocol_buffer) LoginAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoginAck(*proto);
				proto->~LoginAck();
				return true;
			}
			else
			{
				proto->~LoginAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ReloginReq::protocol_id:
		{
			ReloginReq* proto = new(m_protocol_buffer) ReloginReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ReloginReq(*proto);
				proto->~ReloginReq();
				return true;
			}
			else
			{
				proto->~ReloginReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ReloginAck::protocol_id:
		{
			ReloginAck* proto = new(m_protocol_buffer) ReloginAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ReloginAck(*proto);
				proto->~ReloginAck();
				return true;
			}
			else
			{
				proto->~ReloginAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LogoutReq::protocol_id:
		{
			LogoutReq* proto = new(m_protocol_buffer) LogoutReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LogoutReq(*proto);
				proto->~LogoutReq();
				return true;
			}
			else
			{
				proto->~LogoutReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LogoutAck::protocol_id:
		{
			LogoutAck* proto = new(m_protocol_buffer) LogoutAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LogoutAck(*proto);
				proto->~LogoutAck();
				return true;
			}
			else
			{
				proto->~LogoutAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case KickNtf::protocol_id:
		{
			KickNtf* proto = new(m_protocol_buffer) KickNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_KickNtf(*proto);
				proto->~KickNtf();
				return true;
			}
			else
			{
				proto->~KickNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case PlayerListReq::protocol_id:
		{
			PlayerListReq* proto = new(m_protocol_buffer) PlayerListReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_PlayerListReq(*proto);
				proto->~PlayerListReq();
				return true;
			}
			else
			{
				proto->~PlayerListReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case PlayerListAck::protocol_id:
		{
			PlayerListAck* proto = new(m_protocol_buffer) PlayerListAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_PlayerListAck(*proto);
				proto->~PlayerListAck();
				return true;
			}
			else
			{
				proto->~PlayerListAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case CreatePlayerReq::protocol_id:
		{
			CreatePlayerReq* proto = new(m_protocol_buffer) CreatePlayerReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_CreatePlayerReq(*proto);
				proto->~CreatePlayerReq();
				return true;
			}
			else
			{
				proto->~CreatePlayerReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case CreatePlayerAck::protocol_id:
		{
			CreatePlayerAck* proto = new(m_protocol_buffer) CreatePlayerAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_CreatePlayerAck(*proto);
				proto->~CreatePlayerAck();
				return true;
			}
			else
			{
				proto->~CreatePlayerAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DestroyPlayerReq::protocol_id:
		{
			DestroyPlayerReq* proto = new(m_protocol_buffer) DestroyPlayerReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DestroyPlayerReq(*proto);
				proto->~DestroyPlayerReq();
				return true;
			}
			else
			{
				proto->~DestroyPlayerReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DestroyPlayerAck::protocol_id:
		{
			DestroyPlayerAck* proto = new(m_protocol_buffer) DestroyPlayerAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DestroyPlayerAck(*proto);
				proto->~DestroyPlayerAck();
				return true;
			}
			else
			{
				proto->~DestroyPlayerAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case RestorePlayerReq::protocol_id:
		{
			RestorePlayerReq* proto = new(m_protocol_buffer) RestorePlayerReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_RestorePlayerReq(*proto);
				proto->~RestorePlayerReq();
				return true;
			}
			else
			{
				proto->~RestorePlayerReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case RestorePlayerAck::protocol_id:
		{
			RestorePlayerAck* proto = new(m_protocol_buffer) RestorePlayerAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_RestorePlayerAck(*proto);
				proto->~RestorePlayerAck();
				return true;
			}
			else
			{
				proto->~RestorePlayerAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case EnterGSReq::protocol_id:
		{
			EnterGSReq* proto = new(m_protocol_buffer) EnterGSReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_EnterGSReq(*proto);
				proto->~EnterGSReq();
				return true;
			}
			else
			{
				proto->~EnterGSReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case EnterGSAck::protocol_id:
		{
			EnterGSAck* proto = new(m_protocol_buffer) EnterGSAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_EnterGSAck(*proto);
				proto->~EnterGSAck();
				return true;
			}
			else
			{
				proto->~EnterGSAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case TrdLoginReq::protocol_id:
		{
			TrdLoginReq* proto = new(m_protocol_buffer) TrdLoginReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_TrdLoginReq(*proto);
				proto->~TrdLoginReq();
				return true;
			}
			else
			{
				proto->~TrdLoginReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case GetWSTimestampReq::protocol_id:
		{
			GetWSTimestampReq* proto = new(m_protocol_buffer) GetWSTimestampReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_GetWSTimestampReq(*proto);
				proto->~GetWSTimestampReq();
				return true;
			}
			else
			{
				proto->~GetWSTimestampReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case GetWSTimestampAck::protocol_id:
		{
			GetWSTimestampAck* proto = new(m_protocol_buffer) GetWSTimestampAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_GetWSTimestampAck(*proto);
				proto->~GetWSTimestampAck();
				return true;
			}
			else
			{
				proto->~GetWSTimestampAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case RealnameInfoNtf::protocol_id:
		{
			RealnameInfoNtf* proto = new(m_protocol_buffer) RealnameInfoNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_RealnameInfoNtf(*proto);
				proto->~RealnameInfoNtf();
				return true;
			}
			else
			{
				proto->~RealnameInfoNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		default:
			return false;
		}

	}

	static inline unsigned short GetModuleID(void){ return 101; }

	static inline unsigned short GetProtocolNum(void){ return 22; }

	static const unsigned short module_id = 101;

	static const unsigned short protocol_num = 22;

	unsigned short ModuleId() override { return D::GetModuleID(); }
	unsigned short ProtocolNum() override { return D::GetProtocolNum(); }
	bool Handle(NetDeCode & net_data) override { return static_cast<D*>(this)->HandleProtocol(net_data); }
//===============以下协议回调函数需要使用者来实现===============
	void OnRecv_RandKeyNtf(RandKeyNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoginReq(LoginReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoginAck(LoginAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ReloginReq(ReloginReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ReloginAck(ReloginAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LogoutReq(LogoutReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LogoutAck(LogoutAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_KickNtf(KickNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_PlayerListReq(PlayerListReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_PlayerListAck(PlayerListAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_CreatePlayerReq(CreatePlayerReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_CreatePlayerAck(CreatePlayerAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DestroyPlayerReq(DestroyPlayerReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DestroyPlayerAck(DestroyPlayerAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_RestorePlayerReq(RestorePlayerReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_RestorePlayerAck(RestorePlayerAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_EnterGSReq(EnterGSReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_EnterGSAck(EnterGSAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_TrdLoginReq(TrdLoginReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_GetWSTimestampReq(GetWSTimestampReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_GetWSTimestampAck(GetWSTimestampAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_RealnameInfoNtf(RealnameInfoNtf& rstProtocol){ (void)(rstProtocol); };
private:
	 void* m_protocol_buffer;

};
