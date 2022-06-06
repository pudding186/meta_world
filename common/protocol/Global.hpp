#pragma once

#include "net_data.hpp"

//===============包含的其他协议文件===============
#include "StructDef.hpp"
 //===============宏定义开始===============
//===============宏定义结束===============

//===============数据定义开始===============
struct NewGSNtf:TProtocol<NewGSNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 1;

	unsigned int              index; //新GS的索引id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	NewGSNtf& operator= (const NewGSNtf& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const NewGSNtf& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const NewGSNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(NewGSNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        NewGSNtf& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        NewGSNtf::Handler h(*this, nullptr);
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

struct TerminateNtf:TProtocol<TerminateNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 2;

	unsigned long long        session; //会话ID(由网关填写,用于多网关账号互顶区分)
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(session);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(session);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(session))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(session))
			return false;

		return true;
	}
	void Reset(void)
	{
		session = 0;

	}
	TerminateNtf& operator= (const TerminateNtf& src)
	{
		session = src.session;

		return *this;
	}
	bool operator==(const TerminateNtf& src) const
	{
		if (session != src.session)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TerminateNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TerminateNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.session, str, length);
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
        TerminateNtf& m_data;
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

		json_data.AddIntegral(u8"session", session);

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
        
        TerminateNtf::Handler h(*this, nullptr);
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

struct ErrorNtf:TProtocol<ErrorNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 3;

	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
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
	ErrorNtf& operator= (const ErrorNtf& src)
	{
		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const ErrorNtf& src) const
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
	bool operator != (const ErrorNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ErrorNtf& data, JsonHandler* parent = nullptr)
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
        ErrorNtf& m_data;
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
        
        ErrorNtf::Handler h(*this, nullptr);
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

struct ServerStopNtf:TProtocol<ServerStopNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 4;

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
        Handler(ServerStopNtf& data, JsonHandler* parent = nullptr)
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
        ServerStopNtf& m_data;
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
        
        ServerStopNtf::Handler h(*this, nullptr);
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

struct ServerConfigNtf:TProtocol<ServerConfigNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 5;

	unsigned long long        svr_id; //服务器id
	unsigned int              limit; //服务器人数限制
	DataArray<IPConfig, unsigned char> listen; //监听列表
	DataArray<IPConfig, unsigned char> connect; //链接链表
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(svr_id);

		net_data.AddIntegral(limit);

		net_data.AddArray(listen);

		net_data.AddArray(connect);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(svr_id);

		net_data.AddIntegral(limit);

		net_data.AddArray(listen);

		net_data.AddArray(connect);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(svr_id))
			return false;

		if (!net_data.DelIntegral(limit))
			return false;

		if(!net_data.DelArray(listen))
			return false;

		if(!net_data.DelArray(connect))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(svr_id))
			return false;

		if (!net_data.DelIntegral(limit))
			return false;

		if(!net_data.DelArray(listen))
			return false;

		if(!net_data.DelArray(connect))
			return false;

		return true;
	}
	void Reset(void)
	{
		svr_id = 0;

		limit = 0;

		listen.clear();

		connect.clear();

	}
	ServerConfigNtf& operator= (const ServerConfigNtf& src)
	{
		svr_id = src.svr_id;

		limit = src.limit;

		listen = src.listen;

		connect = src.connect;

		return *this;
	}
	bool operator==(const ServerConfigNtf& src) const
	{
		if (svr_id != src.svr_id)
		{
			return false;
		}

		if (limit != src.limit)
		{
			return false;
		}

		if (listen != src.listen)
		{
			return false;
		}

		if (connect != src.connect)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ServerConfigNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ServerConfigNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_listen_handler(m_data.listen, this),
            m_connect_handler(m_data.connect, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_listen_handler.ResetState();
            m_connect_handler.ResetState();
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
                JsonDelIntegral(m_data.svr_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.limit, str, length);
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_listen_handler;
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_connect_handler;
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

            if (!m_listen_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_connect_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ServerConfigNtf& m_data;
        DataArrayHandler<IPConfig, unsigned char, IPConfig::Handler> m_listen_handler;
        DataArrayHandler<IPConfig, unsigned char, IPConfig::Handler> m_connect_handler;
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

		json_data.AddIntegral(u8"svr_id", svr_id);

		json_data.AddIntegral(u8"limit", limit);

		json_data.Writer().Key(u8"listen", sizeof(u8"listen") - 1);
		json_data.AddArray(listen);

		json_data.Writer().Key(u8"connect", sizeof(u8"connect") - 1);
		json_data.AddArray(connect);

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
        
        ServerConfigNtf::Handler h(*this, nullptr);
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

struct DBWrapperPkg:TProtocol<DBWrapperPkg>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 6;

	DataArray<unsigned char, unsigned int> wrapper; //包装数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(wrapper);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddArray(wrapper);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(wrapper))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if(!net_data.DelArray(wrapper))
			return false;

		return true;
	}
	void Reset(void)
	{
		wrapper.clear();

	}
	DBWrapperPkg& operator= (const DBWrapperPkg& src)
	{
		wrapper = src.wrapper;

		return *this;
	}
	bool operator==(const DBWrapperPkg& src) const
	{
		if (wrapper != src.wrapper)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DBWrapperPkg& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DBWrapperPkg& data, JsonHandler* parent = nullptr)
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
        DBWrapperPkg& m_data;
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
        
        DBWrapperPkg::Handler h(*this, nullptr);
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

struct GSWrapperPkg:TProtocol<GSWrapperPkg>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 7;

	unsigned int              index; //gs索引
	DataArray<unsigned char, unsigned int> wrapper; //包装数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddArray(wrapper);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddArray(wrapper);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(wrapper))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(wrapper))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		wrapper.clear();

	}
	GSWrapperPkg& operator= (const GSWrapperPkg& src)
	{
		index = src.index;

		wrapper = src.wrapper;

		return *this;
	}
	bool operator==(const GSWrapperPkg& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (wrapper != src.wrapper)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GSWrapperPkg& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GSWrapperPkg& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        GSWrapperPkg& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        GSWrapperPkg::Handler h(*this, nullptr);
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

struct LoadAuctionObjectDataReq:TProtocol<LoadAuctionObjectDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 8;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadAuctionObjectDataReq& operator= (const LoadAuctionObjectDataReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadAuctionObjectDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadAuctionObjectDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadAuctionObjectDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadAuctionObjectDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadAuctionObjectDataReq::Handler h(*this, nullptr);
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

struct LoadAuctionObjectDataAck:TProtocol<LoadAuctionObjectDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 9;

	unsigned int              index; //gs索引
	unsigned int              total_num; //总记录数
	unsigned int              remain_num; //剩余记录数
	DataArray<AuctionObjectData, unsigned int> datas; //拍卖行道具列表
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(total_num);

		net_data.AddIntegral(remain_num);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(total_num);

		net_data.AddIntegral(remain_num);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(total_num))
			return false;

		if (!net_data.DelIntegral(remain_num))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(total_num))
			return false;

		if (!net_data.DelIntegral(remain_num))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		total_num = 0;

		remain_num = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadAuctionObjectDataAck& operator= (const LoadAuctionObjectDataAck& src)
	{
		index = src.index;

		total_num = src.total_num;

		remain_num = src.remain_num;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadAuctionObjectDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (total_num != src.total_num)
		{
			return false;
		}

		if (remain_num != src.remain_num)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadAuctionObjectDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadAuctionObjectDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.total_num, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.remain_num, str, length);
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
            switch (m_state)
            {
            case 3:
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
        LoadAuctionObjectDataAck& m_data;
        DataArrayHandler<AuctionObjectData, unsigned int, AuctionObjectData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"total_num", total_num);

		json_data.AddIntegral(u8"remain_num", remain_num);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadAuctionObjectDataAck::Handler h(*this, nullptr);
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

struct AddAuctionObjectDataNtf:TProtocol<AddAuctionObjectDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 10;

	AuctionObjectData         data; //拍卖行道具
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
	AddAuctionObjectDataNtf& operator= (const AddAuctionObjectDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddAuctionObjectDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddAuctionObjectDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddAuctionObjectDataNtf& data, JsonHandler* parent = nullptr)
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
        AddAuctionObjectDataNtf& m_data;
        AuctionObjectData::Handler m_data_handler;
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
        
        AddAuctionObjectDataNtf::Handler h(*this, nullptr);
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

struct DelAuctionObjectDataNtf:TProtocol<DelAuctionObjectDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 11;

	unsigned long long        guid; //拍卖行道具GUID
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
	DelAuctionObjectDataNtf& operator= (const DelAuctionObjectDataNtf& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const DelAuctionObjectDataNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelAuctionObjectDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelAuctionObjectDataNtf& data, JsonHandler* parent = nullptr)
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
        DelAuctionObjectDataNtf& m_data;
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
        
        DelAuctionObjectDataNtf::Handler h(*this, nullptr);
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

struct UpdateAuctionObjectDataNtf:TProtocol<UpdateAuctionObjectDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 12;

	AuctionObjectData         data; //拍卖行道具
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
	UpdateAuctionObjectDataNtf& operator= (const UpdateAuctionObjectDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const UpdateAuctionObjectDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateAuctionObjectDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateAuctionObjectDataNtf& data, JsonHandler* parent = nullptr)
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
        UpdateAuctionObjectDataNtf& m_data;
        AuctionObjectData::Handler m_data_handler;
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
        
        UpdateAuctionObjectDataNtf::Handler h(*this, nullptr);
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

struct LoadAuctionCookieDataReq:TProtocol<LoadAuctionCookieDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 13;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadAuctionCookieDataReq& operator= (const LoadAuctionCookieDataReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadAuctionCookieDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadAuctionCookieDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadAuctionCookieDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadAuctionCookieDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadAuctionCookieDataReq::Handler h(*this, nullptr);
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

struct LoadAuctionCookieDataAck:TProtocol<LoadAuctionCookieDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 14;

	unsigned int              index; //gs索引
	unsigned int              total_num; //总记录数
	unsigned int              remain_num; //剩余记录数
	DataArray<AuctionCookieData, unsigned int> datas; //拍卖行个人数据列表
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(total_num);

		net_data.AddIntegral(remain_num);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(total_num);

		net_data.AddIntegral(remain_num);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(total_num))
			return false;

		if (!net_data.DelIntegral(remain_num))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(total_num))
			return false;

		if (!net_data.DelIntegral(remain_num))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		total_num = 0;

		remain_num = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadAuctionCookieDataAck& operator= (const LoadAuctionCookieDataAck& src)
	{
		index = src.index;

		total_num = src.total_num;

		remain_num = src.remain_num;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadAuctionCookieDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (total_num != src.total_num)
		{
			return false;
		}

		if (remain_num != src.remain_num)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadAuctionCookieDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadAuctionCookieDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.total_num, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.remain_num, str, length);
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
            switch (m_state)
            {
            case 3:
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
        LoadAuctionCookieDataAck& m_data;
        DataArrayHandler<AuctionCookieData, unsigned int, AuctionCookieData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"total_num", total_num);

		json_data.AddIntegral(u8"remain_num", remain_num);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadAuctionCookieDataAck::Handler h(*this, nullptr);
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

struct DuplicateAuctionCookieDataNtf:TProtocol<DuplicateAuctionCookieDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 15;

	AuctionCookieData         data; //拍卖行个人数据
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
	DuplicateAuctionCookieDataNtf& operator= (const DuplicateAuctionCookieDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const DuplicateAuctionCookieDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DuplicateAuctionCookieDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DuplicateAuctionCookieDataNtf& data, JsonHandler* parent = nullptr)
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
        DuplicateAuctionCookieDataNtf& m_data;
        AuctionCookieData::Handler m_data_handler;
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
        
        DuplicateAuctionCookieDataNtf::Handler h(*this, nullptr);
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

struct LoadGuildDataReq:TProtocol<LoadGuildDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 16;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadGuildDataReq& operator= (const LoadGuildDataReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadGuildDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadGuildDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadGuildDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadGuildDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadGuildDataReq::Handler h(*this, nullptr);
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

struct LoadGuildDataAck:TProtocol<LoadGuildDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 17;

	unsigned int              index; //gs索引
	DataArray<DBGuildData, unsigned short> datas; //帮派数据
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadGuildDataAck& operator= (const LoadGuildDataAck& src)
	{
		index = src.index;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadGuildDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadGuildDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadGuildDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadGuildDataAck& m_data;
        DataArrayHandler<DBGuildData, unsigned short, DBGuildData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadGuildDataAck::Handler h(*this, nullptr);
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

struct LoadGuildMemberDataAck:TProtocol<LoadGuildMemberDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 18;

	unsigned int              index; //gs索引
	DataArray<DBGuildMemberData, unsigned int> datas; //帮派成员数据
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadGuildMemberDataAck& operator= (const LoadGuildMemberDataAck& src)
	{
		index = src.index;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadGuildMemberDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadGuildMemberDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadGuildMemberDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadGuildMemberDataAck& m_data;
        DataArrayHandler<DBGuildMemberData, unsigned int, DBGuildMemberData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadGuildMemberDataAck::Handler h(*this, nullptr);
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

struct LoadGuildApplicantDataAck:TProtocol<LoadGuildApplicantDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 19;

	unsigned int              index; //gs索引
	DataArray<DBGuildApplicantData, unsigned int> datas; //帮派申请数据
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadGuildApplicantDataAck& operator= (const LoadGuildApplicantDataAck& src)
	{
		index = src.index;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadGuildApplicantDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadGuildApplicantDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadGuildApplicantDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadGuildApplicantDataAck& m_data;
        DataArrayHandler<DBGuildApplicantData, unsigned int, DBGuildApplicantData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadGuildApplicantDataAck::Handler h(*this, nullptr);
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

struct AddGuildDataNtf:TProtocol<AddGuildDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 20;

	DBGuildData               guild; //帮派数据
	bool EnCode(NetEnCode& net_data)
	{
		guild.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		guild.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!guild.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!guild.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guild.Reset();

	}
	AddGuildDataNtf& operator= (const AddGuildDataNtf& src)
	{
		guild = src.guild;

		return *this;
	}
	bool operator==(const AddGuildDataNtf& src) const
	{
		if (guild != src.guild)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddGuildDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddGuildDataNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_guild_handler(m_data.guild, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_guild_handler.ResetState();
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
                return &m_guild_handler;
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

            if (!m_guild_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        AddGuildDataNtf& m_data;
        DBGuildData::Handler m_guild_handler;
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

		json_data.Writer().Key(u8"guild", sizeof(u8"guild") - 1);
		guild.ToJson(json_data);

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
        
        AddGuildDataNtf::Handler h(*this, nullptr);
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

struct DelGuildDataNtf:TProtocol<DelGuildDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 21;

	unsigned long long        guid; //帮派GUID
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
	DelGuildDataNtf& operator= (const DelGuildDataNtf& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const DelGuildDataNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelGuildDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelGuildDataNtf& data, JsonHandler* parent = nullptr)
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
        DelGuildDataNtf& m_data;
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
        
        DelGuildDataNtf::Handler h(*this, nullptr);
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

struct UpdateGuildDataNtf:TProtocol<UpdateGuildDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 22;

	DBGuildData               guild; //帮派数据
	bool EnCode(NetEnCode& net_data)
	{
		guild.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		guild.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!guild.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!guild.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guild.Reset();

	}
	UpdateGuildDataNtf& operator= (const UpdateGuildDataNtf& src)
	{
		guild = src.guild;

		return *this;
	}
	bool operator==(const UpdateGuildDataNtf& src) const
	{
		if (guild != src.guild)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateGuildDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateGuildDataNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_guild_handler(m_data.guild, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_guild_handler.ResetState();
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
                return &m_guild_handler;
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

            if (!m_guild_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        UpdateGuildDataNtf& m_data;
        DBGuildData::Handler m_guild_handler;
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

		json_data.Writer().Key(u8"guild", sizeof(u8"guild") - 1);
		guild.ToJson(json_data);

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
        
        UpdateGuildDataNtf::Handler h(*this, nullptr);
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

struct AddGuildMemberDataNtf:TProtocol<AddGuildMemberDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 23;

	DBGuildMemberData         member; //帮派数据
	bool EnCode(NetEnCode& net_data)
	{
		member.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		member.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!member.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!member.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		member.Reset();

	}
	AddGuildMemberDataNtf& operator= (const AddGuildMemberDataNtf& src)
	{
		member = src.member;

		return *this;
	}
	bool operator==(const AddGuildMemberDataNtf& src) const
	{
		if (member != src.member)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddGuildMemberDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddGuildMemberDataNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_member_handler(m_data.member, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_member_handler.ResetState();
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
                return &m_member_handler;
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

            if (!m_member_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        AddGuildMemberDataNtf& m_data;
        DBGuildMemberData::Handler m_member_handler;
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

		json_data.Writer().Key(u8"member", sizeof(u8"member") - 1);
		member.ToJson(json_data);

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
        
        AddGuildMemberDataNtf::Handler h(*this, nullptr);
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

struct DelGuildMemberDataNtf:TProtocol<DelGuildMemberDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 24;

	unsigned long long        player_guid; //成员GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(player_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(player_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		player_guid = 0;

	}
	DelGuildMemberDataNtf& operator= (const DelGuildMemberDataNtf& src)
	{
		player_guid = src.player_guid;

		return *this;
	}
	bool operator==(const DelGuildMemberDataNtf& src) const
	{
		if (player_guid != src.player_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelGuildMemberDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelGuildMemberDataNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.player_guid, str, length);
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
        DelGuildMemberDataNtf& m_data;
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

		json_data.AddIntegral(u8"player_guid", player_guid);

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
        
        DelGuildMemberDataNtf::Handler h(*this, nullptr);
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

struct UpdateGuildMemberDataNtf:TProtocol<UpdateGuildMemberDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 25;

	DBGuildMemberData         member; //帮派成员数据
	bool EnCode(NetEnCode& net_data)
	{
		member.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		member.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!member.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!member.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		member.Reset();

	}
	UpdateGuildMemberDataNtf& operator= (const UpdateGuildMemberDataNtf& src)
	{
		member = src.member;

		return *this;
	}
	bool operator==(const UpdateGuildMemberDataNtf& src) const
	{
		if (member != src.member)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateGuildMemberDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateGuildMemberDataNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_member_handler(m_data.member, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_member_handler.ResetState();
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
                return &m_member_handler;
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

            if (!m_member_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        UpdateGuildMemberDataNtf& m_data;
        DBGuildMemberData::Handler m_member_handler;
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

		json_data.Writer().Key(u8"member", sizeof(u8"member") - 1);
		member.ToJson(json_data);

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
        
        UpdateGuildMemberDataNtf::Handler h(*this, nullptr);
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

struct AddGuildApplicantDataNtf:TProtocol<AddGuildApplicantDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 26;

	DBGuildApplicantData      applicant; //帮派申请数据
	bool EnCode(NetEnCode& net_data)
	{
		applicant.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		applicant.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!applicant.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!applicant.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		applicant.Reset();

	}
	AddGuildApplicantDataNtf& operator= (const AddGuildApplicantDataNtf& src)
	{
		applicant = src.applicant;

		return *this;
	}
	bool operator==(const AddGuildApplicantDataNtf& src) const
	{
		if (applicant != src.applicant)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddGuildApplicantDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddGuildApplicantDataNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_applicant_handler(m_data.applicant, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_applicant_handler.ResetState();
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
                return &m_applicant_handler;
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

            if (!m_applicant_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        AddGuildApplicantDataNtf& m_data;
        DBGuildApplicantData::Handler m_applicant_handler;
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

		json_data.Writer().Key(u8"applicant", sizeof(u8"applicant") - 1);
		applicant.ToJson(json_data);

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
        
        AddGuildApplicantDataNtf::Handler h(*this, nullptr);
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

struct DelGuildApplicantDataNtf:TProtocol<DelGuildApplicantDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 27;

	unsigned long long        player_guid; //玩家GUID
	unsigned long long        guild_guid; //帮派GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(player_guid);

		net_data.AddIntegral(guild_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(player_guid);

		net_data.AddIntegral(guild_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelIntegral(guild_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelIntegral(guild_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		player_guid = 0;

		guild_guid = 0;

	}
	DelGuildApplicantDataNtf& operator= (const DelGuildApplicantDataNtf& src)
	{
		player_guid = src.player_guid;

		guild_guid = src.guild_guid;

		return *this;
	}
	bool operator==(const DelGuildApplicantDataNtf& src) const
	{
		if (player_guid != src.player_guid)
		{
			return false;
		}

		if (guild_guid != src.guild_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelGuildApplicantDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelGuildApplicantDataNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.player_guid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.guild_guid, str, length);
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
        DelGuildApplicantDataNtf& m_data;
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

		json_data.AddIntegral(u8"player_guid", player_guid);

		json_data.AddIntegral(u8"guild_guid", guild_guid);

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
        
        DelGuildApplicantDataNtf::Handler h(*this, nullptr);
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

struct UpdateGuildApplicantDataNtf:TProtocol<UpdateGuildApplicantDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 28;

	DBGuildApplicantData      applicant; //帮派申请数据
	bool EnCode(NetEnCode& net_data)
	{
		applicant.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		applicant.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!applicant.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!applicant.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		applicant.Reset();

	}
	UpdateGuildApplicantDataNtf& operator= (const UpdateGuildApplicantDataNtf& src)
	{
		applicant = src.applicant;

		return *this;
	}
	bool operator==(const UpdateGuildApplicantDataNtf& src) const
	{
		if (applicant != src.applicant)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateGuildApplicantDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateGuildApplicantDataNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_applicant_handler(m_data.applicant, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_applicant_handler.ResetState();
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
                return &m_applicant_handler;
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

            if (!m_applicant_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        UpdateGuildApplicantDataNtf& m_data;
        DBGuildApplicantData::Handler m_applicant_handler;
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

		json_data.Writer().Key(u8"applicant", sizeof(u8"applicant") - 1);
		applicant.ToJson(json_data);

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
        
        UpdateGuildApplicantDataNtf::Handler h(*this, nullptr);
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

struct LoadContactDataReq:TProtocol<LoadContactDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 29;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadContactDataReq& operator= (const LoadContactDataReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadContactDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadContactDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadContactDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadContactDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadContactDataReq::Handler h(*this, nullptr);
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

struct LoadContactInfoDataAck:TProtocol<LoadContactInfoDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 30;

	unsigned int              index; //gs索引
	DataArray<ContactInfoData, unsigned int> datas; //联系人信息数据
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadContactInfoDataAck& operator= (const LoadContactInfoDataAck& src)
	{
		index = src.index;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadContactInfoDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadContactInfoDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadContactInfoDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadContactInfoDataAck& m_data;
        DataArrayHandler<ContactInfoData, unsigned int, ContactInfoData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadContactInfoDataAck::Handler h(*this, nullptr);
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

struct LoadContactDataAck:TProtocol<LoadContactDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 31;

	unsigned int              index; //gs索引
	DataArray<DBContactData, unsigned int> datas; //联系人数据
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadContactDataAck& operator= (const LoadContactDataAck& src)
	{
		index = src.index;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadContactDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadContactDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadContactDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadContactDataAck& m_data;
        DataArrayHandler<DBContactData, unsigned int, DBContactData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadContactDataAck::Handler h(*this, nullptr);
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

struct AddContactDataNtf:TProtocol<AddContactDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 32;

	DBContactData             data; //联系人数据
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
	AddContactDataNtf& operator= (const AddContactDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddContactDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddContactDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddContactDataNtf& data, JsonHandler* parent = nullptr)
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
        AddContactDataNtf& m_data;
        DBContactData::Handler m_data_handler;
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
        
        AddContactDataNtf::Handler h(*this, nullptr);
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

struct DelContactDataNtf:TProtocol<DelContactDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 33;

	unsigned long long        player_guid; //玩家GUID
	unsigned long long        target_guid; //联系人GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(player_guid);

		net_data.AddIntegral(target_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(player_guid);

		net_data.AddIntegral(target_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelIntegral(target_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelIntegral(target_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		player_guid = 0;

		target_guid = 0;

	}
	DelContactDataNtf& operator= (const DelContactDataNtf& src)
	{
		player_guid = src.player_guid;

		target_guid = src.target_guid;

		return *this;
	}
	bool operator==(const DelContactDataNtf& src) const
	{
		if (player_guid != src.player_guid)
		{
			return false;
		}

		if (target_guid != src.target_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelContactDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelContactDataNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.player_guid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target_guid, str, length);
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
        DelContactDataNtf& m_data;
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

		json_data.AddIntegral(u8"player_guid", player_guid);

		json_data.AddIntegral(u8"target_guid", target_guid);

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
        
        DelContactDataNtf::Handler h(*this, nullptr);
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

struct UpdateContactDataNtf:TProtocol<UpdateContactDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 34;

	DBContactData             data; //联系人数据
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
	UpdateContactDataNtf& operator= (const UpdateContactDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const UpdateContactDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateContactDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateContactDataNtf& data, JsonHandler* parent = nullptr)
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
        UpdateContactDataNtf& m_data;
        DBContactData::Handler m_data_handler;
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
        
        UpdateContactDataNtf::Handler h(*this, nullptr);
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

struct LoadDBVarDataReq:TProtocol<LoadDBVarDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 35;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadDBVarDataReq& operator= (const LoadDBVarDataReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadDBVarDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadDBVarDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadDBVarDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadDBVarDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadDBVarDataReq::Handler h(*this, nullptr);
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

struct LoadDBVarDataAck:TProtocol<LoadDBVarDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 36;

	unsigned int              index; //gs索引
	DataArray<DBVarData, unsigned int> datas; //DB变量数据
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadDBVarDataAck& operator= (const LoadDBVarDataAck& src)
	{
		index = src.index;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadDBVarDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadDBVarDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadDBVarDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadDBVarDataAck& m_data;
        DataArrayHandler<DBVarData, unsigned int, DBVarData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadDBVarDataAck::Handler h(*this, nullptr);
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

struct AddDBVarDataNtf:TProtocol<AddDBVarDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 37;

	DBVarData                 data; //DB变量数据
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
	AddDBVarDataNtf& operator= (const AddDBVarDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddDBVarDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddDBVarDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddDBVarDataNtf& data, JsonHandler* parent = nullptr)
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
        AddDBVarDataNtf& m_data;
        DBVarData::Handler m_data_handler;
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
        
        AddDBVarDataNtf::Handler h(*this, nullptr);
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

struct DelDBVarDataNtf:TProtocol<DelDBVarDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 38;

	DataArray<unsigned char, unsigned char> key; //key值
	unsigned char             value_type; //值类型
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(key);

		net_data.AddIntegral(value_type);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddArray(key);

		net_data.AddIntegral(value_type);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(key))
			return false;

		if (!net_data.DelIntegral(value_type))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if(!net_data.DelArray(key))
			return false;

		if (!net_data.DelIntegral(value_type))
			return false;

		return true;
	}
	void Reset(void)
	{
		key.clear();

		value_type = 0;

	}
	DelDBVarDataNtf& operator= (const DelDBVarDataNtf& src)
	{
		key = src.key;

		value_type = src.value_type;

		return *this;
	}
	bool operator==(const DelDBVarDataNtf& src) const
	{
		if (key != src.key)
		{
			return false;
		}

		if (value_type != src.value_type)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelDBVarDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelDBVarDataNtf& data, JsonHandler* parent = nullptr)
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
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.value_type, str, length);
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
            case 0:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned char> handler(m_data.key, this);
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
        DelDBVarDataNtf& m_data;
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

		json_data.Writer().Key(u8"key", sizeof(u8"key") - 1);
		json_data.AddArray(key);

		json_data.AddIntegral(u8"value_type", value_type);

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
        
        DelDBVarDataNtf::Handler h(*this, nullptr);
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

struct LoadMailDataReq:TProtocol<LoadMailDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 39;

	unsigned int              index; //gs索引
	unsigned long long        recv_guid; //收件人GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(recv_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(recv_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		recv_guid = 0;

	}
	LoadMailDataReq& operator= (const LoadMailDataReq& src)
	{
		index = src.index;

		recv_guid = src.recv_guid;

		return *this;
	}
	bool operator==(const LoadMailDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (recv_guid != src.recv_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadMailDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadMailDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.recv_guid, str, length);
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
        LoadMailDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"recv_guid", recv_guid);

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
        
        LoadMailDataReq::Handler h(*this, nullptr);
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

struct LoadMailDataAck:TProtocol<LoadMailDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 40;

	unsigned int              index; //gs索引
	unsigned long long        recv_guid; //收件人GUID
	DataArray<DBMailData, unsigned short> datas; //邮件数据
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(recv_guid);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(recv_guid);

		net_data.AddArray(datas);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		if(!net_data.DelArray(datas))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		recv_guid = 0;

		datas.clear();

		errcode = 0;

		errmsg[0] = 0;

	}
	LoadMailDataAck& operator= (const LoadMailDataAck& src)
	{
		index = src.index;

		recv_guid = src.recv_guid;

		datas = src.datas;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const LoadMailDataAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (recv_guid != src.recv_guid)
		{
			return false;
		}

		if (datas != src.datas)
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
	bool operator != (const LoadMailDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadMailDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.recv_guid, str, length);
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
        LoadMailDataAck& m_data;
        DataArrayHandler<DBMailData, unsigned short, DBMailData::Handler> m_datas_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"recv_guid", recv_guid);

		json_data.Writer().Key(u8"datas", sizeof(u8"datas") - 1);
		json_data.AddArray(datas);

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
        
        LoadMailDataAck::Handler h(*this, nullptr);
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

struct AddMailDataNtf:TProtocol<AddMailDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 41;

	DBMailData                data; //邮件数据
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
	AddMailDataNtf& operator= (const AddMailDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddMailDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddMailDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddMailDataNtf& data, JsonHandler* parent = nullptr)
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
        AddMailDataNtf& m_data;
        DBMailData::Handler m_data_handler;
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
        
        AddMailDataNtf::Handler h(*this, nullptr);
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

struct DelMailDataNtf:TProtocol<DelMailDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 42;

	unsigned long long        mail_guid; //邮件GUID
	unsigned long long        recv_guid; //收件人GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(mail_guid);

		net_data.AddIntegral(recv_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(mail_guid);

		net_data.AddIntegral(recv_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(mail_guid))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(mail_guid))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		mail_guid = 0;

		recv_guid = 0;

	}
	DelMailDataNtf& operator= (const DelMailDataNtf& src)
	{
		mail_guid = src.mail_guid;

		recv_guid = src.recv_guid;

		return *this;
	}
	bool operator==(const DelMailDataNtf& src) const
	{
		if (mail_guid != src.mail_guid)
		{
			return false;
		}

		if (recv_guid != src.recv_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelMailDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelMailDataNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.mail_guid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.recv_guid, str, length);
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
        DelMailDataNtf& m_data;
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

		json_data.AddIntegral(u8"mail_guid", mail_guid);

		json_data.AddIntegral(u8"recv_guid", recv_guid);

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
        
        DelMailDataNtf::Handler h(*this, nullptr);
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

struct UpdateMailDataNtf:TProtocol<UpdateMailDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 43;

	DBMailData                data; //邮件数据
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
	UpdateMailDataNtf& operator= (const UpdateMailDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const UpdateMailDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateMailDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateMailDataNtf& data, JsonHandler* parent = nullptr)
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
        UpdateMailDataNtf& m_data;
        DBMailData::Handler m_data_handler;
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
        
        UpdateMailDataNtf::Handler h(*this, nullptr);
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

struct LoadRanklistDataReq:TProtocol<LoadRanklistDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 44;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadRanklistDataReq& operator= (const LoadRanklistDataReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadRanklistDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadRanklistDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadRanklistDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadRanklistDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadRanklistDataReq::Handler h(*this, nullptr);
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

struct LoadRanklistDataAck:TProtocol<LoadRanklistDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 45;

	unsigned int              index; //gs索引
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	DataArray<DBRanklistData, unsigned int> data; //排行榜数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		net_data.AddArray(data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		net_data.AddArray(data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		if(!net_data.DelArray(data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		if(!net_data.DelArray(data))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		errcode = 0;

		errmsg[0] = 0;

		data.clear();

	}
	LoadRanklistDataAck& operator= (const LoadRanklistDataAck& src)
	{
		index = src.index;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		data = src.data;

		return *this;
	}
	bool operator==(const LoadRanklistDataAck& src) const
	{
		if (index != src.index)
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

		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadRanklistDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadRanklistDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
            switch (m_state)
            {
            case 3:
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

            if (!m_data_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoadRanklistDataAck& m_data;
        DataArrayHandler<DBRanklistData, unsigned int, DBRanklistData::Handler> m_data_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"errcode", errcode);

		json_data.AddString(u8"errmsg", errmsg, sizeof(errmsg));

		json_data.Writer().Key(u8"data", sizeof(u8"data") - 1);
		json_data.AddArray(data);

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
        
        LoadRanklistDataAck::Handler h(*this, nullptr);
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

struct AddRanklistDataNtf:TProtocol<AddRanklistDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 46;

	DBRanklistData            data; //排行榜数据
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
	AddRanklistDataNtf& operator= (const AddRanklistDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddRanklistDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddRanklistDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddRanklistDataNtf& data, JsonHandler* parent = nullptr)
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
        AddRanklistDataNtf& m_data;
        DBRanklistData::Handler m_data_handler;
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
        
        AddRanklistDataNtf::Handler h(*this, nullptr);
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

struct DelRanklistDataNtf:TProtocol<DelRanklistDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 47;

	unsigned long long        object_guid; //对象GUID
	int                       rank_type; //排行榜类型
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(object_guid);

		net_data.AddIntegral(rank_type);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(object_guid);

		net_data.AddIntegral(rank_type);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(object_guid))
			return false;

		if (!net_data.DelIntegral(rank_type))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(object_guid))
			return false;

		if (!net_data.DelIntegral(rank_type))
			return false;

		return true;
	}
	void Reset(void)
	{
		object_guid = 0;

		rank_type = 0;

	}
	DelRanklistDataNtf& operator= (const DelRanklistDataNtf& src)
	{
		object_guid = src.object_guid;

		rank_type = src.rank_type;

		return *this;
	}
	bool operator==(const DelRanklistDataNtf& src) const
	{
		if (object_guid != src.object_guid)
		{
			return false;
		}

		if (rank_type != src.rank_type)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelRanklistDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelRanklistDataNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.object_guid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rank_type, str, length);
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
        DelRanklistDataNtf& m_data;
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

		json_data.AddIntegral(u8"object_guid", object_guid);

		json_data.AddIntegral(u8"rank_type", rank_type);

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
        
        DelRanklistDataNtf::Handler h(*this, nullptr);
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

struct UpdateRanklistDataNtf:TProtocol<UpdateRanklistDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 48;

	DBRanklistData            data; //邮件数据
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
	UpdateRanklistDataNtf& operator= (const UpdateRanklistDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const UpdateRanklistDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateRanklistDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateRanklistDataNtf& data, JsonHandler* parent = nullptr)
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
        UpdateRanklistDataNtf& m_data;
        DBRanklistData::Handler m_data_handler;
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
        
        UpdateRanklistDataNtf::Handler h(*this, nullptr);
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

struct BillInMockReq:TProtocol<BillInMockReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 49;

	unsigned int              bill_id; //订单ID
	unsigned int              user_id; //用户ID
	int                       added_yb; //用户充值元宝数
	int                       award_yb; //用户奖励元宝数
	char                      user[20]; //用户名
	char                      desc[256]; //充值描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(bill_id);

		net_data.AddIntegral(user_id);

		net_data.AddIntegral(added_yb);

		net_data.AddIntegral(award_yb);

		net_data.AddString(user, sizeof(user));

		net_data.AddString(desc, sizeof(desc));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(bill_id);

		net_data.AddIntegral(user_id);

		net_data.AddIntegral(added_yb);

		net_data.AddIntegral(award_yb);

		net_data.AddString(user, sizeof(user));

		net_data.AddString(desc, sizeof(desc));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(bill_id))
			return false;

		if (!net_data.DelIntegral(user_id))
			return false;

		if (!net_data.DelIntegral(added_yb))
			return false;

		if (!net_data.DelIntegral(award_yb))
			return false;

		if (!net_data.DelString(user, sizeof(user)))
			return false;

		if (!net_data.DelString(desc, sizeof(desc)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(bill_id))
			return false;

		if (!net_data.DelIntegral(user_id))
			return false;

		if (!net_data.DelIntegral(added_yb))
			return false;

		if (!net_data.DelIntegral(award_yb))
			return false;

		if (!net_data.DelString(user, sizeof(user)))
			return false;

		if (!net_data.DelString(desc, sizeof(desc)))
			return false;

		return true;
	}
	void Reset(void)
	{
		bill_id = 0;

		user_id = 0;

		added_yb = 0;

		award_yb = 0;

		user[0] = 0;

		desc[0] = 0;

	}
	BillInMockReq& operator= (const BillInMockReq& src)
	{
		bill_id = src.bill_id;

		user_id = src.user_id;

		added_yb = src.added_yb;

		award_yb = src.award_yb;

		{
			size_t str_len = strnlen(src.user, sizeof(user) - 1);
			memcpy(user, src.user, str_len + 1);
			user[sizeof(user) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.desc, sizeof(desc) - 1);
			memcpy(desc, src.desc, str_len + 1);
			desc[sizeof(desc) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const BillInMockReq& src) const
	{
		if (bill_id != src.bill_id)
		{
			return false;
		}

		if (user_id != src.user_id)
		{
			return false;
		}

		if (added_yb != src.added_yb)
		{
			return false;
		}

		if (award_yb != src.award_yb)
		{
			return false;
		}

		if (strncmp(user, src.user, sizeof(user)-1))
		{
			return false;
		}

		if (strncmp(desc, src.desc, sizeof(desc)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const BillInMockReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(BillInMockReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.bill_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.user_id, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.added_yb, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.award_yb, str, length);
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
                JsonDelString(m_data.user, str, length);
            }
            break;
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
        BillInMockReq& m_data;
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

		json_data.AddIntegral(u8"bill_id", bill_id);

		json_data.AddIntegral(u8"user_id", user_id);

		json_data.AddIntegral(u8"added_yb", added_yb);

		json_data.AddIntegral(u8"award_yb", award_yb);

		json_data.AddString(u8"user", user, sizeof(user));

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
        
        BillInMockReq::Handler h(*this, nullptr);
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

struct AddPasturePetDataNtf:TProtocol<AddPasturePetDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 50;

	PasturePetData            data; //牧场宠物数据
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
	AddPasturePetDataNtf& operator= (const AddPasturePetDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddPasturePetDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddPasturePetDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddPasturePetDataNtf& data, JsonHandler* parent = nullptr)
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
        AddPasturePetDataNtf& m_data;
        PasturePetData::Handler m_data_handler;
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
        
        AddPasturePetDataNtf::Handler h(*this, nullptr);
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

struct LoadPasturePetDataReq:TProtocol<LoadPasturePetDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 51;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadPasturePetDataReq& operator= (const LoadPasturePetDataReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadPasturePetDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadPasturePetDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadPasturePetDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadPasturePetDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadPasturePetDataReq::Handler h(*this, nullptr);
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

struct LoadPasturePetDataAck:TProtocol<LoadPasturePetDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 52;

	unsigned int              index; //gs索引
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	DataArray<PasturePetData, unsigned int> data; //牧场宠物
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		net_data.AddArray(data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		net_data.AddArray(data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		if(!net_data.DelArray(data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		if(!net_data.DelArray(data))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		errcode = 0;

		errmsg[0] = 0;

		data.clear();

	}
	LoadPasturePetDataAck& operator= (const LoadPasturePetDataAck& src)
	{
		index = src.index;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		data = src.data;

		return *this;
	}
	bool operator==(const LoadPasturePetDataAck& src) const
	{
		if (index != src.index)
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

		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadPasturePetDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadPasturePetDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
            switch (m_state)
            {
            case 3:
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

            if (!m_data_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoadPasturePetDataAck& m_data;
        DataArrayHandler<PasturePetData, unsigned int, PasturePetData::Handler> m_data_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"errcode", errcode);

		json_data.AddString(u8"errmsg", errmsg, sizeof(errmsg));

		json_data.Writer().Key(u8"data", sizeof(u8"data") - 1);
		json_data.AddArray(data);

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
        
        LoadPasturePetDataAck::Handler h(*this, nullptr);
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

struct FregmentPkg:TProtocol<FregmentPkg>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 53;

	unsigned char             frag_mark; //分片标志 0=开始 1=中间 2=结束
	DataArray<unsigned char, unsigned int> frag_data; //包装数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(frag_mark);

		net_data.AddArray(frag_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(frag_mark);

		net_data.AddArray(frag_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(frag_mark))
			return false;

		if(!net_data.DelArray(frag_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(frag_mark))
			return false;

		if(!net_data.DelArray(frag_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		frag_mark = 0;

		frag_data.clear();

	}
	FregmentPkg& operator= (const FregmentPkg& src)
	{
		frag_mark = src.frag_mark;

		frag_data = src.frag_data;

		return *this;
	}
	bool operator==(const FregmentPkg& src) const
	{
		if (frag_mark != src.frag_mark)
		{
			return false;
		}

		if (frag_data != src.frag_data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const FregmentPkg& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(FregmentPkg& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.frag_mark, str, length);
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
                DataArrayHandler<unsigned char, unsigned int> handler(m_data.frag_data, this);
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
        FregmentPkg& m_data;
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

		json_data.AddIntegral(u8"frag_mark", frag_mark);

		json_data.Writer().Key(u8"frag_data", sizeof(u8"frag_data") - 1);
		json_data.AddArray(frag_data);

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
        
        FregmentPkg::Handler h(*this, nullptr);
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

struct DelPasturePetDataNtf:TProtocol<DelPasturePetDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 54;

	unsigned long long        pet_guid; //宠物GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(pet_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(pet_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(pet_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(pet_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		pet_guid = 0;

	}
	DelPasturePetDataNtf& operator= (const DelPasturePetDataNtf& src)
	{
		pet_guid = src.pet_guid;

		return *this;
	}
	bool operator==(const DelPasturePetDataNtf& src) const
	{
		if (pet_guid != src.pet_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelPasturePetDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelPasturePetDataNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.pet_guid, str, length);
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
        DelPasturePetDataNtf& m_data;
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

		json_data.AddIntegral(u8"pet_guid", pet_guid);

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
        
        DelPasturePetDataNtf::Handler h(*this, nullptr);
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

struct UpdatePasturePetDataNtf:TProtocol<UpdatePasturePetDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 55;

	PasturePetData            data; //牧场宠物数据
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
	UpdatePasturePetDataNtf& operator= (const UpdatePasturePetDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const UpdatePasturePetDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdatePasturePetDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdatePasturePetDataNtf& data, JsonHandler* parent = nullptr)
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
        UpdatePasturePetDataNtf& m_data;
        PasturePetData::Handler m_data_handler;
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
        
        UpdatePasturePetDataNtf::Handler h(*this, nullptr);
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

struct LoadContactInfoReq:TProtocol<LoadContactInfoReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 56;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadContactInfoReq& operator= (const LoadContactInfoReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadContactInfoReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadContactInfoReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadContactInfoReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadContactInfoReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadContactInfoReq::Handler h(*this, nullptr);
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

struct LoadLadderDataReq:TProtocol<LoadLadderDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 57;

	unsigned int              index; //gs索引
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

	}
	LoadLadderDataReq& operator= (const LoadLadderDataReq& src)
	{
		index = src.index;

		return *this;
	}
	bool operator==(const LoadLadderDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadLadderDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadLadderDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
        LoadLadderDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

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
        
        LoadLadderDataReq::Handler h(*this, nullptr);
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

struct LoadLadderDataAck:TProtocol<LoadLadderDataAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 58;

	unsigned int              index; //gs索引
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	DataArray<LadderData, unsigned int> data; //天梯数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		net_data.AddArray(data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		net_data.AddArray(data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		if(!net_data.DelArray(data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		if(!net_data.DelArray(data))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		errcode = 0;

		errmsg[0] = 0;

		data.clear();

	}
	LoadLadderDataAck& operator= (const LoadLadderDataAck& src)
	{
		index = src.index;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		data = src.data;

		return *this;
	}
	bool operator==(const LoadLadderDataAck& src) const
	{
		if (index != src.index)
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

		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadLadderDataAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadLadderDataAck& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
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
            switch (m_state)
            {
            case 3:
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

            if (!m_data_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoadLadderDataAck& m_data;
        DataArrayHandler<LadderData, unsigned int, LadderData::Handler> m_data_handler;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"errcode", errcode);

		json_data.AddString(u8"errmsg", errmsg, sizeof(errmsg));

		json_data.Writer().Key(u8"data", sizeof(u8"data") - 1);
		json_data.AddArray(data);

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
        
        LoadLadderDataAck::Handler h(*this, nullptr);
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

struct AddLadderDataNtf:TProtocol<AddLadderDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 59;

	LadderData                data; //天梯数据
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
	AddLadderDataNtf& operator= (const AddLadderDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddLadderDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddLadderDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddLadderDataNtf& data, JsonHandler* parent = nullptr)
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
        AddLadderDataNtf& m_data;
        LadderData::Handler m_data_handler;
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
        
        AddLadderDataNtf::Handler h(*this, nullptr);
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

struct DelLadderDataNtf:TProtocol<DelLadderDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 60;

	unsigned long long        player_guid; //玩家GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(player_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(player_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		player_guid = 0;

	}
	DelLadderDataNtf& operator= (const DelLadderDataNtf& src)
	{
		player_guid = src.player_guid;

		return *this;
	}
	bool operator==(const DelLadderDataNtf& src) const
	{
		if (player_guid != src.player_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelLadderDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelLadderDataNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.player_guid, str, length);
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
        DelLadderDataNtf& m_data;
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

		json_data.AddIntegral(u8"player_guid", player_guid);

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
        
        DelLadderDataNtf::Handler h(*this, nullptr);
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

struct UpdateLadderDataNtf:TProtocol<UpdateLadderDataNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 61;

	LadderData                data; //天梯数据
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
	UpdateLadderDataNtf& operator= (const UpdateLadderDataNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const UpdateLadderDataNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UpdateLadderDataNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UpdateLadderDataNtf& data, JsonHandler* parent = nullptr)
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
        UpdateLadderDataNtf& m_data;
        LadderData::Handler m_data_handler;
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
        
        UpdateLadderDataNtf::Handler h(*this, nullptr);
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

struct UnloadMailDataReq:TProtocol<UnloadMailDataReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 62;

	unsigned int              index; //gs索引
	unsigned long long        recv_guid; //收件人GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(recv_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(recv_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		recv_guid = 0;

	}
	UnloadMailDataReq& operator= (const UnloadMailDataReq& src)
	{
		index = src.index;

		recv_guid = src.recv_guid;

		return *this;
	}
	bool operator==(const UnloadMailDataReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (recv_guid != src.recv_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UnloadMailDataReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UnloadMailDataReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.recv_guid, str, length);
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
        UnloadMailDataReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"recv_guid", recv_guid);

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
        
        UnloadMailDataReq::Handler h(*this, nullptr);
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

struct LoadOfflinePlayerReq:TProtocol<LoadOfflinePlayerReq>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 63;

	unsigned int              index; //gs索引
	unsigned int              seq; //序号
	unsigned long long        target_guid; //目标GUID
	unsigned int              object_type; //对象类型
	unsigned long long        object_guid; //对象GUID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(seq);

		net_data.AddIntegral(target_guid);

		net_data.AddIntegral(object_type);

		net_data.AddIntegral(object_guid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(seq);

		net_data.AddIntegral(target_guid);

		net_data.AddIntegral(object_type);

		net_data.AddIntegral(object_guid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(seq))
			return false;

		if (!net_data.DelIntegral(target_guid))
			return false;

		if (!net_data.DelIntegral(object_type))
			return false;

		if (!net_data.DelIntegral(object_guid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(seq))
			return false;

		if (!net_data.DelIntegral(target_guid))
			return false;

		if (!net_data.DelIntegral(object_type))
			return false;

		if (!net_data.DelIntegral(object_guid))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		seq = 0;

		target_guid = 0;

		object_type = 0;

		object_guid = 0;

	}
	LoadOfflinePlayerReq& operator= (const LoadOfflinePlayerReq& src)
	{
		index = src.index;

		seq = src.seq;

		target_guid = src.target_guid;

		object_type = src.object_type;

		object_guid = src.object_guid;

		return *this;
	}
	bool operator==(const LoadOfflinePlayerReq& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (seq != src.seq)
		{
			return false;
		}

		if (target_guid != src.target_guid)
		{
			return false;
		}

		if (object_type != src.object_type)
		{
			return false;
		}

		if (object_guid != src.object_guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadOfflinePlayerReq& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadOfflinePlayerReq& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.index, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.seq, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target_guid, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.object_type, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.object_guid, str, length);
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
        LoadOfflinePlayerReq& m_data;
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

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"seq", seq);

		json_data.AddIntegral(u8"target_guid", target_guid);

		json_data.AddIntegral(u8"object_type", object_type);

		json_data.AddIntegral(u8"object_guid", object_guid);

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
        
        LoadOfflinePlayerReq::Handler h(*this, nullptr);
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

struct LoadOfflinePlayerAck:TProtocol<LoadOfflinePlayerAck>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 64;

	unsigned int              index; //gs索引
	unsigned int              seq; //序号
	unsigned long long        target_guid; //目标GUID
	unsigned int              object_type; //对象类型
	unsigned long long        object_guid; //对象GUID
	int                       errcode; //0=成功, 其他表示错误码
	char                      errmsg[256]; //错误码不为0时表示 错误消息
	PlayerData                offline_data; //离线角色数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(seq);

		net_data.AddIntegral(target_guid);

		net_data.AddIntegral(object_type);

		net_data.AddIntegral(object_guid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		offline_data.EnCode(net_data);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(index);

		net_data.AddIntegral(seq);

		net_data.AddIntegral(target_guid);

		net_data.AddIntegral(object_type);

		net_data.AddIntegral(object_guid);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		offline_data.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(seq))
			return false;

		if (!net_data.DelIntegral(target_guid))
			return false;

		if (!net_data.DelIntegral(object_type))
			return false;

		if (!net_data.DelIntegral(object_guid))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		if (!offline_data.DeCode(net_data))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(index))
			return false;

		if (!net_data.DelIntegral(seq))
			return false;

		if (!net_data.DelIntegral(target_guid))
			return false;

		if (!net_data.DelIntegral(object_type))
			return false;

		if (!net_data.DelIntegral(object_guid))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		if (!offline_data.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		seq = 0;

		target_guid = 0;

		object_type = 0;

		object_guid = 0;

		errcode = 0;

		errmsg[0] = 0;

		offline_data.Reset();

	}
	LoadOfflinePlayerAck& operator= (const LoadOfflinePlayerAck& src)
	{
		index = src.index;

		seq = src.seq;

		target_guid = src.target_guid;

		object_type = src.object_type;

		object_guid = src.object_guid;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		offline_data = src.offline_data;

		return *this;
	}
	bool operator==(const LoadOfflinePlayerAck& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (seq != src.seq)
		{
			return false;
		}

		if (target_guid != src.target_guid)
		{
			return false;
		}

		if (object_type != src.object_type)
		{
			return false;
		}

		if (object_guid != src.object_guid)
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

		if (offline_data != src.offline_data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LoadOfflinePlayerAck& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LoadOfflinePlayerAck& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_offline_data_handler(m_data.offline_data, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_offline_data_handler.ResetState();
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
                JsonDelIntegral(m_data.index, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.seq, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target_guid, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.object_type, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.object_guid, str, length);
            }
            break;
            case 5:
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
            case 6:
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
            case 7:
            {
                m_assigned_bitset.set(m_state);
                return &m_offline_data_handler;
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

            if (!m_offline_data_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LoadOfflinePlayerAck& m_data;
        PlayerData::Handler m_offline_data_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<8> m_assigned_bitset;

        static const std::bitset<8> template_assigned_bitset;
        static const char* m_key_list[8];
        static const char* m_alias_list[8];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"index", index);

		json_data.AddIntegral(u8"seq", seq);

		json_data.AddIntegral(u8"target_guid", target_guid);

		json_data.AddIntegral(u8"object_type", object_type);

		json_data.AddIntegral(u8"object_guid", object_guid);

		json_data.AddIntegral(u8"errcode", errcode);

		json_data.AddString(u8"errmsg", errmsg, sizeof(errmsg));

		json_data.Writer().Key(u8"offline_data", sizeof(u8"offline_data") - 1);
		offline_data.ToJson(json_data);

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
        
        LoadOfflinePlayerAck::Handler h(*this, nullptr);
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

struct SyncViolateNtf:TProtocol<SyncViolateNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 65;

	DataArray<ViolateData, unsigned short> violates; //违禁字数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(violates);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddArray(violates);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(violates))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if(!net_data.DelArray(violates))
			return false;

		return true;
	}
	void Reset(void)
	{
		violates.clear();

	}
	SyncViolateNtf& operator= (const SyncViolateNtf& src)
	{
		violates = src.violates;

		return *this;
	}
	bool operator==(const SyncViolateNtf& src) const
	{
		if (violates != src.violates)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SyncViolateNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SyncViolateNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_violates_handler(m_data.violates, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_violates_handler.ResetState();
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
                return &m_violates_handler;
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

            if (!m_violates_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        SyncViolateNtf& m_data;
        DataArrayHandler<ViolateData, unsigned short, ViolateData::Handler> m_violates_handler;
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

		json_data.Writer().Key(u8"violates", sizeof(u8"violates") - 1);
		json_data.AddArray(violates);

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
        
        SyncViolateNtf::Handler h(*this, nullptr);
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

struct SyncForbidTalkNtf:TProtocol<SyncForbidTalkNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 66;

	DataArray<ForbidTalkData, unsigned int> forbids; //禁止聊天数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(forbids);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddArray(forbids);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(forbids))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if(!net_data.DelArray(forbids))
			return false;

		return true;
	}
	void Reset(void)
	{
		forbids.clear();

	}
	SyncForbidTalkNtf& operator= (const SyncForbidTalkNtf& src)
	{
		forbids = src.forbids;

		return *this;
	}
	bool operator==(const SyncForbidTalkNtf& src) const
	{
		if (forbids != src.forbids)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SyncForbidTalkNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SyncForbidTalkNtf& data, JsonHandler* parent = nullptr)
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
        SyncForbidTalkNtf& m_data;
        DataArrayHandler<ForbidTalkData, unsigned int, ForbidTalkData::Handler> m_forbids_handler;
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

		json_data.Writer().Key(u8"forbids", sizeof(u8"forbids") - 1);
		json_data.AddArray(forbids);

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
        
        SyncForbidTalkNtf::Handler h(*this, nullptr);
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

struct AddForbidTalkNtf:TProtocol<AddForbidTalkNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 67;

	ForbidTalkData            data; //禁止聊天数据
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
	AddForbidTalkNtf& operator= (const AddForbidTalkNtf& src)
	{
		data = src.data;

		return *this;
	}
	bool operator==(const AddForbidTalkNtf& src) const
	{
		if (data != src.data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddForbidTalkNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddForbidTalkNtf& data, JsonHandler* parent = nullptr)
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
        AddForbidTalkNtf& m_data;
        ForbidTalkData::Handler m_data_handler;
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
        
        AddForbidTalkNtf::Handler h(*this, nullptr);
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

struct DelForbidTalkNtf:TProtocol<DelForbidTalkNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 68;

	unsigned long long        guid; //角色GUID
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
	DelForbidTalkNtf& operator= (const DelForbidTalkNtf& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const DelForbidTalkNtf& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelForbidTalkNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelForbidTalkNtf& data, JsonHandler* parent = nullptr)
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
        DelForbidTalkNtf& m_data;
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
        
        DelForbidTalkNtf::Handler h(*this, nullptr);
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

struct LogLevelNtf:TProtocol<LogLevelNtf>
{
	static constexpr unsigned short module_id = 105;
	static constexpr unsigned short protocol_id = 69;

	unsigned int              level; //日志级别 0=LogSys 1=LogErr 2=LogWrn 3=LogInf 4=LogDbg
	unsigned int              enable; //0关闭 1开启
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(level);

		net_data.AddIntegral(enable);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(level);

		net_data.AddIntegral(enable);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(enable))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(enable))
			return false;

		return true;
	}
	void Reset(void)
	{
		level = 0;

		enable = 0;

	}
	LogLevelNtf& operator= (const LogLevelNtf& src)
	{
		level = src.level;

		enable = src.enable;

		return *this;
	}
	bool operator==(const LogLevelNtf& src) const
	{
		if (level != src.level)
		{
			return false;
		}

		if (enable != src.enable)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LogLevelNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LogLevelNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.enable, str, length);
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
        LogLevelNtf& m_data;
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

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"enable", enable);

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
        
        LogLevelNtf::Handler h(*this, nullptr);
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
class CGlobal: public ProtocolModule
{
public:
	CGlobal()
	{
		size_t max_protocol_size = 0;
		if (sizeof(NewGSNtf) > max_protocol_size)
			max_protocol_size = sizeof(NewGSNtf);

		if (sizeof(TerminateNtf) > max_protocol_size)
			max_protocol_size = sizeof(TerminateNtf);

		if (sizeof(ErrorNtf) > max_protocol_size)
			max_protocol_size = sizeof(ErrorNtf);

		if (sizeof(ServerStopNtf) > max_protocol_size)
			max_protocol_size = sizeof(ServerStopNtf);

		if (sizeof(ServerConfigNtf) > max_protocol_size)
			max_protocol_size = sizeof(ServerConfigNtf);

		if (sizeof(DBWrapperPkg) > max_protocol_size)
			max_protocol_size = sizeof(DBWrapperPkg);

		if (sizeof(GSWrapperPkg) > max_protocol_size)
			max_protocol_size = sizeof(GSWrapperPkg);

		if (sizeof(LoadAuctionObjectDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadAuctionObjectDataReq);

		if (sizeof(LoadAuctionObjectDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadAuctionObjectDataAck);

		if (sizeof(AddAuctionObjectDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddAuctionObjectDataNtf);

		if (sizeof(DelAuctionObjectDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelAuctionObjectDataNtf);

		if (sizeof(UpdateAuctionObjectDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateAuctionObjectDataNtf);

		if (sizeof(LoadAuctionCookieDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadAuctionCookieDataReq);

		if (sizeof(LoadAuctionCookieDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadAuctionCookieDataAck);

		if (sizeof(DuplicateAuctionCookieDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DuplicateAuctionCookieDataNtf);

		if (sizeof(LoadGuildDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadGuildDataReq);

		if (sizeof(LoadGuildDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadGuildDataAck);

		if (sizeof(LoadGuildMemberDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadGuildMemberDataAck);

		if (sizeof(LoadGuildApplicantDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadGuildApplicantDataAck);

		if (sizeof(AddGuildDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddGuildDataNtf);

		if (sizeof(DelGuildDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelGuildDataNtf);

		if (sizeof(UpdateGuildDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateGuildDataNtf);

		if (sizeof(AddGuildMemberDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddGuildMemberDataNtf);

		if (sizeof(DelGuildMemberDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelGuildMemberDataNtf);

		if (sizeof(UpdateGuildMemberDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateGuildMemberDataNtf);

		if (sizeof(AddGuildApplicantDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddGuildApplicantDataNtf);

		if (sizeof(DelGuildApplicantDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelGuildApplicantDataNtf);

		if (sizeof(UpdateGuildApplicantDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateGuildApplicantDataNtf);

		if (sizeof(LoadContactDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadContactDataReq);

		if (sizeof(LoadContactInfoDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadContactInfoDataAck);

		if (sizeof(LoadContactDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadContactDataAck);

		if (sizeof(AddContactDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddContactDataNtf);

		if (sizeof(DelContactDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelContactDataNtf);

		if (sizeof(UpdateContactDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateContactDataNtf);

		if (sizeof(LoadDBVarDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadDBVarDataReq);

		if (sizeof(LoadDBVarDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadDBVarDataAck);

		if (sizeof(AddDBVarDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddDBVarDataNtf);

		if (sizeof(DelDBVarDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelDBVarDataNtf);

		if (sizeof(LoadMailDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadMailDataReq);

		if (sizeof(LoadMailDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadMailDataAck);

		if (sizeof(AddMailDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddMailDataNtf);

		if (sizeof(DelMailDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelMailDataNtf);

		if (sizeof(UpdateMailDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateMailDataNtf);

		if (sizeof(LoadRanklistDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadRanklistDataReq);

		if (sizeof(LoadRanklistDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadRanklistDataAck);

		if (sizeof(AddRanklistDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddRanklistDataNtf);

		if (sizeof(DelRanklistDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelRanklistDataNtf);

		if (sizeof(UpdateRanklistDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateRanklistDataNtf);

		if (sizeof(BillInMockReq) > max_protocol_size)
			max_protocol_size = sizeof(BillInMockReq);

		if (sizeof(AddPasturePetDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddPasturePetDataNtf);

		if (sizeof(LoadPasturePetDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadPasturePetDataReq);

		if (sizeof(LoadPasturePetDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadPasturePetDataAck);

		if (sizeof(FregmentPkg) > max_protocol_size)
			max_protocol_size = sizeof(FregmentPkg);

		if (sizeof(DelPasturePetDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelPasturePetDataNtf);

		if (sizeof(UpdatePasturePetDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdatePasturePetDataNtf);

		if (sizeof(LoadContactInfoReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadContactInfoReq);

		if (sizeof(LoadLadderDataReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadLadderDataReq);

		if (sizeof(LoadLadderDataAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadLadderDataAck);

		if (sizeof(AddLadderDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddLadderDataNtf);

		if (sizeof(DelLadderDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelLadderDataNtf);

		if (sizeof(UpdateLadderDataNtf) > max_protocol_size)
			max_protocol_size = sizeof(UpdateLadderDataNtf);

		if (sizeof(UnloadMailDataReq) > max_protocol_size)
			max_protocol_size = sizeof(UnloadMailDataReq);

		if (sizeof(LoadOfflinePlayerReq) > max_protocol_size)
			max_protocol_size = sizeof(LoadOfflinePlayerReq);

		if (sizeof(LoadOfflinePlayerAck) > max_protocol_size)
			max_protocol_size = sizeof(LoadOfflinePlayerAck);

		if (sizeof(SyncViolateNtf) > max_protocol_size)
			max_protocol_size = sizeof(SyncViolateNtf);

		if (sizeof(SyncForbidTalkNtf) > max_protocol_size)
			max_protocol_size = sizeof(SyncForbidTalkNtf);

		if (sizeof(AddForbidTalkNtf) > max_protocol_size)
			max_protocol_size = sizeof(AddForbidTalkNtf);

		if (sizeof(DelForbidTalkNtf) > max_protocol_size)
			max_protocol_size = sizeof(DelForbidTalkNtf);

		if (sizeof(LogLevelNtf) > max_protocol_size)
			max_protocol_size = sizeof(LogLevelNtf);

		m_protocol_buffer = S_MALLOC(max_protocol_size);
	}

	virtual ~CGlobal()
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
		case NewGSNtf::protocol_id:
		{
			return NewGSNtf::Name();
		}
		break;
		case TerminateNtf::protocol_id:
		{
			return TerminateNtf::Name();
		}
		break;
		case ErrorNtf::protocol_id:
		{
			return ErrorNtf::Name();
		}
		break;
		case ServerStopNtf::protocol_id:
		{
			return ServerStopNtf::Name();
		}
		break;
		case ServerConfigNtf::protocol_id:
		{
			return ServerConfigNtf::Name();
		}
		break;
		case DBWrapperPkg::protocol_id:
		{
			return DBWrapperPkg::Name();
		}
		break;
		case GSWrapperPkg::protocol_id:
		{
			return GSWrapperPkg::Name();
		}
		break;
		case LoadAuctionObjectDataReq::protocol_id:
		{
			return LoadAuctionObjectDataReq::Name();
		}
		break;
		case LoadAuctionObjectDataAck::protocol_id:
		{
			return LoadAuctionObjectDataAck::Name();
		}
		break;
		case AddAuctionObjectDataNtf::protocol_id:
		{
			return AddAuctionObjectDataNtf::Name();
		}
		break;
		case DelAuctionObjectDataNtf::protocol_id:
		{
			return DelAuctionObjectDataNtf::Name();
		}
		break;
		case UpdateAuctionObjectDataNtf::protocol_id:
		{
			return UpdateAuctionObjectDataNtf::Name();
		}
		break;
		case LoadAuctionCookieDataReq::protocol_id:
		{
			return LoadAuctionCookieDataReq::Name();
		}
		break;
		case LoadAuctionCookieDataAck::protocol_id:
		{
			return LoadAuctionCookieDataAck::Name();
		}
		break;
		case DuplicateAuctionCookieDataNtf::protocol_id:
		{
			return DuplicateAuctionCookieDataNtf::Name();
		}
		break;
		case LoadGuildDataReq::protocol_id:
		{
			return LoadGuildDataReq::Name();
		}
		break;
		case LoadGuildDataAck::protocol_id:
		{
			return LoadGuildDataAck::Name();
		}
		break;
		case LoadGuildMemberDataAck::protocol_id:
		{
			return LoadGuildMemberDataAck::Name();
		}
		break;
		case LoadGuildApplicantDataAck::protocol_id:
		{
			return LoadGuildApplicantDataAck::Name();
		}
		break;
		case AddGuildDataNtf::protocol_id:
		{
			return AddGuildDataNtf::Name();
		}
		break;
		case DelGuildDataNtf::protocol_id:
		{
			return DelGuildDataNtf::Name();
		}
		break;
		case UpdateGuildDataNtf::protocol_id:
		{
			return UpdateGuildDataNtf::Name();
		}
		break;
		case AddGuildMemberDataNtf::protocol_id:
		{
			return AddGuildMemberDataNtf::Name();
		}
		break;
		case DelGuildMemberDataNtf::protocol_id:
		{
			return DelGuildMemberDataNtf::Name();
		}
		break;
		case UpdateGuildMemberDataNtf::protocol_id:
		{
			return UpdateGuildMemberDataNtf::Name();
		}
		break;
		case AddGuildApplicantDataNtf::protocol_id:
		{
			return AddGuildApplicantDataNtf::Name();
		}
		break;
		case DelGuildApplicantDataNtf::protocol_id:
		{
			return DelGuildApplicantDataNtf::Name();
		}
		break;
		case UpdateGuildApplicantDataNtf::protocol_id:
		{
			return UpdateGuildApplicantDataNtf::Name();
		}
		break;
		case LoadContactDataReq::protocol_id:
		{
			return LoadContactDataReq::Name();
		}
		break;
		case LoadContactInfoDataAck::protocol_id:
		{
			return LoadContactInfoDataAck::Name();
		}
		break;
		case LoadContactDataAck::protocol_id:
		{
			return LoadContactDataAck::Name();
		}
		break;
		case AddContactDataNtf::protocol_id:
		{
			return AddContactDataNtf::Name();
		}
		break;
		case DelContactDataNtf::protocol_id:
		{
			return DelContactDataNtf::Name();
		}
		break;
		case UpdateContactDataNtf::protocol_id:
		{
			return UpdateContactDataNtf::Name();
		}
		break;
		case LoadDBVarDataReq::protocol_id:
		{
			return LoadDBVarDataReq::Name();
		}
		break;
		case LoadDBVarDataAck::protocol_id:
		{
			return LoadDBVarDataAck::Name();
		}
		break;
		case AddDBVarDataNtf::protocol_id:
		{
			return AddDBVarDataNtf::Name();
		}
		break;
		case DelDBVarDataNtf::protocol_id:
		{
			return DelDBVarDataNtf::Name();
		}
		break;
		case LoadMailDataReq::protocol_id:
		{
			return LoadMailDataReq::Name();
		}
		break;
		case LoadMailDataAck::protocol_id:
		{
			return LoadMailDataAck::Name();
		}
		break;
		case AddMailDataNtf::protocol_id:
		{
			return AddMailDataNtf::Name();
		}
		break;
		case DelMailDataNtf::protocol_id:
		{
			return DelMailDataNtf::Name();
		}
		break;
		case UpdateMailDataNtf::protocol_id:
		{
			return UpdateMailDataNtf::Name();
		}
		break;
		case LoadRanklistDataReq::protocol_id:
		{
			return LoadRanklistDataReq::Name();
		}
		break;
		case LoadRanklistDataAck::protocol_id:
		{
			return LoadRanklistDataAck::Name();
		}
		break;
		case AddRanklistDataNtf::protocol_id:
		{
			return AddRanklistDataNtf::Name();
		}
		break;
		case DelRanklistDataNtf::protocol_id:
		{
			return DelRanklistDataNtf::Name();
		}
		break;
		case UpdateRanklistDataNtf::protocol_id:
		{
			return UpdateRanklistDataNtf::Name();
		}
		break;
		case BillInMockReq::protocol_id:
		{
			return BillInMockReq::Name();
		}
		break;
		case AddPasturePetDataNtf::protocol_id:
		{
			return AddPasturePetDataNtf::Name();
		}
		break;
		case LoadPasturePetDataReq::protocol_id:
		{
			return LoadPasturePetDataReq::Name();
		}
		break;
		case LoadPasturePetDataAck::protocol_id:
		{
			return LoadPasturePetDataAck::Name();
		}
		break;
		case FregmentPkg::protocol_id:
		{
			return FregmentPkg::Name();
		}
		break;
		case DelPasturePetDataNtf::protocol_id:
		{
			return DelPasturePetDataNtf::Name();
		}
		break;
		case UpdatePasturePetDataNtf::protocol_id:
		{
			return UpdatePasturePetDataNtf::Name();
		}
		break;
		case LoadContactInfoReq::protocol_id:
		{
			return LoadContactInfoReq::Name();
		}
		break;
		case LoadLadderDataReq::protocol_id:
		{
			return LoadLadderDataReq::Name();
		}
		break;
		case LoadLadderDataAck::protocol_id:
		{
			return LoadLadderDataAck::Name();
		}
		break;
		case AddLadderDataNtf::protocol_id:
		{
			return AddLadderDataNtf::Name();
		}
		break;
		case DelLadderDataNtf::protocol_id:
		{
			return DelLadderDataNtf::Name();
		}
		break;
		case UpdateLadderDataNtf::protocol_id:
		{
			return UpdateLadderDataNtf::Name();
		}
		break;
		case UnloadMailDataReq::protocol_id:
		{
			return UnloadMailDataReq::Name();
		}
		break;
		case LoadOfflinePlayerReq::protocol_id:
		{
			return LoadOfflinePlayerReq::Name();
		}
		break;
		case LoadOfflinePlayerAck::protocol_id:
		{
			return LoadOfflinePlayerAck::Name();
		}
		break;
		case SyncViolateNtf::protocol_id:
		{
			return SyncViolateNtf::Name();
		}
		break;
		case SyncForbidTalkNtf::protocol_id:
		{
			return SyncForbidTalkNtf::Name();
		}
		break;
		case AddForbidTalkNtf::protocol_id:
		{
			return AddForbidTalkNtf::Name();
		}
		break;
		case DelForbidTalkNtf::protocol_id:
		{
			return DelForbidTalkNtf::Name();
		}
		break;
		case LogLevelNtf::protocol_id:
		{
			return LogLevelNtf::Name();
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

		if (m_id != 105)
		{
			net_data.Reset(net_data_pos);
			return false;
		}

		switch(p_id)
		{
		case NewGSNtf::protocol_id:
		{
			NewGSNtf* proto = new(m_protocol_buffer) NewGSNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_NewGSNtf(*proto);
				proto->~NewGSNtf();
				return true;
			}
			else
			{
				proto->~NewGSNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case TerminateNtf::protocol_id:
		{
			TerminateNtf* proto = new(m_protocol_buffer) TerminateNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_TerminateNtf(*proto);
				proto->~TerminateNtf();
				return true;
			}
			else
			{
				proto->~TerminateNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ErrorNtf::protocol_id:
		{
			ErrorNtf* proto = new(m_protocol_buffer) ErrorNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ErrorNtf(*proto);
				proto->~ErrorNtf();
				return true;
			}
			else
			{
				proto->~ErrorNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ServerStopNtf::protocol_id:
		{
			ServerStopNtf* proto = new(m_protocol_buffer) ServerStopNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ServerStopNtf(*proto);
				proto->~ServerStopNtf();
				return true;
			}
			else
			{
				proto->~ServerStopNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case ServerConfigNtf::protocol_id:
		{
			ServerConfigNtf* proto = new(m_protocol_buffer) ServerConfigNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ServerConfigNtf(*proto);
				proto->~ServerConfigNtf();
				return true;
			}
			else
			{
				proto->~ServerConfigNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DBWrapperPkg::protocol_id:
		{
			DBWrapperPkg* proto = new(m_protocol_buffer) DBWrapperPkg();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DBWrapperPkg(*proto);
				proto->~DBWrapperPkg();
				return true;
			}
			else
			{
				proto->~DBWrapperPkg();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case GSWrapperPkg::protocol_id:
		{
			GSWrapperPkg* proto = new(m_protocol_buffer) GSWrapperPkg();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_GSWrapperPkg(*proto);
				proto->~GSWrapperPkg();
				return true;
			}
			else
			{
				proto->~GSWrapperPkg();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadAuctionObjectDataReq::protocol_id:
		{
			LoadAuctionObjectDataReq* proto = new(m_protocol_buffer) LoadAuctionObjectDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadAuctionObjectDataReq(*proto);
				proto->~LoadAuctionObjectDataReq();
				return true;
			}
			else
			{
				proto->~LoadAuctionObjectDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadAuctionObjectDataAck::protocol_id:
		{
			LoadAuctionObjectDataAck* proto = new(m_protocol_buffer) LoadAuctionObjectDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadAuctionObjectDataAck(*proto);
				proto->~LoadAuctionObjectDataAck();
				return true;
			}
			else
			{
				proto->~LoadAuctionObjectDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddAuctionObjectDataNtf::protocol_id:
		{
			AddAuctionObjectDataNtf* proto = new(m_protocol_buffer) AddAuctionObjectDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddAuctionObjectDataNtf(*proto);
				proto->~AddAuctionObjectDataNtf();
				return true;
			}
			else
			{
				proto->~AddAuctionObjectDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelAuctionObjectDataNtf::protocol_id:
		{
			DelAuctionObjectDataNtf* proto = new(m_protocol_buffer) DelAuctionObjectDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelAuctionObjectDataNtf(*proto);
				proto->~DelAuctionObjectDataNtf();
				return true;
			}
			else
			{
				proto->~DelAuctionObjectDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateAuctionObjectDataNtf::protocol_id:
		{
			UpdateAuctionObjectDataNtf* proto = new(m_protocol_buffer) UpdateAuctionObjectDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateAuctionObjectDataNtf(*proto);
				proto->~UpdateAuctionObjectDataNtf();
				return true;
			}
			else
			{
				proto->~UpdateAuctionObjectDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadAuctionCookieDataReq::protocol_id:
		{
			LoadAuctionCookieDataReq* proto = new(m_protocol_buffer) LoadAuctionCookieDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadAuctionCookieDataReq(*proto);
				proto->~LoadAuctionCookieDataReq();
				return true;
			}
			else
			{
				proto->~LoadAuctionCookieDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadAuctionCookieDataAck::protocol_id:
		{
			LoadAuctionCookieDataAck* proto = new(m_protocol_buffer) LoadAuctionCookieDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadAuctionCookieDataAck(*proto);
				proto->~LoadAuctionCookieDataAck();
				return true;
			}
			else
			{
				proto->~LoadAuctionCookieDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DuplicateAuctionCookieDataNtf::protocol_id:
		{
			DuplicateAuctionCookieDataNtf* proto = new(m_protocol_buffer) DuplicateAuctionCookieDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DuplicateAuctionCookieDataNtf(*proto);
				proto->~DuplicateAuctionCookieDataNtf();
				return true;
			}
			else
			{
				proto->~DuplicateAuctionCookieDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadGuildDataReq::protocol_id:
		{
			LoadGuildDataReq* proto = new(m_protocol_buffer) LoadGuildDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadGuildDataReq(*proto);
				proto->~LoadGuildDataReq();
				return true;
			}
			else
			{
				proto->~LoadGuildDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadGuildDataAck::protocol_id:
		{
			LoadGuildDataAck* proto = new(m_protocol_buffer) LoadGuildDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadGuildDataAck(*proto);
				proto->~LoadGuildDataAck();
				return true;
			}
			else
			{
				proto->~LoadGuildDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadGuildMemberDataAck::protocol_id:
		{
			LoadGuildMemberDataAck* proto = new(m_protocol_buffer) LoadGuildMemberDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadGuildMemberDataAck(*proto);
				proto->~LoadGuildMemberDataAck();
				return true;
			}
			else
			{
				proto->~LoadGuildMemberDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadGuildApplicantDataAck::protocol_id:
		{
			LoadGuildApplicantDataAck* proto = new(m_protocol_buffer) LoadGuildApplicantDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadGuildApplicantDataAck(*proto);
				proto->~LoadGuildApplicantDataAck();
				return true;
			}
			else
			{
				proto->~LoadGuildApplicantDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddGuildDataNtf::protocol_id:
		{
			AddGuildDataNtf* proto = new(m_protocol_buffer) AddGuildDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddGuildDataNtf(*proto);
				proto->~AddGuildDataNtf();
				return true;
			}
			else
			{
				proto->~AddGuildDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelGuildDataNtf::protocol_id:
		{
			DelGuildDataNtf* proto = new(m_protocol_buffer) DelGuildDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelGuildDataNtf(*proto);
				proto->~DelGuildDataNtf();
				return true;
			}
			else
			{
				proto->~DelGuildDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateGuildDataNtf::protocol_id:
		{
			UpdateGuildDataNtf* proto = new(m_protocol_buffer) UpdateGuildDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateGuildDataNtf(*proto);
				proto->~UpdateGuildDataNtf();
				return true;
			}
			else
			{
				proto->~UpdateGuildDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddGuildMemberDataNtf::protocol_id:
		{
			AddGuildMemberDataNtf* proto = new(m_protocol_buffer) AddGuildMemberDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddGuildMemberDataNtf(*proto);
				proto->~AddGuildMemberDataNtf();
				return true;
			}
			else
			{
				proto->~AddGuildMemberDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelGuildMemberDataNtf::protocol_id:
		{
			DelGuildMemberDataNtf* proto = new(m_protocol_buffer) DelGuildMemberDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelGuildMemberDataNtf(*proto);
				proto->~DelGuildMemberDataNtf();
				return true;
			}
			else
			{
				proto->~DelGuildMemberDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateGuildMemberDataNtf::protocol_id:
		{
			UpdateGuildMemberDataNtf* proto = new(m_protocol_buffer) UpdateGuildMemberDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateGuildMemberDataNtf(*proto);
				proto->~UpdateGuildMemberDataNtf();
				return true;
			}
			else
			{
				proto->~UpdateGuildMemberDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddGuildApplicantDataNtf::protocol_id:
		{
			AddGuildApplicantDataNtf* proto = new(m_protocol_buffer) AddGuildApplicantDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddGuildApplicantDataNtf(*proto);
				proto->~AddGuildApplicantDataNtf();
				return true;
			}
			else
			{
				proto->~AddGuildApplicantDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelGuildApplicantDataNtf::protocol_id:
		{
			DelGuildApplicantDataNtf* proto = new(m_protocol_buffer) DelGuildApplicantDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelGuildApplicantDataNtf(*proto);
				proto->~DelGuildApplicantDataNtf();
				return true;
			}
			else
			{
				proto->~DelGuildApplicantDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateGuildApplicantDataNtf::protocol_id:
		{
			UpdateGuildApplicantDataNtf* proto = new(m_protocol_buffer) UpdateGuildApplicantDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateGuildApplicantDataNtf(*proto);
				proto->~UpdateGuildApplicantDataNtf();
				return true;
			}
			else
			{
				proto->~UpdateGuildApplicantDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadContactDataReq::protocol_id:
		{
			LoadContactDataReq* proto = new(m_protocol_buffer) LoadContactDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadContactDataReq(*proto);
				proto->~LoadContactDataReq();
				return true;
			}
			else
			{
				proto->~LoadContactDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadContactInfoDataAck::protocol_id:
		{
			LoadContactInfoDataAck* proto = new(m_protocol_buffer) LoadContactInfoDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadContactInfoDataAck(*proto);
				proto->~LoadContactInfoDataAck();
				return true;
			}
			else
			{
				proto->~LoadContactInfoDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadContactDataAck::protocol_id:
		{
			LoadContactDataAck* proto = new(m_protocol_buffer) LoadContactDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadContactDataAck(*proto);
				proto->~LoadContactDataAck();
				return true;
			}
			else
			{
				proto->~LoadContactDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddContactDataNtf::protocol_id:
		{
			AddContactDataNtf* proto = new(m_protocol_buffer) AddContactDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddContactDataNtf(*proto);
				proto->~AddContactDataNtf();
				return true;
			}
			else
			{
				proto->~AddContactDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelContactDataNtf::protocol_id:
		{
			DelContactDataNtf* proto = new(m_protocol_buffer) DelContactDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelContactDataNtf(*proto);
				proto->~DelContactDataNtf();
				return true;
			}
			else
			{
				proto->~DelContactDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateContactDataNtf::protocol_id:
		{
			UpdateContactDataNtf* proto = new(m_protocol_buffer) UpdateContactDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateContactDataNtf(*proto);
				proto->~UpdateContactDataNtf();
				return true;
			}
			else
			{
				proto->~UpdateContactDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadDBVarDataReq::protocol_id:
		{
			LoadDBVarDataReq* proto = new(m_protocol_buffer) LoadDBVarDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadDBVarDataReq(*proto);
				proto->~LoadDBVarDataReq();
				return true;
			}
			else
			{
				proto->~LoadDBVarDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadDBVarDataAck::protocol_id:
		{
			LoadDBVarDataAck* proto = new(m_protocol_buffer) LoadDBVarDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadDBVarDataAck(*proto);
				proto->~LoadDBVarDataAck();
				return true;
			}
			else
			{
				proto->~LoadDBVarDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddDBVarDataNtf::protocol_id:
		{
			AddDBVarDataNtf* proto = new(m_protocol_buffer) AddDBVarDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddDBVarDataNtf(*proto);
				proto->~AddDBVarDataNtf();
				return true;
			}
			else
			{
				proto->~AddDBVarDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelDBVarDataNtf::protocol_id:
		{
			DelDBVarDataNtf* proto = new(m_protocol_buffer) DelDBVarDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelDBVarDataNtf(*proto);
				proto->~DelDBVarDataNtf();
				return true;
			}
			else
			{
				proto->~DelDBVarDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadMailDataReq::protocol_id:
		{
			LoadMailDataReq* proto = new(m_protocol_buffer) LoadMailDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadMailDataReq(*proto);
				proto->~LoadMailDataReq();
				return true;
			}
			else
			{
				proto->~LoadMailDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadMailDataAck::protocol_id:
		{
			LoadMailDataAck* proto = new(m_protocol_buffer) LoadMailDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadMailDataAck(*proto);
				proto->~LoadMailDataAck();
				return true;
			}
			else
			{
				proto->~LoadMailDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddMailDataNtf::protocol_id:
		{
			AddMailDataNtf* proto = new(m_protocol_buffer) AddMailDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddMailDataNtf(*proto);
				proto->~AddMailDataNtf();
				return true;
			}
			else
			{
				proto->~AddMailDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelMailDataNtf::protocol_id:
		{
			DelMailDataNtf* proto = new(m_protocol_buffer) DelMailDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelMailDataNtf(*proto);
				proto->~DelMailDataNtf();
				return true;
			}
			else
			{
				proto->~DelMailDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateMailDataNtf::protocol_id:
		{
			UpdateMailDataNtf* proto = new(m_protocol_buffer) UpdateMailDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateMailDataNtf(*proto);
				proto->~UpdateMailDataNtf();
				return true;
			}
			else
			{
				proto->~UpdateMailDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadRanklistDataReq::protocol_id:
		{
			LoadRanklistDataReq* proto = new(m_protocol_buffer) LoadRanklistDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadRanklistDataReq(*proto);
				proto->~LoadRanklistDataReq();
				return true;
			}
			else
			{
				proto->~LoadRanklistDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadRanklistDataAck::protocol_id:
		{
			LoadRanklistDataAck* proto = new(m_protocol_buffer) LoadRanklistDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadRanklistDataAck(*proto);
				proto->~LoadRanklistDataAck();
				return true;
			}
			else
			{
				proto->~LoadRanklistDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddRanklistDataNtf::protocol_id:
		{
			AddRanklistDataNtf* proto = new(m_protocol_buffer) AddRanklistDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddRanklistDataNtf(*proto);
				proto->~AddRanklistDataNtf();
				return true;
			}
			else
			{
				proto->~AddRanklistDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelRanklistDataNtf::protocol_id:
		{
			DelRanklistDataNtf* proto = new(m_protocol_buffer) DelRanklistDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelRanklistDataNtf(*proto);
				proto->~DelRanklistDataNtf();
				return true;
			}
			else
			{
				proto->~DelRanklistDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateRanklistDataNtf::protocol_id:
		{
			UpdateRanklistDataNtf* proto = new(m_protocol_buffer) UpdateRanklistDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateRanklistDataNtf(*proto);
				proto->~UpdateRanklistDataNtf();
				return true;
			}
			else
			{
				proto->~UpdateRanklistDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case BillInMockReq::protocol_id:
		{
			BillInMockReq* proto = new(m_protocol_buffer) BillInMockReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_BillInMockReq(*proto);
				proto->~BillInMockReq();
				return true;
			}
			else
			{
				proto->~BillInMockReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddPasturePetDataNtf::protocol_id:
		{
			AddPasturePetDataNtf* proto = new(m_protocol_buffer) AddPasturePetDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddPasturePetDataNtf(*proto);
				proto->~AddPasturePetDataNtf();
				return true;
			}
			else
			{
				proto->~AddPasturePetDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadPasturePetDataReq::protocol_id:
		{
			LoadPasturePetDataReq* proto = new(m_protocol_buffer) LoadPasturePetDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadPasturePetDataReq(*proto);
				proto->~LoadPasturePetDataReq();
				return true;
			}
			else
			{
				proto->~LoadPasturePetDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadPasturePetDataAck::protocol_id:
		{
			LoadPasturePetDataAck* proto = new(m_protocol_buffer) LoadPasturePetDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadPasturePetDataAck(*proto);
				proto->~LoadPasturePetDataAck();
				return true;
			}
			else
			{
				proto->~LoadPasturePetDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case FregmentPkg::protocol_id:
		{
			FregmentPkg* proto = new(m_protocol_buffer) FregmentPkg();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_FregmentPkg(*proto);
				proto->~FregmentPkg();
				return true;
			}
			else
			{
				proto->~FregmentPkg();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelPasturePetDataNtf::protocol_id:
		{
			DelPasturePetDataNtf* proto = new(m_protocol_buffer) DelPasturePetDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelPasturePetDataNtf(*proto);
				proto->~DelPasturePetDataNtf();
				return true;
			}
			else
			{
				proto->~DelPasturePetDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdatePasturePetDataNtf::protocol_id:
		{
			UpdatePasturePetDataNtf* proto = new(m_protocol_buffer) UpdatePasturePetDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdatePasturePetDataNtf(*proto);
				proto->~UpdatePasturePetDataNtf();
				return true;
			}
			else
			{
				proto->~UpdatePasturePetDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadContactInfoReq::protocol_id:
		{
			LoadContactInfoReq* proto = new(m_protocol_buffer) LoadContactInfoReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadContactInfoReq(*proto);
				proto->~LoadContactInfoReq();
				return true;
			}
			else
			{
				proto->~LoadContactInfoReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadLadderDataReq::protocol_id:
		{
			LoadLadderDataReq* proto = new(m_protocol_buffer) LoadLadderDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadLadderDataReq(*proto);
				proto->~LoadLadderDataReq();
				return true;
			}
			else
			{
				proto->~LoadLadderDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadLadderDataAck::protocol_id:
		{
			LoadLadderDataAck* proto = new(m_protocol_buffer) LoadLadderDataAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadLadderDataAck(*proto);
				proto->~LoadLadderDataAck();
				return true;
			}
			else
			{
				proto->~LoadLadderDataAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddLadderDataNtf::protocol_id:
		{
			AddLadderDataNtf* proto = new(m_protocol_buffer) AddLadderDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddLadderDataNtf(*proto);
				proto->~AddLadderDataNtf();
				return true;
			}
			else
			{
				proto->~AddLadderDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelLadderDataNtf::protocol_id:
		{
			DelLadderDataNtf* proto = new(m_protocol_buffer) DelLadderDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelLadderDataNtf(*proto);
				proto->~DelLadderDataNtf();
				return true;
			}
			else
			{
				proto->~DelLadderDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UpdateLadderDataNtf::protocol_id:
		{
			UpdateLadderDataNtf* proto = new(m_protocol_buffer) UpdateLadderDataNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UpdateLadderDataNtf(*proto);
				proto->~UpdateLadderDataNtf();
				return true;
			}
			else
			{
				proto->~UpdateLadderDataNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case UnloadMailDataReq::protocol_id:
		{
			UnloadMailDataReq* proto = new(m_protocol_buffer) UnloadMailDataReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_UnloadMailDataReq(*proto);
				proto->~UnloadMailDataReq();
				return true;
			}
			else
			{
				proto->~UnloadMailDataReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadOfflinePlayerReq::protocol_id:
		{
			LoadOfflinePlayerReq* proto = new(m_protocol_buffer) LoadOfflinePlayerReq();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadOfflinePlayerReq(*proto);
				proto->~LoadOfflinePlayerReq();
				return true;
			}
			else
			{
				proto->~LoadOfflinePlayerReq();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LoadOfflinePlayerAck::protocol_id:
		{
			LoadOfflinePlayerAck* proto = new(m_protocol_buffer) LoadOfflinePlayerAck();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LoadOfflinePlayerAck(*proto);
				proto->~LoadOfflinePlayerAck();
				return true;
			}
			else
			{
				proto->~LoadOfflinePlayerAck();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SyncViolateNtf::protocol_id:
		{
			SyncViolateNtf* proto = new(m_protocol_buffer) SyncViolateNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SyncViolateNtf(*proto);
				proto->~SyncViolateNtf();
				return true;
			}
			else
			{
				proto->~SyncViolateNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case SyncForbidTalkNtf::protocol_id:
		{
			SyncForbidTalkNtf* proto = new(m_protocol_buffer) SyncForbidTalkNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SyncForbidTalkNtf(*proto);
				proto->~SyncForbidTalkNtf();
				return true;
			}
			else
			{
				proto->~SyncForbidTalkNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case AddForbidTalkNtf::protocol_id:
		{
			AddForbidTalkNtf* proto = new(m_protocol_buffer) AddForbidTalkNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddForbidTalkNtf(*proto);
				proto->~AddForbidTalkNtf();
				return true;
			}
			else
			{
				proto->~AddForbidTalkNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case DelForbidTalkNtf::protocol_id:
		{
			DelForbidTalkNtf* proto = new(m_protocol_buffer) DelForbidTalkNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelForbidTalkNtf(*proto);
				proto->~DelForbidTalkNtf();
				return true;
			}
			else
			{
				proto->~DelForbidTalkNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case LogLevelNtf::protocol_id:
		{
			LogLevelNtf* proto = new(m_protocol_buffer) LogLevelNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_LogLevelNtf(*proto);
				proto->~LogLevelNtf();
				return true;
			}
			else
			{
				proto->~LogLevelNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		default:
			return false;
		}

	}

	static inline unsigned short GetModuleID(void){ return 105; }

	static inline unsigned short GetProtocolNum(void){ return 69; }

	static const unsigned short module_id = 105;

	static const unsigned short protocol_num = 69;

	unsigned short ModuleId() override { return D::GetModuleID(); }
	unsigned short ProtocolNum() override { return D::GetProtocolNum(); }
	bool Handle(NetDeCode & net_data) override { return static_cast<D*>(this)->HandleProtocol(net_data); }
//===============以下协议回调函数需要使用者来实现===============
	void OnRecv_NewGSNtf(NewGSNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_TerminateNtf(TerminateNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ErrorNtf(ErrorNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ServerStopNtf(ServerStopNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ServerConfigNtf(ServerConfigNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DBWrapperPkg(DBWrapperPkg& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_GSWrapperPkg(GSWrapperPkg& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadAuctionObjectDataReq(LoadAuctionObjectDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadAuctionObjectDataAck(LoadAuctionObjectDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddAuctionObjectDataNtf(AddAuctionObjectDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelAuctionObjectDataNtf(DelAuctionObjectDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateAuctionObjectDataNtf(UpdateAuctionObjectDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadAuctionCookieDataReq(LoadAuctionCookieDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadAuctionCookieDataAck(LoadAuctionCookieDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DuplicateAuctionCookieDataNtf(DuplicateAuctionCookieDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadGuildDataReq(LoadGuildDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadGuildDataAck(LoadGuildDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadGuildMemberDataAck(LoadGuildMemberDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadGuildApplicantDataAck(LoadGuildApplicantDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddGuildDataNtf(AddGuildDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelGuildDataNtf(DelGuildDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateGuildDataNtf(UpdateGuildDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddGuildMemberDataNtf(AddGuildMemberDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelGuildMemberDataNtf(DelGuildMemberDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateGuildMemberDataNtf(UpdateGuildMemberDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddGuildApplicantDataNtf(AddGuildApplicantDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelGuildApplicantDataNtf(DelGuildApplicantDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateGuildApplicantDataNtf(UpdateGuildApplicantDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadContactDataReq(LoadContactDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadContactInfoDataAck(LoadContactInfoDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadContactDataAck(LoadContactDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddContactDataNtf(AddContactDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelContactDataNtf(DelContactDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateContactDataNtf(UpdateContactDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadDBVarDataReq(LoadDBVarDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadDBVarDataAck(LoadDBVarDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddDBVarDataNtf(AddDBVarDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelDBVarDataNtf(DelDBVarDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadMailDataReq(LoadMailDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadMailDataAck(LoadMailDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddMailDataNtf(AddMailDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelMailDataNtf(DelMailDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateMailDataNtf(UpdateMailDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadRanklistDataReq(LoadRanklistDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadRanklistDataAck(LoadRanklistDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddRanklistDataNtf(AddRanklistDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelRanklistDataNtf(DelRanklistDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateRanklistDataNtf(UpdateRanklistDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_BillInMockReq(BillInMockReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddPasturePetDataNtf(AddPasturePetDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadPasturePetDataReq(LoadPasturePetDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadPasturePetDataAck(LoadPasturePetDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_FregmentPkg(FregmentPkg& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelPasturePetDataNtf(DelPasturePetDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdatePasturePetDataNtf(UpdatePasturePetDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadContactInfoReq(LoadContactInfoReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadLadderDataReq(LoadLadderDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadLadderDataAck(LoadLadderDataAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddLadderDataNtf(AddLadderDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelLadderDataNtf(DelLadderDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UpdateLadderDataNtf(UpdateLadderDataNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_UnloadMailDataReq(UnloadMailDataReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadOfflinePlayerReq(LoadOfflinePlayerReq& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LoadOfflinePlayerAck(LoadOfflinePlayerAck& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SyncViolateNtf(SyncViolateNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SyncForbidTalkNtf(SyncForbidTalkNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddForbidTalkNtf(AddForbidTalkNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelForbidTalkNtf(DelForbidTalkNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_LogLevelNtf(LogLevelNtf& rstProtocol){ (void)(rstProtocol); };
private:
	 void* m_protocol_buffer;

};
