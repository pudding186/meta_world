#pragma once

#include "net_data.hpp"

//===============包含的其他协议文件===============
//===============宏定义开始===============
//===============宏定义结束===============

//===============数据定义开始===============
struct CustomStrData{
	DataArray<unsigned char, unsigned short> key; //key
	DataArray<unsigned char, unsigned int> value; //value
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(key);

		net_data.AddArray(value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(key))
			return false;

		if(!net_data.DelArray(value))
			return false;

		return true;
	}
	void Reset(void)
	{
		key.clear();

		value.clear();

	}
	CustomStrData& operator= (const CustomStrData& src)
	{
		key = src.key;

		value = src.value;

		return *this;
	}
	bool operator==(const CustomStrData& src) const
	{
		if (key != src.key)
		{
			return false;
		}

		if (value != src.value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const CustomStrData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CustomStrData& data, JsonHandler* parent = nullptr)
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.key, this);
                handler.String(str, length, copy);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned int> handler(m_data.value, this);
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
        CustomStrData& m_data;
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

		json_data.Writer().Key(u8"k", sizeof(u8"k") - 1);
		json_data.AddArray(key);

		json_data.Writer().Key(u8"v", sizeof(u8"v") - 1);
		json_data.AddArray(value);

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
        
        CustomStrData::Handler h(*this, nullptr);
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

struct CustomIntData{
	DataArray<unsigned char, unsigned short> key; //key
	signed long long          value; //value
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(key);

		net_data.AddIntegral(value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(key))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		return true;
	}
	void Reset(void)
	{
		key.clear();

		value = 0;

	}
	CustomIntData& operator= (const CustomIntData& src)
	{
		key = src.key;

		value = src.value;

		return *this;
	}
	bool operator==(const CustomIntData& src) const
	{
		if (key != src.key)
		{
			return false;
		}

		if (value != src.value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const CustomIntData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CustomIntData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.value, str, length);
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.key, this);
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
        CustomIntData& m_data;
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

		json_data.Writer().Key(u8"k", sizeof(u8"k") - 1);
		json_data.AddArray(key);

		json_data.AddIntegral(u8"v", value);

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
        
        CustomIntData::Handler h(*this, nullptr);
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

struct CustomData{
	DataArray<CustomStrData, unsigned int> strdata; //字符串数据集
	DataArray<CustomIntData, unsigned int> intdata; //数值数据集
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(strdata);

		net_data.AddArray(intdata);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(strdata))
			return false;

		if(!net_data.DelArray(intdata))
			return false;

		return true;
	}
	void Reset(void)
	{
		strdata.clear();

		intdata.clear();

	}
	CustomData& operator= (const CustomData& src)
	{
		strdata = src.strdata;

		intdata = src.intdata;

		return *this;
	}
	bool operator==(const CustomData& src) const
	{
		if (strdata != src.strdata)
		{
			return false;
		}

		if (intdata != src.intdata)
		{
			return false;
		}

		return true;
	}
	bool operator != (const CustomData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CustomData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_strdata_handler(m_data.strdata, this),
            m_intdata_handler(m_data.intdata, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_strdata_handler.ResetState();
            m_intdata_handler.ResetState();
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
                return &m_strdata_handler;
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_intdata_handler;
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

            if (!m_strdata_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_intdata_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        CustomData& m_data;
        DataArrayHandler<CustomStrData, unsigned int, CustomStrData::Handler> m_strdata_handler;
        DataArrayHandler<CustomIntData, unsigned int, CustomIntData::Handler> m_intdata_handler;
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

		json_data.Writer().Key(u8"s", sizeof(u8"s") - 1);
		json_data.AddArray(strdata);

		json_data.Writer().Key(u8"i", sizeof(u8"i") - 1);
		json_data.AddArray(intdata);

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
        
        CustomData::Handler h(*this, nullptr);
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

struct SkillData{
	unsigned short            id; //技能id
	unsigned int              performance; //当前熟练度
	unsigned int              max_performance; //最大熟练度
	unsigned long long        tick; //时间
	unsigned char             enable; //是否激活
	unsigned char             bind; //是否绑定
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(id);

		net_data.AddIntegral(performance);

		net_data.AddIntegral(max_performance);

		net_data.AddIntegral(tick);

		net_data.AddIntegral(enable);

		net_data.AddIntegral(bind);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(performance))
			return false;

		if (!net_data.DelIntegral(max_performance))
			return false;

		if (!net_data.DelIntegral(tick))
			return false;

		if (!net_data.DelIntegral(enable))
			return false;

		if (!net_data.DelIntegral(bind))
			return false;

		return true;
	}
	void Reset(void)
	{
		id = 0;

		performance = 0;

		max_performance = 0;

		tick = 0;

		enable = 0;

		bind = 0;

	}
	SkillData& operator= (const SkillData& src)
	{
		id = src.id;

		performance = src.performance;

		max_performance = src.max_performance;

		tick = src.tick;

		enable = src.enable;

		bind = src.bind;

		return *this;
	}
	bool operator==(const SkillData& src) const
	{
		if (id != src.id)
		{
			return false;
		}

		if (performance != src.performance)
		{
			return false;
		}

		if (max_performance != src.max_performance)
		{
			return false;
		}

		if (tick != src.tick)
		{
			return false;
		}

		if (enable != src.enable)
		{
			return false;
		}

		if (bind != src.bind)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SkillData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SkillData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.performance, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.max_performance, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.tick, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.enable, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.bind, str, length);
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
        SkillData& m_data;
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

		json_data.AddIntegral(u8"i", id);

		json_data.AddIntegral(u8"p", performance);

		json_data.AddIntegral(u8"mp", max_performance);

		json_data.AddIntegral(u8"t", tick);

		json_data.AddIntegral(u8"e", enable);

		json_data.AddIntegral(u8"b", bind);

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
        
        SkillData::Handler h(*this, nullptr);
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

struct DynAttrData{
	unsigned short            mark; //动态标记
	unsigned short            attr; //属性名
	signed long long          value; //属性值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(mark);

		net_data.AddIntegral(attr);

		net_data.AddIntegral(value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(mark))
			return false;

		if (!net_data.DelIntegral(attr))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		return true;
	}
	void Reset(void)
	{
		mark = 0;

		attr = 0;

		value = 0;

	}
	DynAttrData& operator= (const DynAttrData& src)
	{
		mark = src.mark;

		attr = src.attr;

		value = src.value;

		return *this;
	}
	bool operator==(const DynAttrData& src) const
	{
		if (mark != src.mark)
		{
			return false;
		}

		if (attr != src.attr)
		{
			return false;
		}

		if (value != src.value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DynAttrData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DynAttrData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.mark, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.attr, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.value, str, length);
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
        DynAttrData& m_data;
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

		json_data.AddIntegral(u8"m", mark);

		json_data.AddIntegral(u8"a", attr);

		json_data.AddIntegral(u8"v", value);

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
        
        DynAttrData::Handler h(*this, nullptr);
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

struct BuffData{
	unsigned short            buff_id; //Buff的ID
	unsigned int              start_times; //开始时间(仅绝对时间buff有效)
	unsigned int              duration; //持续数据 相对时间buff:为剩余时间 绝时时间buff:为持续时间 次数buff:为剩余次数
	DataArray<DynAttrData, unsigned short> dyn_attrs; //动态属性数据(固定属性值请从buff表读取)
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(buff_id);

		net_data.AddIntegral(start_times);

		net_data.AddIntegral(duration);

		net_data.AddArray(dyn_attrs);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(buff_id))
			return false;

		if (!net_data.DelIntegral(start_times))
			return false;

		if (!net_data.DelIntegral(duration))
			return false;

		if(!net_data.DelArray(dyn_attrs))
			return false;

		return true;
	}
	void Reset(void)
	{
		buff_id = 0;

		start_times = 0;

		duration = 0;

		dyn_attrs.clear();

	}
	BuffData& operator= (const BuffData& src)
	{
		buff_id = src.buff_id;

		start_times = src.start_times;

		duration = src.duration;

		dyn_attrs = src.dyn_attrs;

		return *this;
	}
	bool operator==(const BuffData& src) const
	{
		if (buff_id != src.buff_id)
		{
			return false;
		}

		if (start_times != src.start_times)
		{
			return false;
		}

		if (duration != src.duration)
		{
			return false;
		}

		if (dyn_attrs != src.dyn_attrs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const BuffData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(BuffData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_dyn_attrs_handler(m_data.dyn_attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_dyn_attrs_handler.ResetState();
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
                JsonDelIntegral(m_data.buff_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.start_times, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.duration, str, length);
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
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_dyn_attrs_handler;
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

            if (!m_dyn_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        BuffData& m_data;
        DataArrayHandler<DynAttrData, unsigned short, DynAttrData::Handler> m_dyn_attrs_handler;
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

		json_data.AddIntegral(u8"i", buff_id);

		json_data.AddIntegral(u8"st", start_times);

		json_data.AddIntegral(u8"d", duration);

		json_data.Writer().Key(u8"dyn", sizeof(u8"dyn") - 1);
		json_data.AddArray(dyn_attrs);

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
        
        BuffData::Handler h(*this, nullptr);
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

struct QuestData{
	unsigned int              quest_id; //任务ID
	unsigned char             quest_state; //任务状态
	unsigned int              accept_npc; //接任务目标NPC
	unsigned int              commit_npc; //交任务目标NPC
	unsigned int              quest_store_id; //任务库ID
	unsigned int              accpet_time; //任务接取时间
	unsigned int              limit_time; //任务限制时间
	unsigned int              changed_time; //任务状态改变时间
	DataArray<unsigned char, unsigned short> ext; //任务扩展数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(quest_id);

		net_data.AddIntegral(quest_state);

		net_data.AddIntegral(accept_npc);

		net_data.AddIntegral(commit_npc);

		net_data.AddIntegral(quest_store_id);

		net_data.AddIntegral(accpet_time);

		net_data.AddIntegral(limit_time);

		net_data.AddIntegral(changed_time);

		net_data.AddArray(ext);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(quest_id))
			return false;

		if (!net_data.DelIntegral(quest_state))
			return false;

		if (!net_data.DelIntegral(accept_npc))
			return false;

		if (!net_data.DelIntegral(commit_npc))
			return false;

		if (!net_data.DelIntegral(quest_store_id))
			return false;

		if (!net_data.DelIntegral(accpet_time))
			return false;

		if (!net_data.DelIntegral(limit_time))
			return false;

		if (!net_data.DelIntegral(changed_time))
			return false;

		if(!net_data.DelArray(ext))
			return false;

		return true;
	}
	void Reset(void)
	{
		quest_id = 0;

		quest_state = 0;

		accept_npc = 0;

		commit_npc = 0;

		quest_store_id = 0;

		accpet_time = 0;

		limit_time = 0;

		changed_time = 0;

		ext.clear();

	}
	QuestData& operator= (const QuestData& src)
	{
		quest_id = src.quest_id;

		quest_state = src.quest_state;

		accept_npc = src.accept_npc;

		commit_npc = src.commit_npc;

		quest_store_id = src.quest_store_id;

		accpet_time = src.accpet_time;

		limit_time = src.limit_time;

		changed_time = src.changed_time;

		ext = src.ext;

		return *this;
	}
	bool operator==(const QuestData& src) const
	{
		if (quest_id != src.quest_id)
		{
			return false;
		}

		if (quest_state != src.quest_state)
		{
			return false;
		}

		if (accept_npc != src.accept_npc)
		{
			return false;
		}

		if (commit_npc != src.commit_npc)
		{
			return false;
		}

		if (quest_store_id != src.quest_store_id)
		{
			return false;
		}

		if (accpet_time != src.accpet_time)
		{
			return false;
		}

		if (limit_time != src.limit_time)
		{
			return false;
		}

		if (changed_time != src.changed_time)
		{
			return false;
		}

		if (ext != src.ext)
		{
			return false;
		}

		return true;
	}
	bool operator != (const QuestData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(QuestData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.quest_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.quest_state, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.accept_npc, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.commit_npc, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.quest_store_id, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.accpet_time, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.limit_time, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.changed_time, str, length);
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
            case 8:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.ext, this);
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
        QuestData& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<9> m_assigned_bitset;

        static const std::bitset<9> template_assigned_bitset;
        static const char* m_key_list[9];
        static const char* m_alias_list[9];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"qi", quest_id);

		json_data.AddIntegral(u8"qs", quest_state);

		json_data.AddIntegral(u8"an", accept_npc);

		json_data.AddIntegral(u8"cn", commit_npc);

		json_data.AddIntegral(u8"qsi", quest_store_id);

		json_data.AddIntegral(u8"at", accpet_time);

		json_data.AddIntegral(u8"lt", limit_time);

		json_data.AddIntegral(u8"ct", changed_time);

		json_data.Writer().Key(u8"e", sizeof(u8"e") - 1);
		json_data.AddArray(ext);

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
        
        QuestData::Handler h(*this, nullptr);
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

struct QuestRingData{
	unsigned int              quest_id; //任务ID
	unsigned int              quest_ring; //任务环数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(quest_id);

		net_data.AddIntegral(quest_ring);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(quest_id))
			return false;

		if (!net_data.DelIntegral(quest_ring))
			return false;

		return true;
	}
	void Reset(void)
	{
		quest_id = 0;

		quest_ring = 0;

	}
	QuestRingData& operator= (const QuestRingData& src)
	{
		quest_id = src.quest_id;

		quest_ring = src.quest_ring;

		return *this;
	}
	bool operator==(const QuestRingData& src) const
	{
		if (quest_id != src.quest_id)
		{
			return false;
		}

		if (quest_ring != src.quest_ring)
		{
			return false;
		}

		return true;
	}
	bool operator != (const QuestRingData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(QuestRingData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.quest_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.quest_ring, str, length);
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
        QuestRingData& m_data;
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

		json_data.AddIntegral(u8"qi", quest_id);

		json_data.AddIntegral(u8"qr", quest_ring);

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
        
        QuestRingData::Handler h(*this, nullptr);
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

struct QuestStoreData{
	unsigned int              quest_store_id; //任务库ID
	unsigned int              ring_auto; //自动接取环数
	unsigned int              ring_day; //当天已完成的环数
	unsigned int              ring_week; //本周已完成的环数
	unsigned int              ring_display; //显示环数
	DataArray<QuestRingData, unsigned short> quest_rings; //子任务环数
	unsigned int              last_reset_day; //上次天数重置
	unsigned int              last_reset_week; //上次周数重置
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(quest_store_id);

		net_data.AddIntegral(ring_auto);

		net_data.AddIntegral(ring_day);

		net_data.AddIntegral(ring_week);

		net_data.AddIntegral(ring_display);

		net_data.AddArray(quest_rings);

		net_data.AddIntegral(last_reset_day);

		net_data.AddIntegral(last_reset_week);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(quest_store_id))
			return false;

		if (!net_data.DelIntegral(ring_auto))
			return false;

		if (!net_data.DelIntegral(ring_day))
			return false;

		if (!net_data.DelIntegral(ring_week))
			return false;

		if (!net_data.DelIntegral(ring_display))
			return false;

		if(!net_data.DelArray(quest_rings))
			return false;

		if (!net_data.DelIntegral(last_reset_day))
			return false;

		if (!net_data.DelIntegral(last_reset_week))
			return false;

		return true;
	}
	void Reset(void)
	{
		quest_store_id = 0;

		ring_auto = 0;

		ring_day = 0;

		ring_week = 0;

		ring_display = 0;

		quest_rings.clear();

		last_reset_day = 0;

		last_reset_week = 0;

	}
	QuestStoreData& operator= (const QuestStoreData& src)
	{
		quest_store_id = src.quest_store_id;

		ring_auto = src.ring_auto;

		ring_day = src.ring_day;

		ring_week = src.ring_week;

		ring_display = src.ring_display;

		quest_rings = src.quest_rings;

		last_reset_day = src.last_reset_day;

		last_reset_week = src.last_reset_week;

		return *this;
	}
	bool operator==(const QuestStoreData& src) const
	{
		if (quest_store_id != src.quest_store_id)
		{
			return false;
		}

		if (ring_auto != src.ring_auto)
		{
			return false;
		}

		if (ring_day != src.ring_day)
		{
			return false;
		}

		if (ring_week != src.ring_week)
		{
			return false;
		}

		if (ring_display != src.ring_display)
		{
			return false;
		}

		if (quest_rings != src.quest_rings)
		{
			return false;
		}

		if (last_reset_day != src.last_reset_day)
		{
			return false;
		}

		if (last_reset_week != src.last_reset_week)
		{
			return false;
		}

		return true;
	}
	bool operator != (const QuestStoreData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(QuestStoreData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_quest_rings_handler(m_data.quest_rings, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_quest_rings_handler.ResetState();
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
                JsonDelIntegral(m_data.quest_store_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.ring_auto, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.ring_day, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.ring_week, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.ring_display, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_reset_day, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_reset_week, str, length);
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
            case 5:
            {
                m_assigned_bitset.set(m_state);
                return &m_quest_rings_handler;
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

            if (!m_quest_rings_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        QuestStoreData& m_data;
        DataArrayHandler<QuestRingData, unsigned short, QuestRingData::Handler> m_quest_rings_handler;
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

		json_data.AddIntegral(u8"qsi", quest_store_id);

		json_data.AddIntegral(u8"ring_auto", ring_auto);

		json_data.AddIntegral(u8"ring_day", ring_day);

		json_data.AddIntegral(u8"ring_week", ring_week);

		json_data.AddIntegral(u8"ring_display", ring_display);

		json_data.Writer().Key(u8"quest_rings", sizeof(u8"quest_rings") - 1);
		json_data.AddArray(quest_rings);

		json_data.AddIntegral(u8"last_reset_day", last_reset_day);

		json_data.AddIntegral(u8"last_reset_week", last_reset_week);

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
        
        QuestStoreData::Handler h(*this, nullptr);
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

struct QuestCountData{
	unsigned int              quest_id; //任务ID
	unsigned int              count; //任务计数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(quest_id);

		net_data.AddIntegral(count);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(quest_id))
			return false;

		if (!net_data.DelIntegral(count))
			return false;

		return true;
	}
	void Reset(void)
	{
		quest_id = 0;

		count = 0;

	}
	QuestCountData& operator= (const QuestCountData& src)
	{
		quest_id = src.quest_id;

		count = src.count;

		return *this;
	}
	bool operator==(const QuestCountData& src) const
	{
		if (quest_id != src.quest_id)
		{
			return false;
		}

		if (count != src.count)
		{
			return false;
		}

		return true;
	}
	bool operator != (const QuestCountData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(QuestCountData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.quest_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.count, str, length);
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
        QuestCountData& m_data;
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

		json_data.AddIntegral(u8"qi", quest_id);

		json_data.AddIntegral(u8"c", count);

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
        
        QuestCountData::Handler h(*this, nullptr);
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

struct QuestContainerData{
	DataArray<QuestData, unsigned short> quests; //任务
	DataArray<unsigned int, unsigned short> npc_visible; //可见npc列表
	DataArray<unsigned int, unsigned short> npc_invisible; //不可见npc列表
	DataArray<QuestStoreData, unsigned short> stores; //任务库
	DataArray<unsigned int, unsigned short> abandoned; //放弃任务列表
	DataArray<unsigned int, unsigned short> finished; //完成任务列表
	DataArray<unsigned int, unsigned short> activated; //激活任务列表
	DataArray<QuestCountData, unsigned short> failed_counts; //失败任务计数列表
	DataArray<QuestCountData, unsigned short> finished_counts; //完成任务计数列表
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(quests);

		net_data.AddArray(npc_visible);

		net_data.AddArray(npc_invisible);

		net_data.AddArray(stores);

		net_data.AddArray(abandoned);

		net_data.AddArray(finished);

		net_data.AddArray(activated);

		net_data.AddArray(failed_counts);

		net_data.AddArray(finished_counts);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(quests))
			return false;

		if(!net_data.DelArray(npc_visible))
			return false;

		if(!net_data.DelArray(npc_invisible))
			return false;

		if(!net_data.DelArray(stores))
			return false;

		if(!net_data.DelArray(abandoned))
			return false;

		if(!net_data.DelArray(finished))
			return false;

		if(!net_data.DelArray(activated))
			return false;

		if(!net_data.DelArray(failed_counts))
			return false;

		if(!net_data.DelArray(finished_counts))
			return false;

		return true;
	}
	void Reset(void)
	{
		quests.clear();

		npc_visible.clear();

		npc_invisible.clear();

		stores.clear();

		abandoned.clear();

		finished.clear();

		activated.clear();

		failed_counts.clear();

		finished_counts.clear();

	}
	QuestContainerData& operator= (const QuestContainerData& src)
	{
		quests = src.quests;

		npc_visible = src.npc_visible;

		npc_invisible = src.npc_invisible;

		stores = src.stores;

		abandoned = src.abandoned;

		finished = src.finished;

		activated = src.activated;

		failed_counts = src.failed_counts;

		finished_counts = src.finished_counts;

		return *this;
	}
	bool operator==(const QuestContainerData& src) const
	{
		if (quests != src.quests)
		{
			return false;
		}

		if (npc_visible != src.npc_visible)
		{
			return false;
		}

		if (npc_invisible != src.npc_invisible)
		{
			return false;
		}

		if (stores != src.stores)
		{
			return false;
		}

		if (abandoned != src.abandoned)
		{
			return false;
		}

		if (finished != src.finished)
		{
			return false;
		}

		if (activated != src.activated)
		{
			return false;
		}

		if (failed_counts != src.failed_counts)
		{
			return false;
		}

		if (finished_counts != src.finished_counts)
		{
			return false;
		}

		return true;
	}
	bool operator != (const QuestContainerData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(QuestContainerData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_quests_handler(m_data.quests, this),
            m_npc_visible_handler(m_data.npc_visible, this),
            m_npc_invisible_handler(m_data.npc_invisible, this),
            m_stores_handler(m_data.stores, this),
            m_abandoned_handler(m_data.abandoned, this),
            m_finished_handler(m_data.finished, this),
            m_activated_handler(m_data.activated, this),
            m_failed_counts_handler(m_data.failed_counts, this),
            m_finished_counts_handler(m_data.finished_counts, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_quests_handler.ResetState();
            m_stores_handler.ResetState();
            m_failed_counts_handler.ResetState();
            m_finished_counts_handler.ResetState();
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
                return &m_quests_handler;
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_npc_visible_handler;
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_npc_invisible_handler;
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_stores_handler;
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                return &m_abandoned_handler;
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                return &m_finished_handler;
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                return &m_activated_handler;
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                return &m_failed_counts_handler;
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                return &m_finished_counts_handler;
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

            if (!m_quests_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_npc_visible_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_npc_invisible_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_stores_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_abandoned_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_finished_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_activated_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_failed_counts_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_finished_counts_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        QuestContainerData& m_data;
        DataArrayHandler<QuestData, unsigned short, QuestData::Handler> m_quests_handler;
        DataArrayHandler<unsigned int, unsigned short> m_npc_visible_handler;
        DataArrayHandler<unsigned int, unsigned short> m_npc_invisible_handler;
        DataArrayHandler<QuestStoreData, unsigned short, QuestStoreData::Handler> m_stores_handler;
        DataArrayHandler<unsigned int, unsigned short> m_abandoned_handler;
        DataArrayHandler<unsigned int, unsigned short> m_finished_handler;
        DataArrayHandler<unsigned int, unsigned short> m_activated_handler;
        DataArrayHandler<QuestCountData, unsigned short, QuestCountData::Handler> m_failed_counts_handler;
        DataArrayHandler<QuestCountData, unsigned short, QuestCountData::Handler> m_finished_counts_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<9> m_assigned_bitset;

        static const std::bitset<9> template_assigned_bitset;
        static const char* m_key_list[9];
        static const char* m_alias_list[9];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.Writer().Key(u8"i", sizeof(u8"i") - 1);
		json_data.AddArray(quests);

		json_data.Writer().Key(u8"nv", sizeof(u8"nv") - 1);
		json_data.AddArray(npc_visible);

		json_data.Writer().Key(u8"niv", sizeof(u8"niv") - 1);
		json_data.AddArray(npc_invisible);

		json_data.Writer().Key(u8"s", sizeof(u8"s") - 1);
		json_data.AddArray(stores);

		json_data.Writer().Key(u8"ab", sizeof(u8"ab") - 1);
		json_data.AddArray(abandoned);

		json_data.Writer().Key(u8"fi", sizeof(u8"fi") - 1);
		json_data.AddArray(finished);

		json_data.Writer().Key(u8"ac", sizeof(u8"ac") - 1);
		json_data.AddArray(activated);

		json_data.Writer().Key(u8"fac", sizeof(u8"fac") - 1);
		json_data.AddArray(failed_counts);

		json_data.Writer().Key(u8"fic", sizeof(u8"fic") - 1);
		json_data.AddArray(finished_counts);

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
        
        QuestContainerData::Handler h(*this, nullptr);
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

struct AttrData{
	unsigned short            attr; //属性名
	signed long long          value; //属性值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(attr);

		net_data.AddIntegral(value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(attr))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		return true;
	}
	void Reset(void)
	{
		attr = 0;

		value = 0;

	}
	AttrData& operator= (const AttrData& src)
	{
		attr = src.attr;

		value = src.value;

		return *this;
	}
	bool operator==(const AttrData& src) const
	{
		if (attr != src.attr)
		{
			return false;
		}

		if (value != src.value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AttrData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AttrData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.attr, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.value, str, length);
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
        AttrData& m_data;
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

		json_data.AddIntegral(u8"a", attr);

		json_data.AddIntegral(u8"v", value);

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
        
        AttrData::Handler h(*this, nullptr);
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

struct ItemData{
	unsigned long long        guid; //guid
	unsigned short            id; //模板Id
	unsigned long long        role; //拥有者GUID
	unsigned short            site; //当前所在的位置
	short                     battlesite; //战斗物品site
	unsigned short            amount; //当前的堆叠数量
	unsigned char             isbound; //是否已绑定
	unsigned int              life; //限时物品截止时间，0表示永久
	DataArray<DynAttrData, unsigned short> dyn_attrs; //动态属性数据
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(id);

		net_data.AddIntegral(role);

		net_data.AddIntegral(site);

		net_data.AddIntegral(battlesite);

		net_data.AddIntegral(amount);

		net_data.AddIntegral(isbound);

		net_data.AddIntegral(life);

		net_data.AddArray(dyn_attrs);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(role))
			return false;

		if (!net_data.DelIntegral(site))
			return false;

		if (!net_data.DelIntegral(battlesite))
			return false;

		if (!net_data.DelIntegral(amount))
			return false;

		if (!net_data.DelIntegral(isbound))
			return false;

		if (!net_data.DelIntegral(life))
			return false;

		if(!net_data.DelArray(dyn_attrs))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		id = 0;

		role = 0;

		site = 0;

		battlesite = 0;

		amount = 0;

		isbound = 0;

		life = 0;

		dyn_attrs.clear();

		customs.Reset();

	}
	ItemData& operator= (const ItemData& src)
	{
		guid = src.guid;

		id = src.id;

		role = src.role;

		site = src.site;

		battlesite = src.battlesite;

		amount = src.amount;

		isbound = src.isbound;

		life = src.life;

		dyn_attrs = src.dyn_attrs;

		customs = src.customs;

		return *this;
	}
	bool operator==(const ItemData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (id != src.id)
		{
			return false;
		}

		if (role != src.role)
		{
			return false;
		}

		if (site != src.site)
		{
			return false;
		}

		if (battlesite != src.battlesite)
		{
			return false;
		}

		if (amount != src.amount)
		{
			return false;
		}

		if (isbound != src.isbound)
		{
			return false;
		}

		if (life != src.life)
		{
			return false;
		}

		if (dyn_attrs != src.dyn_attrs)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ItemData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ItemData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_dyn_attrs_handler(m_data.dyn_attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
            m_dyn_attrs_handler.ResetState();
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
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.role, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.site, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.battlesite, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.amount, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.isbound, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.life, str, length);
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
            case 9:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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
            switch (m_state)
            {
            case 8:
            {
                m_assigned_bitset.set(m_state);
                return &m_dyn_attrs_handler;
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

            if (!m_dyn_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ItemData& m_data;
        CustomData::Handler m_customs_handler;
        DataArrayHandler<DynAttrData, unsigned short, DynAttrData::Handler> m_dyn_attrs_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<10> m_assigned_bitset;

        static const std::bitset<10> template_assigned_bitset;
        static const char* m_key_list[10];
        static const char* m_alias_list[10];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"g", guid);

		json_data.AddIntegral(u8"i", id);

		json_data.AddIntegral(u8"r", role);

		json_data.AddIntegral(u8"s", site);

		json_data.AddIntegral(u8"bs", battlesite);

		json_data.AddIntegral(u8"a", amount);

		json_data.AddIntegral(u8"ib", isbound);

		json_data.AddIntegral(u8"l", life);

		json_data.Writer().Key(u8"da", sizeof(u8"da") - 1);
		json_data.AddArray(dyn_attrs);

		json_data.Writer().Key(u8"c", sizeof(u8"c") - 1);
		customs.ToJson(json_data);

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
        
        ItemData::Handler h(*this, nullptr);
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

struct ItemContainerData{
	unsigned short            container_type; //item容器类型
	unsigned short            capacity; //item容器容量上限
	DataArray<ItemData, unsigned short> items; //道具
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(container_type);

		net_data.AddIntegral(capacity);

		net_data.AddArray(items);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(container_type))
			return false;

		if (!net_data.DelIntegral(capacity))
			return false;

		if(!net_data.DelArray(items))
			return false;

		return true;
	}
	void Reset(void)
	{
		container_type = 0;

		capacity = 0;

		items.clear();

	}
	ItemContainerData& operator= (const ItemContainerData& src)
	{
		container_type = src.container_type;

		capacity = src.capacity;

		items = src.items;

		return *this;
	}
	bool operator==(const ItemContainerData& src) const
	{
		if (container_type != src.container_type)
		{
			return false;
		}

		if (capacity != src.capacity)
		{
			return false;
		}

		if (items != src.items)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ItemContainerData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ItemContainerData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_items_handler(m_data.items, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_items_handler.ResetState();
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
                JsonDelIntegral(m_data.container_type, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.capacity, str, length);
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
                return &m_items_handler;
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

            if (!m_items_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ItemContainerData& m_data;
        DataArrayHandler<ItemData, unsigned short, ItemData::Handler> m_items_handler;
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

		json_data.AddIntegral(u8"ct", container_type);

		json_data.AddIntegral(u8"c", capacity);

		json_data.Writer().Key(u8"i", sizeof(u8"i") - 1);
		json_data.AddArray(items);

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
        
        ItemContainerData::Handler h(*this, nullptr);
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

struct UserData{
	unsigned int              uid; //账号ID
	unsigned char             sid; //子区ID
	char                      uname[32]; //账号名字
	signed long long          billinyb; //累计充值元宝
	signed long long          billoutyb; //累计消耗的充值元宝
	signed long long          gameinyb; //累计游戏内产出元宝
	signed long long          gameoutyb; //累计游戏内消耗元宝
	unsigned int              createtime; //创建时间
	unsigned int              lastlogin; //最后一次登陆时间
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddString(uname, sizeof(uname));

		net_data.AddIntegral(billinyb);

		net_data.AddIntegral(billoutyb);

		net_data.AddIntegral(gameinyb);

		net_data.AddIntegral(gameoutyb);

		net_data.AddIntegral(createtime);

		net_data.AddIntegral(lastlogin);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(uid))
			return false;

		if (!net_data.DelIntegral(sid))
			return false;

		if (!net_data.DelString(uname, sizeof(uname)))
			return false;

		if (!net_data.DelIntegral(billinyb))
			return false;

		if (!net_data.DelIntegral(billoutyb))
			return false;

		if (!net_data.DelIntegral(gameinyb))
			return false;

		if (!net_data.DelIntegral(gameoutyb))
			return false;

		if (!net_data.DelIntegral(createtime))
			return false;

		if (!net_data.DelIntegral(lastlogin))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		uname[0] = 0;

		billinyb = 0;

		billoutyb = 0;

		gameinyb = 0;

		gameoutyb = 0;

		createtime = 0;

		lastlogin = 0;

	}
	UserData& operator= (const UserData& src)
	{
		uid = src.uid;

		sid = src.sid;

		{
			size_t str_len = strnlen(src.uname, sizeof(uname) - 1);
			memcpy(uname, src.uname, str_len + 1);
			uname[sizeof(uname) - 1] = 0;
		}

		billinyb = src.billinyb;

		billoutyb = src.billoutyb;

		gameinyb = src.gameinyb;

		gameoutyb = src.gameoutyb;

		createtime = src.createtime;

		lastlogin = src.lastlogin;

		return *this;
	}
	bool operator==(const UserData& src) const
	{
		if (uid != src.uid)
		{
			return false;
		}

		if (sid != src.sid)
		{
			return false;
		}

		if (strncmp(uname, src.uname, sizeof(uname)-1))
		{
			return false;
		}

		if (billinyb != src.billinyb)
		{
			return false;
		}

		if (billoutyb != src.billoutyb)
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

		if (createtime != src.createtime)
		{
			return false;
		}

		if (lastlogin != src.lastlogin)
		{
			return false;
		}

		return true;
	}
	bool operator != (const UserData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(UserData& data, JsonHandler* parent = nullptr)
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
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.billinyb, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.billoutyb, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gameinyb, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gameoutyb, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.createtime, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.lastlogin, str, length);
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
                JsonDelString(m_data.uname, str, length);
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
        UserData& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<9> m_assigned_bitset;

        static const std::bitset<9> template_assigned_bitset;
        static const char* m_key_list[9];
        static const char* m_alias_list[9];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddString(u8"uname", uname, sizeof(uname));

		json_data.AddIntegral(u8"billinyb", billinyb);

		json_data.AddIntegral(u8"billoutyb", billoutyb);

		json_data.AddIntegral(u8"gameinyb", gameinyb);

		json_data.AddIntegral(u8"gameoutyb", gameoutyb);

		json_data.AddIntegral(u8"createtime", createtime);

		json_data.AddIntegral(u8"lastlogin", lastlogin);

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
        
        UserData::Handler h(*this, nullptr);
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

struct SceneData{
	unsigned long long        map_guid; //地图guid
	unsigned short            x; //地图x坐标
	unsigned short            y; //地图y坐标
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(map_guid);

		net_data.AddIntegral(x);

		net_data.AddIntegral(y);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(map_guid))
			return false;

		if (!net_data.DelIntegral(x))
			return false;

		if (!net_data.DelIntegral(y))
			return false;

		return true;
	}
	void Reset(void)
	{
		map_guid = 0;

		x = 0;

		y = 0;

	}
	SceneData& operator= (const SceneData& src)
	{
		map_guid = src.map_guid;

		x = src.x;

		y = src.y;

		return *this;
	}
	bool operator==(const SceneData& src) const
	{
		if (map_guid != src.map_guid)
		{
			return false;
		}

		if (x != src.x)
		{
			return false;
		}

		if (y != src.y)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SceneData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SceneData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.map_guid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.x, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.y, str, length);
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
        SceneData& m_data;
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

		json_data.AddIntegral(u8"map_guid", map_guid);

		json_data.AddIntegral(u8"x", x);

		json_data.AddIntegral(u8"y", y);

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
        
        SceneData::Handler h(*this, nullptr);
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

struct TicketData{
	unsigned long long        map_guid; //地图guid
	unsigned int              create_time; //创建时间
	unsigned short            id; //地图ID
	unsigned short            x; //地图x坐标
	unsigned short            y; //地图y坐标
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(map_guid);

		net_data.AddIntegral(create_time);

		net_data.AddIntegral(id);

		net_data.AddIntegral(x);

		net_data.AddIntegral(y);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(map_guid))
			return false;

		if (!net_data.DelIntegral(create_time))
			return false;

		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(x))
			return false;

		if (!net_data.DelIntegral(y))
			return false;

		return true;
	}
	void Reset(void)
	{
		map_guid = 0;

		create_time = 0;

		id = 0;

		x = 0;

		y = 0;

	}
	TicketData& operator= (const TicketData& src)
	{
		map_guid = src.map_guid;

		create_time = src.create_time;

		id = src.id;

		x = src.x;

		y = src.y;

		return *this;
	}
	bool operator==(const TicketData& src) const
	{
		if (map_guid != src.map_guid)
		{
			return false;
		}

		if (create_time != src.create_time)
		{
			return false;
		}

		if (id != src.id)
		{
			return false;
		}

		if (x != src.x)
		{
			return false;
		}

		if (y != src.y)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TicketData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TicketData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.map_guid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.create_time, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.x, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.y, str, length);
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
        TicketData& m_data;
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

		json_data.AddIntegral(u8"map_guid", map_guid);

		json_data.AddIntegral(u8"create_time", create_time);

		json_data.AddIntegral(u8"id", id);

		json_data.AddIntegral(u8"x", x);

		json_data.AddIntegral(u8"y", y);

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
        
        TicketData::Handler h(*this, nullptr);
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

struct PetData{
	unsigned long long        guid; //宠物GUID
	char                      name[32]; //宠物名字
	ItemContainerData         equips; //宠物装备
	DataArray<AttrData, unsigned short> attrs; //属性数据
	DataArray<AttrData, unsigned short> extra_attrs; //额外属性数据
	DataArray<SkillData, unsigned short> skills; //技能
	DataArray<BuffData, unsigned short> buffs; //buff
	DataArray<DynAttrData, unsigned short> dyn_attrs; //动态属性数据
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddString(name, sizeof(name));

		equips.EnCode(net_data);

		net_data.AddArray(attrs);

		net_data.AddArray(extra_attrs);

		net_data.AddArray(skills);

		net_data.AddArray(buffs);

		net_data.AddArray(dyn_attrs);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!equips.DeCode(net_data))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if(!net_data.DelArray(extra_attrs))
			return false;

		if(!net_data.DelArray(skills))
			return false;

		if(!net_data.DelArray(buffs))
			return false;

		if(!net_data.DelArray(dyn_attrs))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		name[0] = 0;

		equips.Reset();

		attrs.clear();

		extra_attrs.clear();

		skills.clear();

		buffs.clear();

		dyn_attrs.clear();

		customs.Reset();

	}
	PetData& operator= (const PetData& src)
	{
		guid = src.guid;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		equips = src.equips;

		attrs = src.attrs;

		extra_attrs = src.extra_attrs;

		skills = src.skills;

		buffs = src.buffs;

		dyn_attrs = src.dyn_attrs;

		customs = src.customs;

		return *this;
	}
	bool operator==(const PetData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (equips != src.equips)
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (extra_attrs != src.extra_attrs)
		{
			return false;
		}

		if (skills != src.skills)
		{
			return false;
		}

		if (buffs != src.buffs)
		{
			return false;
		}

		if (dyn_attrs != src.dyn_attrs)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const PetData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PetData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_equips_handler(m_data.equips, this),
            m_customs_handler(m_data.customs, this),
            m_attrs_handler(m_data.attrs, this),
            m_extra_attrs_handler(m_data.extra_attrs, this),
            m_skills_handler(m_data.skills, this),
            m_buffs_handler(m_data.buffs, this),
            m_dyn_attrs_handler(m_data.dyn_attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_equips_handler.ResetState();
            m_customs_handler.ResetState();
            m_attrs_handler.ResetState();
            m_extra_attrs_handler.ResetState();
            m_skills_handler.ResetState();
            m_buffs_handler.ResetState();
            m_dyn_attrs_handler.ResetState();
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
            switch (m_state)
            {
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_equips_handler;
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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
            switch (m_state)
            {
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_attrs_handler;
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                return &m_extra_attrs_handler;
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                return &m_skills_handler;
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                return &m_buffs_handler;
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                return &m_dyn_attrs_handler;
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

            if (!m_equips_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_extra_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_skills_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_buffs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_dyn_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        PetData& m_data;
        ItemContainerData::Handler m_equips_handler;
        CustomData::Handler m_customs_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_extra_attrs_handler;
        DataArrayHandler<SkillData, unsigned short, SkillData::Handler> m_skills_handler;
        DataArrayHandler<BuffData, unsigned short, BuffData::Handler> m_buffs_handler;
        DataArrayHandler<DynAttrData, unsigned short, DynAttrData::Handler> m_dyn_attrs_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<9> m_assigned_bitset;

        static const std::bitset<9> template_assigned_bitset;
        static const char* m_key_list[9];
        static const char* m_alias_list[9];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"g", guid);

		json_data.AddString(u8"n", name, sizeof(name));

		json_data.Writer().Key(u8"e", sizeof(u8"e") - 1);
		equips.ToJson(json_data);

		json_data.Writer().Key(u8"a", sizeof(u8"a") - 1);
		json_data.AddArray(attrs);

		json_data.Writer().Key(u8"ea", sizeof(u8"ea") - 1);
		json_data.AddArray(extra_attrs);

		json_data.Writer().Key(u8"s", sizeof(u8"s") - 1);
		json_data.AddArray(skills);

		json_data.Writer().Key(u8"b", sizeof(u8"b") - 1);
		json_data.AddArray(buffs);

		json_data.Writer().Key(u8"da", sizeof(u8"da") - 1);
		json_data.AddArray(dyn_attrs);

		json_data.Writer().Key(u8"c", sizeof(u8"c") - 1);
		customs.ToJson(json_data);

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
        
        PetData::Handler h(*this, nullptr);
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

struct PetContainerData{
	unsigned short            container_type; //pet容器类型
	unsigned short            capacity; //pet容器容量上限
	DataArray<PetData, unsigned short> pets; //宠物
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(container_type);

		net_data.AddIntegral(capacity);

		net_data.AddArray(pets);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(container_type))
			return false;

		if (!net_data.DelIntegral(capacity))
			return false;

		if(!net_data.DelArray(pets))
			return false;

		return true;
	}
	void Reset(void)
	{
		container_type = 0;

		capacity = 0;

		pets.clear();

	}
	PetContainerData& operator= (const PetContainerData& src)
	{
		container_type = src.container_type;

		capacity = src.capacity;

		pets = src.pets;

		return *this;
	}
	bool operator==(const PetContainerData& src) const
	{
		if (container_type != src.container_type)
		{
			return false;
		}

		if (capacity != src.capacity)
		{
			return false;
		}

		if (pets != src.pets)
		{
			return false;
		}

		return true;
	}
	bool operator != (const PetContainerData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PetContainerData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_pets_handler(m_data.pets, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_pets_handler.ResetState();
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
                JsonDelIntegral(m_data.container_type, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.capacity, str, length);
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
                return &m_pets_handler;
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

            if (!m_pets_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        PetContainerData& m_data;
        DataArrayHandler<PetData, unsigned short, PetData::Handler> m_pets_handler;
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

		json_data.AddIntegral(u8"ct", container_type);

		json_data.AddIntegral(u8"c", capacity);

		json_data.Writer().Key(u8"p", sizeof(u8"p") - 1);
		json_data.AddArray(pets);

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
        
        PetContainerData::Handler h(*this, nullptr);
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

struct TitleData{
	unsigned short            title_id; //称号ID
	unsigned int              end_time; //限时称号截止时间，0表示永久称号
	CustomData                custom; //自定义数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(title_id);

		net_data.AddIntegral(end_time);

		custom.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(title_id))
			return false;

		if (!net_data.DelIntegral(end_time))
			return false;

		if (!custom.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		title_id = 0;

		end_time = 0;

		custom.Reset();

	}
	TitleData& operator= (const TitleData& src)
	{
		title_id = src.title_id;

		end_time = src.end_time;

		custom = src.custom;

		return *this;
	}
	bool operator==(const TitleData& src) const
	{
		if (title_id != src.title_id)
		{
			return false;
		}

		if (end_time != src.end_time)
		{
			return false;
		}

		if (custom != src.custom)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TitleData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TitleData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_custom_handler(m_data.custom, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_custom_handler.ResetState();
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
                JsonDelIntegral(m_data.title_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.end_time, str, length);
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
                return &m_custom_handler;
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

            if (!m_custom_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        TitleData& m_data;
        CustomData::Handler m_custom_handler;
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

		json_data.AddIntegral(u8"ti", title_id);

		json_data.AddIntegral(u8"et", end_time);

		json_data.Writer().Key(u8"c", sizeof(u8"c") - 1);
		custom.ToJson(json_data);

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
        
        TitleData::Handler h(*this, nullptr);
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

struct TitleContainerData{
	DataArray<TitleData, unsigned short> titles; //称号
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(titles);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(titles))
			return false;

		return true;
	}
	void Reset(void)
	{
		titles.clear();

	}
	TitleContainerData& operator= (const TitleContainerData& src)
	{
		titles = src.titles;

		return *this;
	}
	bool operator==(const TitleContainerData& src) const
	{
		if (titles != src.titles)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TitleContainerData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TitleContainerData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_titles_handler(m_data.titles, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_titles_handler.ResetState();
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
                return &m_titles_handler;
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

            if (!m_titles_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        TitleContainerData& m_data;
        DataArrayHandler<TitleData, unsigned short, TitleData::Handler> m_titles_handler;
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

		json_data.Writer().Key(u8"t", sizeof(u8"t") - 1);
		json_data.AddArray(titles);

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
        
        TitleContainerData::Handler h(*this, nullptr);
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

struct SeatBasic{
	unsigned short            id; //阵法id
	unsigned char             level; //阵法等级
	unsigned int              score; //积分值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(id);

		net_data.AddIntegral(level);

		net_data.AddIntegral(score);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(score))
			return false;

		return true;
	}
	void Reset(void)
	{
		id = 0;

		level = 0;

		score = 0;

	}
	SeatBasic& operator= (const SeatBasic& src)
	{
		id = src.id;

		level = src.level;

		score = src.score;

		return *this;
	}
	bool operator==(const SeatBasic& src) const
	{
		if (id != src.id)
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (score != src.score)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SeatBasic& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SeatBasic& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.score, str, length);
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
        SeatBasic& m_data;
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

		json_data.AddIntegral(u8"id", id);

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"score", score);

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
        
        SeatBasic::Handler h(*this, nullptr);
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

struct SeatData{
	DataArray<SeatBasic, unsigned char> seats; //所有阵法数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(seats);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(seats))
			return false;

		return true;
	}
	void Reset(void)
	{
		seats.clear();

	}
	SeatData& operator= (const SeatData& src)
	{
		seats = src.seats;

		return *this;
	}
	bool operator==(const SeatData& src) const
	{
		if (seats != src.seats)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SeatData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SeatData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_seats_handler(m_data.seats, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_seats_handler.ResetState();
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
                return &m_seats_handler;
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

            if (!m_seats_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        SeatData& m_data;
        DataArrayHandler<SeatBasic, unsigned char, SeatBasic::Handler> m_seats_handler;
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

		json_data.Writer().Key(u8"seats", sizeof(u8"seats") - 1);
		json_data.AddArray(seats);

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
        
        SeatData::Handler h(*this, nullptr);
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

struct LineupBasic{
	unsigned short            seat_id; //阵法id
	DataArray<unsigned long long, unsigned char> lineup; //阵容顺序
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(seat_id);

		net_data.AddArray(lineup);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(seat_id))
			return false;

		if(!net_data.DelArray(lineup))
			return false;

		return true;
	}
	void Reset(void)
	{
		seat_id = 0;

		lineup.clear();

	}
	LineupBasic& operator= (const LineupBasic& src)
	{
		seat_id = src.seat_id;

		lineup = src.lineup;

		return *this;
	}
	bool operator==(const LineupBasic& src) const
	{
		if (seat_id != src.seat_id)
		{
			return false;
		}

		if (lineup != src.lineup)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LineupBasic& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LineupBasic& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_lineup_handler(m_data.lineup, this),
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
                JsonDelIntegral(m_data.seat_id, str, length);
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
                return &m_lineup_handler;
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

            if (!m_lineup_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LineupBasic& m_data;
        DataArrayHandler<unsigned long long, unsigned char> m_lineup_handler;
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

		json_data.AddIntegral(u8"seat_id", seat_id);

		json_data.Writer().Key(u8"lineup", sizeof(u8"lineup") - 1);
		json_data.AddArray(lineup);

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
        
        LineupBasic::Handler h(*this, nullptr);
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

struct LineupData{
	unsigned char             index; //当前阵容索引
	DataArray<LineupBasic, unsigned char> lineups; //所有阵容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(index);

		net_data.AddArray(lineups);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(index))
			return false;

		if(!net_data.DelArray(lineups))
			return false;

		return true;
	}
	void Reset(void)
	{
		index = 0;

		lineups.clear();

	}
	LineupData& operator= (const LineupData& src)
	{
		index = src.index;

		lineups = src.lineups;

		return *this;
	}
	bool operator==(const LineupData& src) const
	{
		if (index != src.index)
		{
			return false;
		}

		if (lineups != src.lineups)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LineupData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LineupData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_lineups_handler(m_data.lineups, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_lineups_handler.ResetState();
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
            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_lineups_handler;
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

            if (!m_lineups_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LineupData& m_data;
        DataArrayHandler<LineupBasic, unsigned char, LineupBasic::Handler> m_lineups_handler;
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

		json_data.Writer().Key(u8"lineups", sizeof(u8"lineups") - 1);
		json_data.AddArray(lineups);

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
        
        LineupData::Handler h(*this, nullptr);
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

struct InstructionBasic{
	unsigned char             type; //指令类型
	char                      content[32]; //指令内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddString(content, sizeof(content));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelString(content, sizeof(content)))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		content[0] = 0;

	}
	InstructionBasic& operator= (const InstructionBasic& src)
	{
		type = src.type;

		{
			size_t str_len = strnlen(src.content, sizeof(content) - 1);
			memcpy(content, src.content, str_len + 1);
			content[sizeof(content) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const InstructionBasic& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (strncmp(content, src.content, sizeof(content)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const InstructionBasic& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(InstructionBasic& data, JsonHandler* parent = nullptr)
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
            (void)copy;

            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.content, str, length);
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
        InstructionBasic& m_data;
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

		json_data.AddIntegral(u8"type", type);

		json_data.AddString(u8"content", content, sizeof(content));

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
        
        InstructionBasic::Handler h(*this, nullptr);
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

struct PetLineupData{
	DataArray<unsigned long long, unsigned char> lineup; //宠物阵容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(lineup);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(lineup))
			return false;

		return true;
	}
	void Reset(void)
	{
		lineup.clear();

	}
	PetLineupData& operator= (const PetLineupData& src)
	{
		lineup = src.lineup;

		return *this;
	}
	bool operator==(const PetLineupData& src) const
	{
		if (lineup != src.lineup)
		{
			return false;
		}

		return true;
	}
	bool operator != (const PetLineupData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PetLineupData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_lineup_handler(m_data.lineup, this),
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
                return &m_lineup_handler;
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

            if (!m_lineup_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        PetLineupData& m_data;
        DataArrayHandler<unsigned long long, unsigned char> m_lineup_handler;
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

		json_data.Writer().Key(u8"lineup", sizeof(u8"lineup") - 1);
		json_data.AddArray(lineup);

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
        
        PetLineupData::Handler h(*this, nullptr);
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

struct InstructionData{
	DataArray<InstructionBasic, unsigned char> frienddata; //友方指令
	DataArray<InstructionBasic, unsigned char> enemydata; //敌方指令
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(frienddata);

		net_data.AddArray(enemydata);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(frienddata))
			return false;

		if(!net_data.DelArray(enemydata))
			return false;

		return true;
	}
	void Reset(void)
	{
		frienddata.clear();

		enemydata.clear();

	}
	InstructionData& operator= (const InstructionData& src)
	{
		frienddata = src.frienddata;

		enemydata = src.enemydata;

		return *this;
	}
	bool operator==(const InstructionData& src) const
	{
		if (frienddata != src.frienddata)
		{
			return false;
		}

		if (enemydata != src.enemydata)
		{
			return false;
		}

		return true;
	}
	bool operator != (const InstructionData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(InstructionData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_frienddata_handler(m_data.frienddata, this),
            m_enemydata_handler(m_data.enemydata, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_frienddata_handler.ResetState();
            m_enemydata_handler.ResetState();
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
                return &m_frienddata_handler;
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_enemydata_handler;
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

            if (!m_frienddata_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_enemydata_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        InstructionData& m_data;
        DataArrayHandler<InstructionBasic, unsigned char, InstructionBasic::Handler> m_frienddata_handler;
        DataArrayHandler<InstructionBasic, unsigned char, InstructionBasic::Handler> m_enemydata_handler;
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

		json_data.Writer().Key(u8"frienddata", sizeof(u8"frienddata") - 1);
		json_data.AddArray(frienddata);

		json_data.Writer().Key(u8"enemydata", sizeof(u8"enemydata") - 1);
		json_data.AddArray(enemydata);

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
        
        InstructionData::Handler h(*this, nullptr);
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

struct EmojiData{
	DataArray<unsigned short, unsigned int> emojis; //表情包记录
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(emojis);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(emojis))
			return false;

		return true;
	}
	void Reset(void)
	{
		emojis.clear();

	}
	EmojiData& operator= (const EmojiData& src)
	{
		emojis = src.emojis;

		return *this;
	}
	bool operator==(const EmojiData& src) const
	{
		if (emojis != src.emojis)
		{
			return false;
		}

		return true;
	}
	bool operator != (const EmojiData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(EmojiData& data, JsonHandler* parent = nullptr)
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

            if (!m_emojis_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        EmojiData& m_data;
        DataArrayHandler<unsigned short, unsigned int> m_emojis_handler;
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
		json_data.AddArray(emojis);

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
        
        EmojiData::Handler h(*this, nullptr);
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

struct PlayerBrief{
	unsigned int              uid; //账号ID
	unsigned char             sid; //子区ID
	unsigned long long        guid; //玩家GUID
	char                      unid[32]; //玩家UNID
	char                      name[32]; //玩家名字
	DataArray<AttrData, unsigned short> attrs; //属性数据
	unsigned short            map; //地图ID
	unsigned short            x; //x坐标
	unsigned short            y; //y坐标
	unsigned char             state; //玩家状态 0:彻底销毁 1:使用中 2:已删除
	unsigned int              createtime; //创建时间
	unsigned int              lastlogin; //最后一次登陆时间
	unsigned int              destroytime; //销毁时间 state为0=销毁时间 state为2=删除时间
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddString(unid, sizeof(unid));

		net_data.AddString(name, sizeof(name));

		net_data.AddArray(attrs);

		net_data.AddIntegral(map);

		net_data.AddIntegral(x);

		net_data.AddIntegral(y);

		net_data.AddIntegral(state);

		net_data.AddIntegral(createtime);

		net_data.AddIntegral(lastlogin);

		net_data.AddIntegral(destroytime);

		customs.EnCode(net_data);

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

		if (!net_data.DelString(unid, sizeof(unid)))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if (!net_data.DelIntegral(map))
			return false;

		if (!net_data.DelIntegral(x))
			return false;

		if (!net_data.DelIntegral(y))
			return false;

		if (!net_data.DelIntegral(state))
			return false;

		if (!net_data.DelIntegral(createtime))
			return false;

		if (!net_data.DelIntegral(lastlogin))
			return false;

		if (!net_data.DelIntegral(destroytime))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		guid = 0;

		unid[0] = 0;

		name[0] = 0;

		attrs.clear();

		map = 0;

		x = 0;

		y = 0;

		state = 0;

		createtime = 0;

		lastlogin = 0;

		destroytime = 0;

		customs.Reset();

	}
	PlayerBrief& operator= (const PlayerBrief& src)
	{
		uid = src.uid;

		sid = src.sid;

		guid = src.guid;

		{
			size_t str_len = strnlen(src.unid, sizeof(unid) - 1);
			memcpy(unid, src.unid, str_len + 1);
			unid[sizeof(unid) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		attrs = src.attrs;

		map = src.map;

		x = src.x;

		y = src.y;

		state = src.state;

		createtime = src.createtime;

		lastlogin = src.lastlogin;

		destroytime = src.destroytime;

		customs = src.customs;

		return *this;
	}
	bool operator==(const PlayerBrief& src) const
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

		if (strncmp(unid, src.unid, sizeof(unid)-1))
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (map != src.map)
		{
			return false;
		}

		if (x != src.x)
		{
			return false;
		}

		if (y != src.y)
		{
			return false;
		}

		if (state != src.state)
		{
			return false;
		}

		if (createtime != src.createtime)
		{
			return false;
		}

		if (lastlogin != src.lastlogin)
		{
			return false;
		}

		if (destroytime != src.destroytime)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const PlayerBrief& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PlayerBrief& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_attrs_handler(m_data.attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
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
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.map, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.x, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.y, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.state, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.createtime, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.lastlogin, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.destroytime, str, length);
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
                JsonDelString(m_data.unid, str, length);
            }
            break;
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
            switch (m_state)
            {
            case 13:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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
            switch (m_state)
            {
            case 5:
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

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        PlayerBrief& m_data;
        CustomData::Handler m_customs_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<14> m_assigned_bitset;

        static const std::bitset<14> template_assigned_bitset;
        static const char* m_key_list[14];
        static const char* m_alias_list[14];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddString(u8"unid", unid, sizeof(unid));

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.Writer().Key(u8"attrs", sizeof(u8"attrs") - 1);
		json_data.AddArray(attrs);

		json_data.AddIntegral(u8"map", map);

		json_data.AddIntegral(u8"x", x);

		json_data.AddIntegral(u8"y", y);

		json_data.AddIntegral(u8"state", state);

		json_data.AddIntegral(u8"createtime", createtime);

		json_data.AddIntegral(u8"lastlogin", lastlogin);

		json_data.AddIntegral(u8"destroytime", destroytime);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

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
        
        PlayerBrief::Handler h(*this, nullptr);
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

struct GuardData{
	unsigned long long        guid; //侍从GUID
	ItemContainerData         equips; //侍从装备
	DataArray<AttrData, unsigned short> attrs; //属性数据
	DataArray<AttrData, unsigned short> extra_attrs; //额外属性数据
	DataArray<SkillData, unsigned short> skills; //技能
	DataArray<BuffData, unsigned short> buffs; //buff
	DataArray<DynAttrData, unsigned short> dyn_attrs; //动态属性数据
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		equips.EnCode(net_data);

		net_data.AddArray(attrs);

		net_data.AddArray(extra_attrs);

		net_data.AddArray(skills);

		net_data.AddArray(buffs);

		net_data.AddArray(dyn_attrs);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!equips.DeCode(net_data))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if(!net_data.DelArray(extra_attrs))
			return false;

		if(!net_data.DelArray(skills))
			return false;

		if(!net_data.DelArray(buffs))
			return false;

		if(!net_data.DelArray(dyn_attrs))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		equips.Reset();

		attrs.clear();

		extra_attrs.clear();

		skills.clear();

		buffs.clear();

		dyn_attrs.clear();

		customs.Reset();

	}
	GuardData& operator= (const GuardData& src)
	{
		guid = src.guid;

		equips = src.equips;

		attrs = src.attrs;

		extra_attrs = src.extra_attrs;

		skills = src.skills;

		buffs = src.buffs;

		dyn_attrs = src.dyn_attrs;

		customs = src.customs;

		return *this;
	}
	bool operator==(const GuardData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (equips != src.equips)
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (extra_attrs != src.extra_attrs)
		{
			return false;
		}

		if (skills != src.skills)
		{
			return false;
		}

		if (buffs != src.buffs)
		{
			return false;
		}

		if (dyn_attrs != src.dyn_attrs)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GuardData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GuardData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_equips_handler(m_data.equips, this),
            m_customs_handler(m_data.customs, this),
            m_attrs_handler(m_data.attrs, this),
            m_extra_attrs_handler(m_data.extra_attrs, this),
            m_skills_handler(m_data.skills, this),
            m_buffs_handler(m_data.buffs, this),
            m_dyn_attrs_handler(m_data.dyn_attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_equips_handler.ResetState();
            m_customs_handler.ResetState();
            m_attrs_handler.ResetState();
            m_extra_attrs_handler.ResetState();
            m_skills_handler.ResetState();
            m_buffs_handler.ResetState();
            m_dyn_attrs_handler.ResetState();
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
                return &m_equips_handler;
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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
            switch (m_state)
            {
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_attrs_handler;
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_extra_attrs_handler;
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                return &m_skills_handler;
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                return &m_buffs_handler;
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                return &m_dyn_attrs_handler;
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

            if (!m_equips_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_extra_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_skills_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_buffs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_dyn_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        GuardData& m_data;
        ItemContainerData::Handler m_equips_handler;
        CustomData::Handler m_customs_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_extra_attrs_handler;
        DataArrayHandler<SkillData, unsigned short, SkillData::Handler> m_skills_handler;
        DataArrayHandler<BuffData, unsigned short, BuffData::Handler> m_buffs_handler;
        DataArrayHandler<DynAttrData, unsigned short, DynAttrData::Handler> m_dyn_attrs_handler;
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

		json_data.AddIntegral(u8"g", guid);

		json_data.Writer().Key(u8"e", sizeof(u8"e") - 1);
		equips.ToJson(json_data);

		json_data.Writer().Key(u8"a", sizeof(u8"a") - 1);
		json_data.AddArray(attrs);

		json_data.Writer().Key(u8"ea", sizeof(u8"ea") - 1);
		json_data.AddArray(extra_attrs);

		json_data.Writer().Key(u8"s", sizeof(u8"s") - 1);
		json_data.AddArray(skills);

		json_data.Writer().Key(u8"b", sizeof(u8"b") - 1);
		json_data.AddArray(buffs);

		json_data.Writer().Key(u8"da", sizeof(u8"da") - 1);
		json_data.AddArray(dyn_attrs);

		json_data.Writer().Key(u8"c", sizeof(u8"c") - 1);
		customs.ToJson(json_data);

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
        
        GuardData::Handler h(*this, nullptr);
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

struct GuardContainerData{
	DataArray<GuardData, unsigned short> guards; //侍从
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(guards);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(guards))
			return false;

		return true;
	}
	void Reset(void)
	{
		guards.clear();

	}
	GuardContainerData& operator= (const GuardContainerData& src)
	{
		guards = src.guards;

		return *this;
	}
	bool operator==(const GuardContainerData& src) const
	{
		if (guards != src.guards)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GuardContainerData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GuardContainerData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_guards_handler(m_data.guards, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_guards_handler.ResetState();
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
                return &m_guards_handler;
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

            if (!m_guards_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        GuardContainerData& m_data;
        DataArrayHandler<GuardData, unsigned short, GuardData::Handler> m_guards_handler;
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

		json_data.Writer().Key(u8"g", sizeof(u8"g") - 1);
		json_data.AddArray(guards);

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
        
        GuardContainerData::Handler h(*this, nullptr);
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

struct PlayerData{
	unsigned int              uid; //账号ID
	unsigned char             sid; //子区ID
	unsigned long long        guid; //玩家GUID
	char                      unid[32]; //玩家UNID
	char                      name[32]; //玩家名字
	unsigned char             state; //玩家状态 0:彻底销毁 1:使用中 2:已删除
	unsigned int              createtime; //创建时间
	unsigned int              destroytime; //销毁时间 state为0=销毁时间 state为2=删除时间
	unsigned int              lastlogin; //最后一次登陆时间
	unsigned int              lastloginip; //最后一次登陆ip
	unsigned int              lastlogout; //最后一次退出时间
	unsigned int              onlines; //总计在线时间
	SceneData                 cur_map; //当前地图信息
	SceneData                 last_common_map; //最后固定地图信息
	DataArray<AttrData, unsigned short> attrs; //属性数据
	DataArray<DynAttrData, unsigned short> dyn_attrs; //动态属性数据
	DataArray<AttrData, unsigned short> extra_attrs; //额外属性数据
	DataArray<ItemContainerData, unsigned char> items; //物品容器
	DataArray<SkillData, unsigned short> skills; //技能
	DataArray<BuffData, unsigned short> buffs; //buff
	QuestContainerData        quests; //任务
	DataArray<PetContainerData, unsigned short> pets; //宠物
	GuardContainerData        guards; //侍从
	DataArray<TicketData, unsigned short> tickets; //门票
	CustomData                customs; //自定义
	signed long long          gameinyb; //游戏内给予的元宝(不存档)
	signed long long          gameoutyb; //游戏内消耗的元宝(不存档)
	signed long long          billinyb; //充值给予的元宝(不存档)
	signed long long          billoutyb; //充值消耗的元宝(不存档)
	TitleContainerData        titles; //称号
	char                      username[256]; //账号名
	LineupData                lineups; //阵容数据
	SeatData                  seats; //阵法数据
	InstructionData           instructions; //指令数据
	PetLineupData             pet_lineups; //宠物阵容
	EmojiData                 emojis; //表情包阵容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddString(unid, sizeof(unid));

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(state);

		net_data.AddIntegral(createtime);

		net_data.AddIntegral(destroytime);

		net_data.AddIntegral(lastlogin);

		net_data.AddIntegral(lastloginip);

		net_data.AddIntegral(lastlogout);

		net_data.AddIntegral(onlines);

		cur_map.EnCode(net_data);

		last_common_map.EnCode(net_data);

		net_data.AddArray(attrs);

		net_data.AddArray(dyn_attrs);

		net_data.AddArray(extra_attrs);

		net_data.AddArray(items);

		net_data.AddArray(skills);

		net_data.AddArray(buffs);

		quests.EnCode(net_data);

		net_data.AddArray(pets);

		guards.EnCode(net_data);

		net_data.AddArray(tickets);

		customs.EnCode(net_data);

		net_data.AddIntegral(gameinyb);

		net_data.AddIntegral(gameoutyb);

		net_data.AddIntegral(billinyb);

		net_data.AddIntegral(billoutyb);

		titles.EnCode(net_data);

		net_data.AddString(username, sizeof(username));

		lineups.EnCode(net_data);

		seats.EnCode(net_data);

		instructions.EnCode(net_data);

		pet_lineups.EnCode(net_data);

		emojis.EnCode(net_data);

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

		if (!net_data.DelString(unid, sizeof(unid)))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(state))
			return false;

		if (!net_data.DelIntegral(createtime))
			return false;

		if (!net_data.DelIntegral(destroytime))
			return false;

		if (!net_data.DelIntegral(lastlogin))
			return false;

		if (!net_data.DelIntegral(lastloginip))
			return false;

		if (!net_data.DelIntegral(lastlogout))
			return false;

		if (!net_data.DelIntegral(onlines))
			return false;

		if (!cur_map.DeCode(net_data))
			return false;

		if (!last_common_map.DeCode(net_data))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if(!net_data.DelArray(dyn_attrs))
			return false;

		if(!net_data.DelArray(extra_attrs))
			return false;

		if(!net_data.DelArray(items))
			return false;

		if(!net_data.DelArray(skills))
			return false;

		if(!net_data.DelArray(buffs))
			return false;

		if (!quests.DeCode(net_data))
			return false;

		if(!net_data.DelArray(pets))
			return false;

		if (!guards.DeCode(net_data))
			return false;

		if(!net_data.DelArray(tickets))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(gameinyb))
			return false;

		if (!net_data.DelIntegral(gameoutyb))
			return false;

		if (!net_data.DelIntegral(billinyb))
			return false;

		if (!net_data.DelIntegral(billoutyb))
			return false;

		if (!titles.DeCode(net_data))
			return false;

		if (!net_data.DelString(username, sizeof(username)))
			return false;

		if (!lineups.DeCode(net_data))
			return false;

		if (!seats.DeCode(net_data))
			return false;

		if (!instructions.DeCode(net_data))
			return false;

		if (!pet_lineups.DeCode(net_data))
			return false;

		if (!emojis.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		guid = 0;

		unid[0] = 0;

		name[0] = 0;

		state = 0;

		createtime = 0;

		destroytime = 0;

		lastlogin = 0;

		lastloginip = 0;

		lastlogout = 0;

		onlines = 0;

		cur_map.Reset();

		last_common_map.Reset();

		attrs.clear();

		dyn_attrs.clear();

		extra_attrs.clear();

		items.clear();

		skills.clear();

		buffs.clear();

		quests.Reset();

		pets.clear();

		guards.Reset();

		tickets.clear();

		customs.Reset();

		gameinyb = 0;

		gameoutyb = 0;

		billinyb = 0;

		billoutyb = 0;

		titles.Reset();

		username[0] = 0;

		lineups.Reset();

		seats.Reset();

		instructions.Reset();

		pet_lineups.Reset();

		emojis.Reset();

	}
	PlayerData& operator= (const PlayerData& src)
	{
		uid = src.uid;

		sid = src.sid;

		guid = src.guid;

		{
			size_t str_len = strnlen(src.unid, sizeof(unid) - 1);
			memcpy(unid, src.unid, str_len + 1);
			unid[sizeof(unid) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		state = src.state;

		createtime = src.createtime;

		destroytime = src.destroytime;

		lastlogin = src.lastlogin;

		lastloginip = src.lastloginip;

		lastlogout = src.lastlogout;

		onlines = src.onlines;

		cur_map = src.cur_map;

		last_common_map = src.last_common_map;

		attrs = src.attrs;

		dyn_attrs = src.dyn_attrs;

		extra_attrs = src.extra_attrs;

		items = src.items;

		skills = src.skills;

		buffs = src.buffs;

		quests = src.quests;

		pets = src.pets;

		guards = src.guards;

		tickets = src.tickets;

		customs = src.customs;

		gameinyb = src.gameinyb;

		gameoutyb = src.gameoutyb;

		billinyb = src.billinyb;

		billoutyb = src.billoutyb;

		titles = src.titles;

		{
			size_t str_len = strnlen(src.username, sizeof(username) - 1);
			memcpy(username, src.username, str_len + 1);
			username[sizeof(username) - 1] = 0;
		}

		lineups = src.lineups;

		seats = src.seats;

		instructions = src.instructions;

		pet_lineups = src.pet_lineups;

		emojis = src.emojis;

		return *this;
	}
	bool operator==(const PlayerData& src) const
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

		if (strncmp(unid, src.unid, sizeof(unid)-1))
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (state != src.state)
		{
			return false;
		}

		if (createtime != src.createtime)
		{
			return false;
		}

		if (destroytime != src.destroytime)
		{
			return false;
		}

		if (lastlogin != src.lastlogin)
		{
			return false;
		}

		if (lastloginip != src.lastloginip)
		{
			return false;
		}

		if (lastlogout != src.lastlogout)
		{
			return false;
		}

		if (onlines != src.onlines)
		{
			return false;
		}

		if (cur_map != src.cur_map)
		{
			return false;
		}

		if (last_common_map != src.last_common_map)
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (dyn_attrs != src.dyn_attrs)
		{
			return false;
		}

		if (extra_attrs != src.extra_attrs)
		{
			return false;
		}

		if (items != src.items)
		{
			return false;
		}

		if (skills != src.skills)
		{
			return false;
		}

		if (buffs != src.buffs)
		{
			return false;
		}

		if (quests != src.quests)
		{
			return false;
		}

		if (pets != src.pets)
		{
			return false;
		}

		if (guards != src.guards)
		{
			return false;
		}

		if (tickets != src.tickets)
		{
			return false;
		}

		if (customs != src.customs)
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

		if (billinyb != src.billinyb)
		{
			return false;
		}

		if (billoutyb != src.billoutyb)
		{
			return false;
		}

		if (titles != src.titles)
		{
			return false;
		}

		if (strncmp(username, src.username, sizeof(username)-1))
		{
			return false;
		}

		if (lineups != src.lineups)
		{
			return false;
		}

		if (seats != src.seats)
		{
			return false;
		}

		if (instructions != src.instructions)
		{
			return false;
		}

		if (pet_lineups != src.pet_lineups)
		{
			return false;
		}

		if (emojis != src.emojis)
		{
			return false;
		}

		return true;
	}
	bool operator != (const PlayerData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PlayerData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_cur_map_handler(m_data.cur_map, this),
            m_last_common_map_handler(m_data.last_common_map, this),
            m_quests_handler(m_data.quests, this),
            m_guards_handler(m_data.guards, this),
            m_customs_handler(m_data.customs, this),
            m_titles_handler(m_data.titles, this),
            m_lineups_handler(m_data.lineups, this),
            m_seats_handler(m_data.seats, this),
            m_instructions_handler(m_data.instructions, this),
            m_pet_lineups_handler(m_data.pet_lineups, this),
            m_emojis_handler(m_data.emojis, this),
            m_attrs_handler(m_data.attrs, this),
            m_dyn_attrs_handler(m_data.dyn_attrs, this),
            m_extra_attrs_handler(m_data.extra_attrs, this),
            m_items_handler(m_data.items, this),
            m_skills_handler(m_data.skills, this),
            m_buffs_handler(m_data.buffs, this),
            m_pets_handler(m_data.pets, this),
            m_tickets_handler(m_data.tickets, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_cur_map_handler.ResetState();
            m_last_common_map_handler.ResetState();
            m_quests_handler.ResetState();
            m_guards_handler.ResetState();
            m_customs_handler.ResetState();
            m_titles_handler.ResetState();
            m_lineups_handler.ResetState();
            m_seats_handler.ResetState();
            m_instructions_handler.ResetState();
            m_pet_lineups_handler.ResetState();
            m_emojis_handler.ResetState();
            m_attrs_handler.ResetState();
            m_dyn_attrs_handler.ResetState();
            m_extra_attrs_handler.ResetState();
            m_items_handler.ResetState();
            m_skills_handler.ResetState();
            m_buffs_handler.ResetState();
            m_pets_handler.ResetState();
            m_tickets_handler.ResetState();
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
                JsonDelIntegral(m_data.state, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.createtime, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.destroytime, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.lastlogin, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.lastloginip, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.lastlogout, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.onlines, str, length);
            }
            break;
            case 25:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gameinyb, str, length);
            }
            break;
            case 26:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gameoutyb, str, length);
            }
            break;
            case 27:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.billinyb, str, length);
            }
            break;
            case 28:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.billoutyb, str, length);
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
                JsonDelString(m_data.unid, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.name, str, length);
            }
            break;
            case 30:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.username, str, length);
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
            case 12:
            {
                m_assigned_bitset.set(m_state);
                return &m_cur_map_handler;
            }
            break;
            case 13:
            {
                m_assigned_bitset.set(m_state);
                return &m_last_common_map_handler;
            }
            break;
            case 20:
            {
                m_assigned_bitset.set(m_state);
                return &m_quests_handler;
            }
            break;
            case 22:
            {
                m_assigned_bitset.set(m_state);
                return &m_guards_handler;
            }
            break;
            case 24:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
            }
            break;
            case 29:
            {
                m_assigned_bitset.set(m_state);
                return &m_titles_handler;
            }
            break;
            case 31:
            {
                m_assigned_bitset.set(m_state);
                return &m_lineups_handler;
            }
            break;
            case 32:
            {
                m_assigned_bitset.set(m_state);
                return &m_seats_handler;
            }
            break;
            case 33:
            {
                m_assigned_bitset.set(m_state);
                return &m_instructions_handler;
            }
            break;
            case 34:
            {
                m_assigned_bitset.set(m_state);
                return &m_pet_lineups_handler;
            }
            break;
            case 35:
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
            switch (m_state)
            {
            case 14:
            {
                m_assigned_bitset.set(m_state);
                return &m_attrs_handler;
            }
            break;
            case 15:
            {
                m_assigned_bitset.set(m_state);
                return &m_dyn_attrs_handler;
            }
            break;
            case 16:
            {
                m_assigned_bitset.set(m_state);
                return &m_extra_attrs_handler;
            }
            break;
            case 17:
            {
                m_assigned_bitset.set(m_state);
                return &m_items_handler;
            }
            break;
            case 18:
            {
                m_assigned_bitset.set(m_state);
                return &m_skills_handler;
            }
            break;
            case 19:
            {
                m_assigned_bitset.set(m_state);
                return &m_buffs_handler;
            }
            break;
            case 21:
            {
                m_assigned_bitset.set(m_state);
                return &m_pets_handler;
            }
            break;
            case 23:
            {
                m_assigned_bitset.set(m_state);
                return &m_tickets_handler;
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
            if (((template_assigned_bitset & m_assigned_bitset) ^ template_assigned_bitset).any())
            {
                return false;
            }

            if (!m_cur_map_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_last_common_map_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_dyn_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_extra_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_items_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_skills_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_buffs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_quests_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_pets_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_guards_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_tickets_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_titles_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_lineups_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_seats_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_instructions_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_pet_lineups_handler.IsAllMemberSet())
            {
                return false;
            }

            if (m_assigned_bitset.test(35))
            {
                if (!m_emojis_handler.IsAllMemberSet())
                {
                    return false;
                }

            }
            return true;
        }
    private:
        PlayerData& m_data;
        SceneData::Handler m_cur_map_handler;
        SceneData::Handler m_last_common_map_handler;
        QuestContainerData::Handler m_quests_handler;
        GuardContainerData::Handler m_guards_handler;
        CustomData::Handler m_customs_handler;
        TitleContainerData::Handler m_titles_handler;
        LineupData::Handler m_lineups_handler;
        SeatData::Handler m_seats_handler;
        InstructionData::Handler m_instructions_handler;
        PetLineupData::Handler m_pet_lineups_handler;
        EmojiData::Handler m_emojis_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
        DataArrayHandler<DynAttrData, unsigned short, DynAttrData::Handler> m_dyn_attrs_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_extra_attrs_handler;
        DataArrayHandler<ItemContainerData, unsigned char, ItemContainerData::Handler> m_items_handler;
        DataArrayHandler<SkillData, unsigned short, SkillData::Handler> m_skills_handler;
        DataArrayHandler<BuffData, unsigned short, BuffData::Handler> m_buffs_handler;
        DataArrayHandler<PetContainerData, unsigned short, PetContainerData::Handler> m_pets_handler;
        DataArrayHandler<TicketData, unsigned short, TicketData::Handler> m_tickets_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<36> m_assigned_bitset;

        static const std::bitset<36> template_assigned_bitset;
        static const char* m_key_list[36];
        static const char* m_alias_list[36];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddString(u8"unid", unid, sizeof(unid));

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddIntegral(u8"state", state);

		json_data.AddIntegral(u8"createtime", createtime);

		json_data.AddIntegral(u8"destroytime", destroytime);

		json_data.AddIntegral(u8"lastlogin", lastlogin);

		json_data.AddIntegral(u8"lastloginip", lastloginip);

		json_data.AddIntegral(u8"lastlogout", lastlogout);

		json_data.AddIntegral(u8"onlines", onlines);

		json_data.Writer().Key(u8"cur_map", sizeof(u8"cur_map") - 1);
		cur_map.ToJson(json_data);

		json_data.Writer().Key(u8"last_common_map", sizeof(u8"last_common_map") - 1);
		last_common_map.ToJson(json_data);

		json_data.Writer().Key(u8"attrs", sizeof(u8"attrs") - 1);
		json_data.AddArray(attrs);

		json_data.Writer().Key(u8"dyn_attrs", sizeof(u8"dyn_attrs") - 1);
		json_data.AddArray(dyn_attrs);

		json_data.Writer().Key(u8"extra_attrs", sizeof(u8"extra_attrs") - 1);
		json_data.AddArray(extra_attrs);

		json_data.Writer().Key(u8"items", sizeof(u8"items") - 1);
		json_data.AddArray(items);

		json_data.Writer().Key(u8"skills", sizeof(u8"skills") - 1);
		json_data.AddArray(skills);

		json_data.Writer().Key(u8"buffs", sizeof(u8"buffs") - 1);
		json_data.AddArray(buffs);

		json_data.Writer().Key(u8"quests", sizeof(u8"quests") - 1);
		quests.ToJson(json_data);

		json_data.Writer().Key(u8"pets", sizeof(u8"pets") - 1);
		json_data.AddArray(pets);

		json_data.Writer().Key(u8"guards", sizeof(u8"guards") - 1);
		guards.ToJson(json_data);

		json_data.Writer().Key(u8"tickets", sizeof(u8"tickets") - 1);
		json_data.AddArray(tickets);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

		json_data.AddIntegral(u8"gameinyb", gameinyb);

		json_data.AddIntegral(u8"gameoutyb", gameoutyb);

		json_data.AddIntegral(u8"billinyb", billinyb);

		json_data.AddIntegral(u8"billoutyb", billoutyb);

		json_data.Writer().Key(u8"titles", sizeof(u8"titles") - 1);
		titles.ToJson(json_data);

		json_data.AddString(u8"username", username, sizeof(username));

		json_data.Writer().Key(u8"lineups", sizeof(u8"lineups") - 1);
		lineups.ToJson(json_data);

		json_data.Writer().Key(u8"seats", sizeof(u8"seats") - 1);
		seats.ToJson(json_data);

		json_data.Writer().Key(u8"instructions", sizeof(u8"instructions") - 1);
		instructions.ToJson(json_data);

		json_data.Writer().Key(u8"pet_lineups", sizeof(u8"pet_lineups") - 1);
		pet_lineups.ToJson(json_data);

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
        
        PlayerData::Handler h(*this, nullptr);
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

struct MapRegion{
	unsigned short            min_x; //最小x坐标
	unsigned short            max_x; //最大x坐标
	unsigned short            min_y; //最小y坐标
	unsigned short            max_y; //最大y坐标
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(min_x);

		net_data.AddIntegral(max_x);

		net_data.AddIntegral(min_y);

		net_data.AddIntegral(max_y);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(min_x))
			return false;

		if (!net_data.DelIntegral(max_x))
			return false;

		if (!net_data.DelIntegral(min_y))
			return false;

		if (!net_data.DelIntegral(max_y))
			return false;

		return true;
	}
	void Reset(void)
	{
		min_x = 0;

		max_x = 0;

		min_y = 0;

		max_y = 0;

	}
	MapRegion& operator= (const MapRegion& src)
	{
		min_x = src.min_x;

		max_x = src.max_x;

		min_y = src.min_y;

		max_y = src.max_y;

		return *this;
	}
	bool operator==(const MapRegion& src) const
	{
		if (min_x != src.min_x)
		{
			return false;
		}

		if (max_x != src.max_x)
		{
			return false;
		}

		if (min_y != src.min_y)
		{
			return false;
		}

		if (max_y != src.max_y)
		{
			return false;
		}

		return true;
	}
	bool operator != (const MapRegion& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(MapRegion& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.min_x, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.max_x, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.min_y, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.max_y, str, length);
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
        MapRegion& m_data;
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

		json_data.AddIntegral(u8"min_x", min_x);

		json_data.AddIntegral(u8"max_x", max_x);

		json_data.AddIntegral(u8"min_y", min_y);

		json_data.AddIntegral(u8"max_y", max_y);

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
        
        MapRegion::Handler h(*this, nullptr);
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

struct ItemAttrValue64{
	unsigned int              attr_type; //物品属性类型
	signed long long          attr_value; //物品属性值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(attr_type);

		net_data.AddIntegral(attr_value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(attr_type))
			return false;

		if (!net_data.DelIntegral(attr_value))
			return false;

		return true;
	}
	void Reset(void)
	{
		attr_type = 0;

		attr_value = 0;

	}
	ItemAttrValue64& operator= (const ItemAttrValue64& src)
	{
		attr_type = src.attr_type;

		attr_value = src.attr_value;

		return *this;
	}
	bool operator==(const ItemAttrValue64& src) const
	{
		if (attr_type != src.attr_type)
		{
			return false;
		}

		if (attr_value != src.attr_value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ItemAttrValue64& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ItemAttrValue64& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.attr_type, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.attr_value, str, length);
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
        ItemAttrValue64& m_data;
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

		json_data.AddIntegral(u8"attr_type", attr_type);

		json_data.AddIntegral(u8"attr_value", attr_value);

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
        
        ItemAttrValue64::Handler h(*this, nullptr);
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

struct ItemAttrValueS{
	unsigned int              attr_type; //物品属性类型
	DataArray<unsigned char, unsigned short> attr_value; //物品属性值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(attr_type);

		net_data.AddArray(attr_value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(attr_type))
			return false;

		if(!net_data.DelArray(attr_value))
			return false;

		return true;
	}
	void Reset(void)
	{
		attr_type = 0;

		attr_value.clear();

	}
	ItemAttrValueS& operator= (const ItemAttrValueS& src)
	{
		attr_type = src.attr_type;

		attr_value = src.attr_value;

		return *this;
	}
	bool operator==(const ItemAttrValueS& src) const
	{
		if (attr_type != src.attr_type)
		{
			return false;
		}

		if (attr_value != src.attr_value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ItemAttrValueS& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ItemAttrValueS& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.attr_type, str, length);
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.attr_value, this);
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
        ItemAttrValueS& m_data;
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

		json_data.AddIntegral(u8"attr_type", attr_type);

		json_data.Writer().Key(u8"attr_value", sizeof(u8"attr_value") - 1);
		json_data.AddArray(attr_value);

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
        
        ItemAttrValueS::Handler h(*this, nullptr);
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

struct ItemAttrValueS64{
	char                      attr_key[256]; //key
	signed long long          attr_value; //物品属性值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(attr_key, sizeof(attr_key));

		net_data.AddIntegral(attr_value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(attr_key, sizeof(attr_key)))
			return false;

		if (!net_data.DelIntegral(attr_value))
			return false;

		return true;
	}
	void Reset(void)
	{
		attr_key[0] = 0;

		attr_value = 0;

	}
	ItemAttrValueS64& operator= (const ItemAttrValueS64& src)
	{
		{
			size_t str_len = strnlen(src.attr_key, sizeof(attr_key) - 1);
			memcpy(attr_key, src.attr_key, str_len + 1);
			attr_key[sizeof(attr_key) - 1] = 0;
		}

		attr_value = src.attr_value;

		return *this;
	}
	bool operator==(const ItemAttrValueS64& src) const
	{
		if (strncmp(attr_key, src.attr_key, sizeof(attr_key)-1))
		{
			return false;
		}

		if (attr_value != src.attr_value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ItemAttrValueS64& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ItemAttrValueS64& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.attr_value, str, length);
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
                JsonDelString(m_data.attr_key, str, length);
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
        ItemAttrValueS64& m_data;
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

		json_data.AddString(u8"attr_key", attr_key, sizeof(attr_key));

		json_data.AddIntegral(u8"attr_value", attr_value);

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
        
        ItemAttrValueS64::Handler h(*this, nullptr);
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

struct ItemAttrValueSS{
	char                      attr_key[256]; //key
	DataArray<unsigned char, unsigned short> attr_value; //物品属性值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(attr_key, sizeof(attr_key));

		net_data.AddArray(attr_value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(attr_key, sizeof(attr_key)))
			return false;

		if(!net_data.DelArray(attr_value))
			return false;

		return true;
	}
	void Reset(void)
	{
		attr_key[0] = 0;

		attr_value.clear();

	}
	ItemAttrValueSS& operator= (const ItemAttrValueSS& src)
	{
		{
			size_t str_len = strnlen(src.attr_key, sizeof(attr_key) - 1);
			memcpy(attr_key, src.attr_key, str_len + 1);
			attr_key[sizeof(attr_key) - 1] = 0;
		}

		attr_value = src.attr_value;

		return *this;
	}
	bool operator==(const ItemAttrValueSS& src) const
	{
		if (strncmp(attr_key, src.attr_key, sizeof(attr_key)-1))
		{
			return false;
		}

		if (attr_value != src.attr_value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ItemAttrValueSS& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ItemAttrValueSS& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.attr_key, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.attr_value, this);
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
        ItemAttrValueSS& m_data;
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

		json_data.AddString(u8"attr_key", attr_key, sizeof(attr_key));

		json_data.Writer().Key(u8"attr_value", sizeof(u8"attr_value") - 1);
		json_data.AddArray(attr_value);

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
        
        ItemAttrValueSS::Handler h(*this, nullptr);
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

struct ItemAttrValueList{
	unsigned long long        item_guid; //物品guid
	DataArray<ItemAttrValue64, unsigned short> item_values_64; //物品属性值列表:U64
	DataArray<ItemAttrValueS, unsigned short> item_values_s; //物品属性值列表:S
	DataArray<ItemAttrValueS64, unsigned short> item_values_s64; //物品属性值列表:SU64
	DataArray<ItemAttrValueSS, unsigned short> item_values_ss; //物品属性值列表:SS
	DataArray<DynAttrData, unsigned short> dyn_attrs; //物品属性值列表:动态属性
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(item_guid);

		net_data.AddArray(item_values_64);

		net_data.AddArray(item_values_s);

		net_data.AddArray(item_values_s64);

		net_data.AddArray(item_values_ss);

		net_data.AddArray(dyn_attrs);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(item_guid))
			return false;

		if(!net_data.DelArray(item_values_64))
			return false;

		if(!net_data.DelArray(item_values_s))
			return false;

		if(!net_data.DelArray(item_values_s64))
			return false;

		if(!net_data.DelArray(item_values_ss))
			return false;

		if(!net_data.DelArray(dyn_attrs))
			return false;

		return true;
	}
	void Reset(void)
	{
		item_guid = 0;

		item_values_64.clear();

		item_values_s.clear();

		item_values_s64.clear();

		item_values_ss.clear();

		dyn_attrs.clear();

	}
	ItemAttrValueList& operator= (const ItemAttrValueList& src)
	{
		item_guid = src.item_guid;

		item_values_64 = src.item_values_64;

		item_values_s = src.item_values_s;

		item_values_s64 = src.item_values_s64;

		item_values_ss = src.item_values_ss;

		dyn_attrs = src.dyn_attrs;

		return *this;
	}
	bool operator==(const ItemAttrValueList& src) const
	{
		if (item_guid != src.item_guid)
		{
			return false;
		}

		if (item_values_64 != src.item_values_64)
		{
			return false;
		}

		if (item_values_s != src.item_values_s)
		{
			return false;
		}

		if (item_values_s64 != src.item_values_s64)
		{
			return false;
		}

		if (item_values_ss != src.item_values_ss)
		{
			return false;
		}

		if (dyn_attrs != src.dyn_attrs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ItemAttrValueList& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ItemAttrValueList& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_item_values_64_handler(m_data.item_values_64, this),
            m_item_values_s_handler(m_data.item_values_s, this),
            m_item_values_s64_handler(m_data.item_values_s64, this),
            m_item_values_ss_handler(m_data.item_values_ss, this),
            m_dyn_attrs_handler(m_data.dyn_attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_item_values_64_handler.ResetState();
            m_item_values_s_handler.ResetState();
            m_item_values_s64_handler.ResetState();
            m_item_values_ss_handler.ResetState();
            m_dyn_attrs_handler.ResetState();
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
                JsonDelIntegral(m_data.item_guid, str, length);
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
                return &m_item_values_64_handler;
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_item_values_s_handler;
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_item_values_s64_handler;
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                return &m_item_values_ss_handler;
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                return &m_dyn_attrs_handler;
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

            if (!m_item_values_64_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_item_values_s_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_item_values_s64_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_item_values_ss_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_dyn_attrs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ItemAttrValueList& m_data;
        DataArrayHandler<ItemAttrValue64, unsigned short, ItemAttrValue64::Handler> m_item_values_64_handler;
        DataArrayHandler<ItemAttrValueS, unsigned short, ItemAttrValueS::Handler> m_item_values_s_handler;
        DataArrayHandler<ItemAttrValueS64, unsigned short, ItemAttrValueS64::Handler> m_item_values_s64_handler;
        DataArrayHandler<ItemAttrValueSS, unsigned short, ItemAttrValueSS::Handler> m_item_values_ss_handler;
        DataArrayHandler<DynAttrData, unsigned short, DynAttrData::Handler> m_dyn_attrs_handler;
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

		json_data.AddIntegral(u8"item_guid", item_guid);

		json_data.Writer().Key(u8"item_values_64", sizeof(u8"item_values_64") - 1);
		json_data.AddArray(item_values_64);

		json_data.Writer().Key(u8"item_values_s", sizeof(u8"item_values_s") - 1);
		json_data.AddArray(item_values_s);

		json_data.Writer().Key(u8"item_values_s64", sizeof(u8"item_values_s64") - 1);
		json_data.AddArray(item_values_s64);

		json_data.Writer().Key(u8"item_values_ss", sizeof(u8"item_values_ss") - 1);
		json_data.AddArray(item_values_ss);

		json_data.Writer().Key(u8"dyn_attrs", sizeof(u8"dyn_attrs") - 1);
		json_data.AddArray(dyn_attrs);

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
        
        ItemAttrValueList::Handler h(*this, nullptr);
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

struct ChatMsg{
	char                      role_name[256]; //角色名字
	unsigned int              role_id; //角色模板ID
	DataArray<unsigned char, unsigned short> message; //消息内容
	unsigned int              vip; //vip等级
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(role_name, sizeof(role_name));

		net_data.AddIntegral(role_id);

		net_data.AddArray(message);

		net_data.AddIntegral(vip);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(role_name, sizeof(role_name)))
			return false;

		if (!net_data.DelIntegral(role_id))
			return false;

		if(!net_data.DelArray(message))
			return false;

		if (!net_data.DelIntegral(vip))
			return false;

		return true;
	}
	void Reset(void)
	{
		role_name[0] = 0;

		role_id = 0;

		message.clear();

		vip = 0;

	}
	ChatMsg& operator= (const ChatMsg& src)
	{
		{
			size_t str_len = strnlen(src.role_name, sizeof(role_name) - 1);
			memcpy(role_name, src.role_name, str_len + 1);
			role_name[sizeof(role_name) - 1] = 0;
		}

		role_id = src.role_id;

		message = src.message;

		vip = src.vip;

		return *this;
	}
	bool operator==(const ChatMsg& src) const
	{
		if (strncmp(role_name, src.role_name, sizeof(role_name)-1))
		{
			return false;
		}

		if (role_id != src.role_id)
		{
			return false;
		}

		if (message != src.message)
		{
			return false;
		}

		if (vip != src.vip)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ChatMsg& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ChatMsg& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.role_id, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.vip, str, length);
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
                JsonDelString(m_data.role_name, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.message, this);
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
        ChatMsg& m_data;
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

		json_data.AddString(u8"role_name", role_name, sizeof(role_name));

		json_data.AddIntegral(u8"role_id", role_id);

		json_data.Writer().Key(u8"message", sizeof(u8"message") - 1);
		json_data.AddArray(message);

		json_data.AddIntegral(u8"vip", vip);

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
        
        ChatMsg::Handler h(*this, nullptr);
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

struct ParamData{
	unsigned char             type; //数据类型 1=INT8 2=UINT8 3=INT16 4=UINT16 5=INT32 6=UINT32 7=INT64 8=UINT64 9=字符串
	DataArray<unsigned char, unsigned short> param; //命令参数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddArray(param);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if(!net_data.DelArray(param))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		param.clear();

	}
	ParamData& operator= (const ParamData& src)
	{
		type = src.type;

		param = src.param;

		return *this;
	}
	bool operator==(const ParamData& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (param != src.param)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ParamData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ParamData& data, JsonHandler* parent = nullptr)
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
            (void)copy;

            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.param, this);
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
        ParamData& m_data;
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

		json_data.AddIntegral(u8"type", type);

		json_data.Writer().Key(u8"param", sizeof(u8"param") - 1);
		json_data.AddArray(param);

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
        
        ParamData::Handler h(*this, nullptr);
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

struct FightPetData{
	unsigned long long        guid; //宠物guid
	unsigned char             state; //0:不可召出 1:可召出
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(state);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(state))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		state = 0;

	}
	FightPetData& operator= (const FightPetData& src)
	{
		guid = src.guid;

		state = src.state;

		return *this;
	}
	bool operator==(const FightPetData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (state != src.state)
		{
			return false;
		}

		return true;
	}
	bool operator != (const FightPetData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(FightPetData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.state, str, length);
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
        FightPetData& m_data;
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

		json_data.AddIntegral(u8"state", state);

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
        
        FightPetData::Handler h(*this, nullptr);
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

struct FightBuffData{
	unsigned short            id; //buff id
	unsigned char             stack; //堆叠数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(id);

		net_data.AddIntegral(stack);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(stack))
			return false;

		return true;
	}
	void Reset(void)
	{
		id = 0;

		stack = 0;

	}
	FightBuffData& operator= (const FightBuffData& src)
	{
		id = src.id;

		stack = src.stack;

		return *this;
	}
	bool operator==(const FightBuffData& src) const
	{
		if (id != src.id)
		{
			return false;
		}

		if (stack != src.stack)
		{
			return false;
		}

		return true;
	}
	bool operator != (const FightBuffData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(FightBuffData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.stack, str, length);
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
        FightBuffData& m_data;
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

		json_data.AddIntegral(u8"id", id);

		json_data.AddIntegral(u8"stack", stack);

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
        
        FightBuffData::Handler h(*this, nullptr);
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

struct FighterData{
	unsigned char             site; //战斗者站位
	unsigned short            fighter_id; //战斗者id
	unsigned long long        guid; //guid
	unsigned char             type; //对象类型
	unsigned short            template_id; //模板id
	char                      name[32]; //名称
	DataArray<AttrData, unsigned short> attrs; //属性数据
	DataArray<FightPetData, unsigned char> pets; //宠物召出记录
	DataArray<FightBuffData, unsigned char> buffs; //战斗buff数据
	unsigned int              model; //默认模型,玩家填0
	char                      friend_instruction[32]; //显示给友方看的指令
	char                      enemy_instruction[32]; //显示给敌方看的指令
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(site);

		net_data.AddIntegral(fighter_id);

		net_data.AddIntegral(guid);

		net_data.AddIntegral(type);

		net_data.AddIntegral(template_id);

		net_data.AddString(name, sizeof(name));

		net_data.AddArray(attrs);

		net_data.AddArray(pets);

		net_data.AddArray(buffs);

		net_data.AddIntegral(model);

		net_data.AddString(friend_instruction, sizeof(friend_instruction));

		net_data.AddString(enemy_instruction, sizeof(enemy_instruction));

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(site))
			return false;

		if (!net_data.DelIntegral(fighter_id))
			return false;

		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(template_id))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if(!net_data.DelArray(pets))
			return false;

		if(!net_data.DelArray(buffs))
			return false;

		if (!net_data.DelIntegral(model))
			return false;

		if (!net_data.DelString(friend_instruction, sizeof(friend_instruction)))
			return false;

		if (!net_data.DelString(enemy_instruction, sizeof(enemy_instruction)))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		site = 0;

		fighter_id = 0;

		guid = 0;

		type = 0;

		template_id = 0;

		name[0] = 0;

		attrs.clear();

		pets.clear();

		buffs.clear();

		model = 0;

		friend_instruction[0] = 0;

		enemy_instruction[0] = 0;

		customs.Reset();

	}
	FighterData& operator= (const FighterData& src)
	{
		site = src.site;

		fighter_id = src.fighter_id;

		guid = src.guid;

		type = src.type;

		template_id = src.template_id;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		attrs = src.attrs;

		pets = src.pets;

		buffs = src.buffs;

		model = src.model;

		{
			size_t str_len = strnlen(src.friend_instruction, sizeof(friend_instruction) - 1);
			memcpy(friend_instruction, src.friend_instruction, str_len + 1);
			friend_instruction[sizeof(friend_instruction) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.enemy_instruction, sizeof(enemy_instruction) - 1);
			memcpy(enemy_instruction, src.enemy_instruction, str_len + 1);
			enemy_instruction[sizeof(enemy_instruction) - 1] = 0;
		}

		customs = src.customs;

		return *this;
	}
	bool operator==(const FighterData& src) const
	{
		if (site != src.site)
		{
			return false;
		}

		if (fighter_id != src.fighter_id)
		{
			return false;
		}

		if (guid != src.guid)
		{
			return false;
		}

		if (type != src.type)
		{
			return false;
		}

		if (template_id != src.template_id)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (pets != src.pets)
		{
			return false;
		}

		if (buffs != src.buffs)
		{
			return false;
		}

		if (model != src.model)
		{
			return false;
		}

		if (strncmp(friend_instruction, src.friend_instruction, sizeof(friend_instruction)-1))
		{
			return false;
		}

		if (strncmp(enemy_instruction, src.enemy_instruction, sizeof(enemy_instruction)-1))
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const FighterData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(FighterData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_attrs_handler(m_data.attrs, this),
            m_pets_handler(m_data.pets, this),
            m_buffs_handler(m_data.buffs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
            m_attrs_handler.ResetState();
            m_pets_handler.ResetState();
            m_buffs_handler.ResetState();
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
                JsonDelIntegral(m_data.site, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.fighter_id, str, length);
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
                JsonDelIntegral(m_data.type, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.template_id, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.model, str, length);
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
                JsonDelString(m_data.name, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.friend_instruction, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.enemy_instruction, str, length);
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
            case 12:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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
            switch (m_state)
            {
            case 6:
            {
                m_assigned_bitset.set(m_state);
                return &m_attrs_handler;
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                return &m_pets_handler;
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                return &m_buffs_handler;
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

            if (!m_pets_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_buffs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        FighterData& m_data;
        CustomData::Handler m_customs_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
        DataArrayHandler<FightPetData, unsigned char, FightPetData::Handler> m_pets_handler;
        DataArrayHandler<FightBuffData, unsigned char, FightBuffData::Handler> m_buffs_handler;
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

		json_data.AddIntegral(u8"site", site);

		json_data.AddIntegral(u8"fighter_id", fighter_id);

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"type", type);

		json_data.AddIntegral(u8"template_id", template_id);

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.Writer().Key(u8"attrs", sizeof(u8"attrs") - 1);
		json_data.AddArray(attrs);

		json_data.Writer().Key(u8"pets", sizeof(u8"pets") - 1);
		json_data.AddArray(pets);

		json_data.Writer().Key(u8"buffs", sizeof(u8"buffs") - 1);
		json_data.AddArray(buffs);

		json_data.AddIntegral(u8"model", model);

		json_data.AddString(u8"friend_instruction", friend_instruction, sizeof(friend_instruction));

		json_data.AddString(u8"enemy_instruction", enemy_instruction, sizeof(enemy_instruction));

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

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
        
        FighterData::Handler h(*this, nullptr);
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

struct FightGroupData{
	unsigned char             type; //0=观战组 1=组1 2=组2
	unsigned short            lineup_id; //阵型id
	unsigned short            lineup_lv; //阵型lv
	DataArray<FighterData, unsigned char> fighters; //战斗者数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(lineup_id);

		net_data.AddIntegral(lineup_lv);

		net_data.AddArray(fighters);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(lineup_id))
			return false;

		if (!net_data.DelIntegral(lineup_lv))
			return false;

		if(!net_data.DelArray(fighters))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		lineup_id = 0;

		lineup_lv = 0;

		fighters.clear();

	}
	FightGroupData& operator= (const FightGroupData& src)
	{
		type = src.type;

		lineup_id = src.lineup_id;

		lineup_lv = src.lineup_lv;

		fighters = src.fighters;

		return *this;
	}
	bool operator==(const FightGroupData& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (lineup_id != src.lineup_id)
		{
			return false;
		}

		if (lineup_lv != src.lineup_lv)
		{
			return false;
		}

		if (fighters != src.fighters)
		{
			return false;
		}

		return true;
	}
	bool operator != (const FightGroupData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(FightGroupData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_fighters_handler(m_data.fighters, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_fighters_handler.ResetState();
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
                JsonDelIntegral(m_data.lineup_id, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.lineup_lv, str, length);
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
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_fighters_handler;
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

            if (!m_fighters_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        FightGroupData& m_data;
        DataArrayHandler<FighterData, unsigned char, FighterData::Handler> m_fighters_handler;
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

		json_data.AddIntegral(u8"type", type);

		json_data.AddIntegral(u8"lineup_id", lineup_id);

		json_data.AddIntegral(u8"lineup_lv", lineup_lv);

		json_data.Writer().Key(u8"fighters", sizeof(u8"fighters") - 1);
		json_data.AddArray(fighters);

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
        
        FightGroupData::Handler h(*this, nullptr);
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

struct FightOperateData{
	unsigned long long        actor; //发起者guid 玩家自己:填0 宠物:填宠物guid
	unsigned char             operate; //指令
	unsigned long long        target; //目标guid
	unsigned long long        param; //参数(使用技能时的技能ID，使用物品时物品的GUID，召唤宠物时宠物的GUID)
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(operate);

		net_data.AddIntegral(target);

		net_data.AddIntegral(param);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(operate))
			return false;

		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(param))
			return false;

		return true;
	}
	void Reset(void)
	{
		actor = 0;

		operate = 0;

		target = 0;

		param = 0;

	}
	FightOperateData& operator= (const FightOperateData& src)
	{
		actor = src.actor;

		operate = src.operate;

		target = src.target;

		param = src.param;

		return *this;
	}
	bool operator==(const FightOperateData& src) const
	{
		if (actor != src.actor)
		{
			return false;
		}

		if (operate != src.operate)
		{
			return false;
		}

		if (target != src.target)
		{
			return false;
		}

		if (param != src.param)
		{
			return false;
		}

		return true;
	}
	bool operator != (const FightOperateData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(FightOperateData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.actor, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.operate, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.param, str, length);
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
        FightOperateData& m_data;
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

		json_data.AddIntegral(u8"actor", actor);

		json_data.AddIntegral(u8"operate", operate);

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"param", param);

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
        
        FightOperateData::Handler h(*this, nullptr);
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

struct MemberData{
	unsigned long long        guid; //玩家GUID
	char                      name[32]; //玩家名字
	unsigned char             temp_leave; //暂离标记
	DataArray<AttrData, unsigned short> attrs; //属性数据
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(temp_leave);

		net_data.AddArray(attrs);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(temp_leave))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		name[0] = 0;

		temp_leave = 0;

		attrs.clear();

		customs.Reset();

	}
	MemberData& operator= (const MemberData& src)
	{
		guid = src.guid;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		temp_leave = src.temp_leave;

		attrs = src.attrs;

		customs = src.customs;

		return *this;
	}
	bool operator==(const MemberData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (temp_leave != src.temp_leave)
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const MemberData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(MemberData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_attrs_handler(m_data.attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.temp_leave, str, length);
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
            switch (m_state)
            {
            case 4:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        MemberData& m_data;
        CustomData::Handler m_customs_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
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

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddIntegral(u8"temp_leave", temp_leave);

		json_data.Writer().Key(u8"attrs", sizeof(u8"attrs") - 1);
		json_data.AddArray(attrs);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

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
        
        MemberData::Handler h(*this, nullptr);
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

struct KeyValueU32{
	unsigned int              key; //键
	unsigned int              value; //值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(key);

		net_data.AddIntegral(value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(key))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		return true;
	}
	void Reset(void)
	{
		key = 0;

		value = 0;

	}
	KeyValueU32& operator= (const KeyValueU32& src)
	{
		key = src.key;

		value = src.value;

		return *this;
	}
	bool operator==(const KeyValueU32& src) const
	{
		if (key != src.key)
		{
			return false;
		}

		if (value != src.value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const KeyValueU32& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(KeyValueU32& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.key, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.value, str, length);
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
        KeyValueU32& m_data;
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

		json_data.AddIntegral(u8"key", key);

		json_data.AddIntegral(u8"value", value);

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
        
        KeyValueU32::Handler h(*this, nullptr);
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

struct KeyValueU64{
	unsigned int              key; //键
	unsigned long long        value; //值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(key);

		net_data.AddIntegral(value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(key))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		return true;
	}
	void Reset(void)
	{
		key = 0;

		value = 0;

	}
	KeyValueU64& operator= (const KeyValueU64& src)
	{
		key = src.key;

		value = src.value;

		return *this;
	}
	bool operator==(const KeyValueU64& src) const
	{
		if (key != src.key)
		{
			return false;
		}

		if (value != src.value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const KeyValueU64& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(KeyValueU64& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.key, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.value, str, length);
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
        KeyValueU64& m_data;
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

		json_data.AddIntegral(u8"key", key);

		json_data.AddIntegral(u8"value", value);

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
        
        KeyValueU64::Handler h(*this, nullptr);
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

struct KeyValueS{
	unsigned int              key; //键
	char                      value[256]; //值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(key);

		net_data.AddString(value, sizeof(value));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(key))
			return false;

		if (!net_data.DelString(value, sizeof(value)))
			return false;

		return true;
	}
	void Reset(void)
	{
		key = 0;

		value[0] = 0;

	}
	KeyValueS& operator= (const KeyValueS& src)
	{
		key = src.key;

		{
			size_t str_len = strnlen(src.value, sizeof(value) - 1);
			memcpy(value, src.value, str_len + 1);
			value[sizeof(value) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const KeyValueS& src) const
	{
		if (key != src.key)
		{
			return false;
		}

		if (strncmp(value, src.value, sizeof(value)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const KeyValueS& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(KeyValueS& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.key, str, length);
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
                JsonDelString(m_data.value, str, length);
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
        KeyValueS& m_data;
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

		json_data.AddIntegral(u8"key", key);

		json_data.AddString(u8"value", value, sizeof(value));

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
        
        KeyValueS::Handler h(*this, nullptr);
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

struct OptionData{
	unsigned short            id; //选项id
	unsigned int              mark; //选项标识
	char                      text[256]; //选项文字
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(id);

		net_data.AddIntegral(mark);

		net_data.AddString(text, sizeof(text));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(mark))
			return false;

		if (!net_data.DelString(text, sizeof(text)))
			return false;

		return true;
	}
	void Reset(void)
	{
		id = 0;

		mark = 0;

		text[0] = 0;

	}
	OptionData& operator= (const OptionData& src)
	{
		id = src.id;

		mark = src.mark;

		{
			size_t str_len = strnlen(src.text, sizeof(text) - 1);
			memcpy(text, src.text, str_len + 1);
			text[sizeof(text) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const OptionData& src) const
	{
		if (id != src.id)
		{
			return false;
		}

		if (mark != src.mark)
		{
			return false;
		}

		if (strncmp(text, src.text, sizeof(text)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const OptionData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(OptionData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.mark, str, length);
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
                JsonDelString(m_data.text, str, length);
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
        OptionData& m_data;
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

		json_data.AddIntegral(u8"id", id);

		json_data.AddIntegral(u8"mark", mark);

		json_data.AddString(u8"text", text, sizeof(text));

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
        
        OptionData::Handler h(*this, nullptr);
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

struct ShopItemData{
	unsigned short            id; //根据商店类型决定是道具或者宠物id
	unsigned short            price_type; //价格类型 数值参考属性表对应字段
	unsigned int              price; //价格
	unsigned char             bind; //0为不绑定 1为绑定
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(id);

		net_data.AddIntegral(price_type);

		net_data.AddIntegral(price);

		net_data.AddIntegral(bind);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(price_type))
			return false;

		if (!net_data.DelIntegral(price))
			return false;

		if (!net_data.DelIntegral(bind))
			return false;

		return true;
	}
	void Reset(void)
	{
		id = 0;

		price_type = 0;

		price = 0;

		bind = 0;

	}
	ShopItemData& operator= (const ShopItemData& src)
	{
		id = src.id;

		price_type = src.price_type;

		price = src.price;

		bind = src.bind;

		return *this;
	}
	bool operator==(const ShopItemData& src) const
	{
		if (id != src.id)
		{
			return false;
		}

		if (price_type != src.price_type)
		{
			return false;
		}

		if (price != src.price)
		{
			return false;
		}

		if (bind != src.bind)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ShopItemData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ShopItemData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.price_type, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.price, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.bind, str, length);
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
        ShopItemData& m_data;
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

		json_data.AddIntegral(u8"id", id);

		json_data.AddIntegral(u8"price_type", price_type);

		json_data.AddIntegral(u8"price", price);

		json_data.AddIntegral(u8"bind", bind);

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
        
        ShopItemData::Handler h(*this, nullptr);
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

struct SellItemData{
	unsigned long long        guid; //出售物品guid
	unsigned short            amount; //出售物品数量
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(amount);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(amount))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		amount = 0;

	}
	SellItemData& operator= (const SellItemData& src)
	{
		guid = src.guid;

		amount = src.amount;

		return *this;
	}
	bool operator==(const SellItemData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (amount != src.amount)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SellItemData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SellItemData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.amount, str, length);
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
        SellItemData& m_data;
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

		json_data.AddIntegral(u8"amount", amount);

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
        
        SellItemData::Handler h(*this, nullptr);
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

struct Col1Data{
	char                      col1[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

	}
	Col1Data& operator= (const Col1Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col1Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col1Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col1Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
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
        Col1Data& m_data;
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

		json_data.AddString(u8"col1", col1, sizeof(col1));

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
        
        Col1Data::Handler h(*this, nullptr);
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

struct Col2Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

	}
	Col2Data& operator= (const Col2Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col2Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col2Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col2Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
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
        Col2Data& m_data;
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

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

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
        
        Col2Data::Handler h(*this, nullptr);
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

struct Col3Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	char                      col3[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		net_data.AddString(col3, sizeof(col3));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		if (!net_data.DelString(col3, sizeof(col3)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

		col3[0] = 0;

	}
	Col3Data& operator= (const Col3Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col3, sizeof(col3) - 1);
			memcpy(col3, src.col3, str_len + 1);
			col3[sizeof(col3) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col3Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		if (strncmp(col3, src.col3, sizeof(col3)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col3Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col3Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col3, str, length);
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
        Col3Data& m_data;
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

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

		json_data.AddString(u8"col3", col3, sizeof(col3));

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
        
        Col3Data::Handler h(*this, nullptr);
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

struct Col4Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	char                      col3[256]; //列数据
	char                      col4[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		net_data.AddString(col3, sizeof(col3));

		net_data.AddString(col4, sizeof(col4));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		if (!net_data.DelString(col3, sizeof(col3)))
			return false;

		if (!net_data.DelString(col4, sizeof(col4)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

		col3[0] = 0;

		col4[0] = 0;

	}
	Col4Data& operator= (const Col4Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col3, sizeof(col3) - 1);
			memcpy(col3, src.col3, str_len + 1);
			col3[sizeof(col3) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col4, sizeof(col4) - 1);
			memcpy(col4, src.col4, str_len + 1);
			col4[sizeof(col4) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col4Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		if (strncmp(col3, src.col3, sizeof(col3)-1))
		{
			return false;
		}

		if (strncmp(col4, src.col4, sizeof(col4)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col4Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col4Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col3, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col4, str, length);
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
        Col4Data& m_data;
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

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

		json_data.AddString(u8"col3", col3, sizeof(col3));

		json_data.AddString(u8"col4", col4, sizeof(col4));

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
        
        Col4Data::Handler h(*this, nullptr);
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

struct Col5Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	char                      col3[256]; //列数据
	char                      col4[256]; //列数据
	char                      col5[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		net_data.AddString(col3, sizeof(col3));

		net_data.AddString(col4, sizeof(col4));

		net_data.AddString(col5, sizeof(col5));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		if (!net_data.DelString(col3, sizeof(col3)))
			return false;

		if (!net_data.DelString(col4, sizeof(col4)))
			return false;

		if (!net_data.DelString(col5, sizeof(col5)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

		col3[0] = 0;

		col4[0] = 0;

		col5[0] = 0;

	}
	Col5Data& operator= (const Col5Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col3, sizeof(col3) - 1);
			memcpy(col3, src.col3, str_len + 1);
			col3[sizeof(col3) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col4, sizeof(col4) - 1);
			memcpy(col4, src.col4, str_len + 1);
			col4[sizeof(col4) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col5, sizeof(col5) - 1);
			memcpy(col5, src.col5, str_len + 1);
			col5[sizeof(col5) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col5Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		if (strncmp(col3, src.col3, sizeof(col3)-1))
		{
			return false;
		}

		if (strncmp(col4, src.col4, sizeof(col4)-1))
		{
			return false;
		}

		if (strncmp(col5, src.col5, sizeof(col5)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col5Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col5Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col3, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col4, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col5, str, length);
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
        Col5Data& m_data;
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

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

		json_data.AddString(u8"col3", col3, sizeof(col3));

		json_data.AddString(u8"col4", col4, sizeof(col4));

		json_data.AddString(u8"col5", col5, sizeof(col5));

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
        
        Col5Data::Handler h(*this, nullptr);
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

struct Col6Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	char                      col3[256]; //列数据
	char                      col4[256]; //列数据
	char                      col5[256]; //列数据
	char                      col6[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		net_data.AddString(col3, sizeof(col3));

		net_data.AddString(col4, sizeof(col4));

		net_data.AddString(col5, sizeof(col5));

		net_data.AddString(col6, sizeof(col6));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		if (!net_data.DelString(col3, sizeof(col3)))
			return false;

		if (!net_data.DelString(col4, sizeof(col4)))
			return false;

		if (!net_data.DelString(col5, sizeof(col5)))
			return false;

		if (!net_data.DelString(col6, sizeof(col6)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

		col3[0] = 0;

		col4[0] = 0;

		col5[0] = 0;

		col6[0] = 0;

	}
	Col6Data& operator= (const Col6Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col3, sizeof(col3) - 1);
			memcpy(col3, src.col3, str_len + 1);
			col3[sizeof(col3) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col4, sizeof(col4) - 1);
			memcpy(col4, src.col4, str_len + 1);
			col4[sizeof(col4) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col5, sizeof(col5) - 1);
			memcpy(col5, src.col5, str_len + 1);
			col5[sizeof(col5) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col6, sizeof(col6) - 1);
			memcpy(col6, src.col6, str_len + 1);
			col6[sizeof(col6) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col6Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		if (strncmp(col3, src.col3, sizeof(col3)-1))
		{
			return false;
		}

		if (strncmp(col4, src.col4, sizeof(col4)-1))
		{
			return false;
		}

		if (strncmp(col5, src.col5, sizeof(col5)-1))
		{
			return false;
		}

		if (strncmp(col6, src.col6, sizeof(col6)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col6Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col6Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col3, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col4, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col5, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col6, str, length);
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
        Col6Data& m_data;
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

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

		json_data.AddString(u8"col3", col3, sizeof(col3));

		json_data.AddString(u8"col4", col4, sizeof(col4));

		json_data.AddString(u8"col5", col5, sizeof(col5));

		json_data.AddString(u8"col6", col6, sizeof(col6));

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
        
        Col6Data::Handler h(*this, nullptr);
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

struct Col7Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	char                      col3[256]; //列数据
	char                      col4[256]; //列数据
	char                      col5[256]; //列数据
	char                      col6[256]; //列数据
	char                      col7[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		net_data.AddString(col3, sizeof(col3));

		net_data.AddString(col4, sizeof(col4));

		net_data.AddString(col5, sizeof(col5));

		net_data.AddString(col6, sizeof(col6));

		net_data.AddString(col7, sizeof(col7));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		if (!net_data.DelString(col3, sizeof(col3)))
			return false;

		if (!net_data.DelString(col4, sizeof(col4)))
			return false;

		if (!net_data.DelString(col5, sizeof(col5)))
			return false;

		if (!net_data.DelString(col6, sizeof(col6)))
			return false;

		if (!net_data.DelString(col7, sizeof(col7)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

		col3[0] = 0;

		col4[0] = 0;

		col5[0] = 0;

		col6[0] = 0;

		col7[0] = 0;

	}
	Col7Data& operator= (const Col7Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col3, sizeof(col3) - 1);
			memcpy(col3, src.col3, str_len + 1);
			col3[sizeof(col3) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col4, sizeof(col4) - 1);
			memcpy(col4, src.col4, str_len + 1);
			col4[sizeof(col4) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col5, sizeof(col5) - 1);
			memcpy(col5, src.col5, str_len + 1);
			col5[sizeof(col5) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col6, sizeof(col6) - 1);
			memcpy(col6, src.col6, str_len + 1);
			col6[sizeof(col6) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col7, sizeof(col7) - 1);
			memcpy(col7, src.col7, str_len + 1);
			col7[sizeof(col7) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col7Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		if (strncmp(col3, src.col3, sizeof(col3)-1))
		{
			return false;
		}

		if (strncmp(col4, src.col4, sizeof(col4)-1))
		{
			return false;
		}

		if (strncmp(col5, src.col5, sizeof(col5)-1))
		{
			return false;
		}

		if (strncmp(col6, src.col6, sizeof(col6)-1))
		{
			return false;
		}

		if (strncmp(col7, src.col7, sizeof(col7)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col7Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col7Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col3, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col4, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col5, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col6, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col7, str, length);
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
        Col7Data& m_data;
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

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

		json_data.AddString(u8"col3", col3, sizeof(col3));

		json_data.AddString(u8"col4", col4, sizeof(col4));

		json_data.AddString(u8"col5", col5, sizeof(col5));

		json_data.AddString(u8"col6", col6, sizeof(col6));

		json_data.AddString(u8"col7", col7, sizeof(col7));

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
        
        Col7Data::Handler h(*this, nullptr);
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

struct Col8Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	char                      col3[256]; //列数据
	char                      col4[256]; //列数据
	char                      col5[256]; //列数据
	char                      col6[256]; //列数据
	char                      col7[256]; //列数据
	char                      col8[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		net_data.AddString(col3, sizeof(col3));

		net_data.AddString(col4, sizeof(col4));

		net_data.AddString(col5, sizeof(col5));

		net_data.AddString(col6, sizeof(col6));

		net_data.AddString(col7, sizeof(col7));

		net_data.AddString(col8, sizeof(col8));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		if (!net_data.DelString(col3, sizeof(col3)))
			return false;

		if (!net_data.DelString(col4, sizeof(col4)))
			return false;

		if (!net_data.DelString(col5, sizeof(col5)))
			return false;

		if (!net_data.DelString(col6, sizeof(col6)))
			return false;

		if (!net_data.DelString(col7, sizeof(col7)))
			return false;

		if (!net_data.DelString(col8, sizeof(col8)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

		col3[0] = 0;

		col4[0] = 0;

		col5[0] = 0;

		col6[0] = 0;

		col7[0] = 0;

		col8[0] = 0;

	}
	Col8Data& operator= (const Col8Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col3, sizeof(col3) - 1);
			memcpy(col3, src.col3, str_len + 1);
			col3[sizeof(col3) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col4, sizeof(col4) - 1);
			memcpy(col4, src.col4, str_len + 1);
			col4[sizeof(col4) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col5, sizeof(col5) - 1);
			memcpy(col5, src.col5, str_len + 1);
			col5[sizeof(col5) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col6, sizeof(col6) - 1);
			memcpy(col6, src.col6, str_len + 1);
			col6[sizeof(col6) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col7, sizeof(col7) - 1);
			memcpy(col7, src.col7, str_len + 1);
			col7[sizeof(col7) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col8, sizeof(col8) - 1);
			memcpy(col8, src.col8, str_len + 1);
			col8[sizeof(col8) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col8Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		if (strncmp(col3, src.col3, sizeof(col3)-1))
		{
			return false;
		}

		if (strncmp(col4, src.col4, sizeof(col4)-1))
		{
			return false;
		}

		if (strncmp(col5, src.col5, sizeof(col5)-1))
		{
			return false;
		}

		if (strncmp(col6, src.col6, sizeof(col6)-1))
		{
			return false;
		}

		if (strncmp(col7, src.col7, sizeof(col7)-1))
		{
			return false;
		}

		if (strncmp(col8, src.col8, sizeof(col8)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col8Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col8Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col3, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col4, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col5, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col6, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col7, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col8, str, length);
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
        Col8Data& m_data;
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

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

		json_data.AddString(u8"col3", col3, sizeof(col3));

		json_data.AddString(u8"col4", col4, sizeof(col4));

		json_data.AddString(u8"col5", col5, sizeof(col5));

		json_data.AddString(u8"col6", col6, sizeof(col6));

		json_data.AddString(u8"col7", col7, sizeof(col7));

		json_data.AddString(u8"col8", col8, sizeof(col8));

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
        
        Col8Data::Handler h(*this, nullptr);
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

struct Col9Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	char                      col3[256]; //列数据
	char                      col4[256]; //列数据
	char                      col5[256]; //列数据
	char                      col6[256]; //列数据
	char                      col7[256]; //列数据
	char                      col8[256]; //列数据
	char                      col9[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		net_data.AddString(col3, sizeof(col3));

		net_data.AddString(col4, sizeof(col4));

		net_data.AddString(col5, sizeof(col5));

		net_data.AddString(col6, sizeof(col6));

		net_data.AddString(col7, sizeof(col7));

		net_data.AddString(col8, sizeof(col8));

		net_data.AddString(col9, sizeof(col9));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		if (!net_data.DelString(col3, sizeof(col3)))
			return false;

		if (!net_data.DelString(col4, sizeof(col4)))
			return false;

		if (!net_data.DelString(col5, sizeof(col5)))
			return false;

		if (!net_data.DelString(col6, sizeof(col6)))
			return false;

		if (!net_data.DelString(col7, sizeof(col7)))
			return false;

		if (!net_data.DelString(col8, sizeof(col8)))
			return false;

		if (!net_data.DelString(col9, sizeof(col9)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

		col3[0] = 0;

		col4[0] = 0;

		col5[0] = 0;

		col6[0] = 0;

		col7[0] = 0;

		col8[0] = 0;

		col9[0] = 0;

	}
	Col9Data& operator= (const Col9Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col3, sizeof(col3) - 1);
			memcpy(col3, src.col3, str_len + 1);
			col3[sizeof(col3) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col4, sizeof(col4) - 1);
			memcpy(col4, src.col4, str_len + 1);
			col4[sizeof(col4) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col5, sizeof(col5) - 1);
			memcpy(col5, src.col5, str_len + 1);
			col5[sizeof(col5) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col6, sizeof(col6) - 1);
			memcpy(col6, src.col6, str_len + 1);
			col6[sizeof(col6) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col7, sizeof(col7) - 1);
			memcpy(col7, src.col7, str_len + 1);
			col7[sizeof(col7) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col8, sizeof(col8) - 1);
			memcpy(col8, src.col8, str_len + 1);
			col8[sizeof(col8) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col9, sizeof(col9) - 1);
			memcpy(col9, src.col9, str_len + 1);
			col9[sizeof(col9) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col9Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		if (strncmp(col3, src.col3, sizeof(col3)-1))
		{
			return false;
		}

		if (strncmp(col4, src.col4, sizeof(col4)-1))
		{
			return false;
		}

		if (strncmp(col5, src.col5, sizeof(col5)-1))
		{
			return false;
		}

		if (strncmp(col6, src.col6, sizeof(col6)-1))
		{
			return false;
		}

		if (strncmp(col7, src.col7, sizeof(col7)-1))
		{
			return false;
		}

		if (strncmp(col8, src.col8, sizeof(col8)-1))
		{
			return false;
		}

		if (strncmp(col9, src.col9, sizeof(col9)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col9Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col9Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col3, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col4, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col5, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col6, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col7, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col8, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col9, str, length);
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
        Col9Data& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<9> m_assigned_bitset;

        static const std::bitset<9> template_assigned_bitset;
        static const char* m_key_list[9];
        static const char* m_alias_list[9];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

		json_data.AddString(u8"col3", col3, sizeof(col3));

		json_data.AddString(u8"col4", col4, sizeof(col4));

		json_data.AddString(u8"col5", col5, sizeof(col5));

		json_data.AddString(u8"col6", col6, sizeof(col6));

		json_data.AddString(u8"col7", col7, sizeof(col7));

		json_data.AddString(u8"col8", col8, sizeof(col8));

		json_data.AddString(u8"col9", col9, sizeof(col9));

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
        
        Col9Data::Handler h(*this, nullptr);
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

struct Col10Data{
	char                      col1[256]; //列数据
	char                      col2[256]; //列数据
	char                      col3[256]; //列数据
	char                      col4[256]; //列数据
	char                      col5[256]; //列数据
	char                      col6[256]; //列数据
	char                      col7[256]; //列数据
	char                      col8[256]; //列数据
	char                      col9[256]; //列数据
	char                      col10[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(col1, sizeof(col1));

		net_data.AddString(col2, sizeof(col2));

		net_data.AddString(col3, sizeof(col3));

		net_data.AddString(col4, sizeof(col4));

		net_data.AddString(col5, sizeof(col5));

		net_data.AddString(col6, sizeof(col6));

		net_data.AddString(col7, sizeof(col7));

		net_data.AddString(col8, sizeof(col8));

		net_data.AddString(col9, sizeof(col9));

		net_data.AddString(col10, sizeof(col10));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(col1, sizeof(col1)))
			return false;

		if (!net_data.DelString(col2, sizeof(col2)))
			return false;

		if (!net_data.DelString(col3, sizeof(col3)))
			return false;

		if (!net_data.DelString(col4, sizeof(col4)))
			return false;

		if (!net_data.DelString(col5, sizeof(col5)))
			return false;

		if (!net_data.DelString(col6, sizeof(col6)))
			return false;

		if (!net_data.DelString(col7, sizeof(col7)))
			return false;

		if (!net_data.DelString(col8, sizeof(col8)))
			return false;

		if (!net_data.DelString(col9, sizeof(col9)))
			return false;

		if (!net_data.DelString(col10, sizeof(col10)))
			return false;

		return true;
	}
	void Reset(void)
	{
		col1[0] = 0;

		col2[0] = 0;

		col3[0] = 0;

		col4[0] = 0;

		col5[0] = 0;

		col6[0] = 0;

		col7[0] = 0;

		col8[0] = 0;

		col9[0] = 0;

		col10[0] = 0;

	}
	Col10Data& operator= (const Col10Data& src)
	{
		{
			size_t str_len = strnlen(src.col1, sizeof(col1) - 1);
			memcpy(col1, src.col1, str_len + 1);
			col1[sizeof(col1) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col2, sizeof(col2) - 1);
			memcpy(col2, src.col2, str_len + 1);
			col2[sizeof(col2) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col3, sizeof(col3) - 1);
			memcpy(col3, src.col3, str_len + 1);
			col3[sizeof(col3) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col4, sizeof(col4) - 1);
			memcpy(col4, src.col4, str_len + 1);
			col4[sizeof(col4) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col5, sizeof(col5) - 1);
			memcpy(col5, src.col5, str_len + 1);
			col5[sizeof(col5) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col6, sizeof(col6) - 1);
			memcpy(col6, src.col6, str_len + 1);
			col6[sizeof(col6) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col7, sizeof(col7) - 1);
			memcpy(col7, src.col7, str_len + 1);
			col7[sizeof(col7) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col8, sizeof(col8) - 1);
			memcpy(col8, src.col8, str_len + 1);
			col8[sizeof(col8) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col9, sizeof(col9) - 1);
			memcpy(col9, src.col9, str_len + 1);
			col9[sizeof(col9) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.col10, sizeof(col10) - 1);
			memcpy(col10, src.col10, str_len + 1);
			col10[sizeof(col10) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const Col10Data& src) const
	{
		if (strncmp(col1, src.col1, sizeof(col1)-1))
		{
			return false;
		}

		if (strncmp(col2, src.col2, sizeof(col2)-1))
		{
			return false;
		}

		if (strncmp(col3, src.col3, sizeof(col3)-1))
		{
			return false;
		}

		if (strncmp(col4, src.col4, sizeof(col4)-1))
		{
			return false;
		}

		if (strncmp(col5, src.col5, sizeof(col5)-1))
		{
			return false;
		}

		if (strncmp(col6, src.col6, sizeof(col6)-1))
		{
			return false;
		}

		if (strncmp(col7, src.col7, sizeof(col7)-1))
		{
			return false;
		}

		if (strncmp(col8, src.col8, sizeof(col8)-1))
		{
			return false;
		}

		if (strncmp(col9, src.col9, sizeof(col9)-1))
		{
			return false;
		}

		if (strncmp(col10, src.col10, sizeof(col10)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const Col10Data& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Col10Data& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.col1, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col2, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col3, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col4, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col5, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col6, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col7, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col8, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col9, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.col10, str, length);
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
        Col10Data& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<10> m_assigned_bitset;

        static const std::bitset<10> template_assigned_bitset;
        static const char* m_key_list[10];
        static const char* m_alias_list[10];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddString(u8"col1", col1, sizeof(col1));

		json_data.AddString(u8"col2", col2, sizeof(col2));

		json_data.AddString(u8"col3", col3, sizeof(col3));

		json_data.AddString(u8"col4", col4, sizeof(col4));

		json_data.AddString(u8"col5", col5, sizeof(col5));

		json_data.AddString(u8"col6", col6, sizeof(col6));

		json_data.AddString(u8"col7", col7, sizeof(col7));

		json_data.AddString(u8"col8", col8, sizeof(col8));

		json_data.AddString(u8"col9", col9, sizeof(col9));

		json_data.AddString(u8"col10", col10, sizeof(col10));

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
        
        Col10Data::Handler h(*this, nullptr);
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

struct TalkNodeData{
	char                      node_type[256]; //列数据
	char                      node_id[256]; //列数据
	char                      display_text[256]; //列数据
	char                      connect_text[256]; //列数据
	char                      npc_id[256]; //列数据
	char                      npc_emotion[256]; //列数据
	char                      npc_talk[512]; //列数据
	char                      npc_template[256]; //列数据
	char                      func_type[256]; //列数据
	char                      link_node[256]; //列数据
	char                      belong_to[256]; //列数据
	char                      player_talk[256]; //列数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(node_type, sizeof(node_type));

		net_data.AddString(node_id, sizeof(node_id));

		net_data.AddString(display_text, sizeof(display_text));

		net_data.AddString(connect_text, sizeof(connect_text));

		net_data.AddString(npc_id, sizeof(npc_id));

		net_data.AddString(npc_emotion, sizeof(npc_emotion));

		net_data.AddString(npc_talk, sizeof(npc_talk));

		net_data.AddString(npc_template, sizeof(npc_template));

		net_data.AddString(func_type, sizeof(func_type));

		net_data.AddString(link_node, sizeof(link_node));

		net_data.AddString(belong_to, sizeof(belong_to));

		net_data.AddString(player_talk, sizeof(player_talk));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(node_type, sizeof(node_type)))
			return false;

		if (!net_data.DelString(node_id, sizeof(node_id)))
			return false;

		if (!net_data.DelString(display_text, sizeof(display_text)))
			return false;

		if (!net_data.DelString(connect_text, sizeof(connect_text)))
			return false;

		if (!net_data.DelString(npc_id, sizeof(npc_id)))
			return false;

		if (!net_data.DelString(npc_emotion, sizeof(npc_emotion)))
			return false;

		if (!net_data.DelString(npc_talk, sizeof(npc_talk)))
			return false;

		if (!net_data.DelString(npc_template, sizeof(npc_template)))
			return false;

		if (!net_data.DelString(func_type, sizeof(func_type)))
			return false;

		if (!net_data.DelString(link_node, sizeof(link_node)))
			return false;

		if (!net_data.DelString(belong_to, sizeof(belong_to)))
			return false;

		if (!net_data.DelString(player_talk, sizeof(player_talk)))
			return false;

		return true;
	}
	void Reset(void)
	{
		node_type[0] = 0;

		node_id[0] = 0;

		display_text[0] = 0;

		connect_text[0] = 0;

		npc_id[0] = 0;

		npc_emotion[0] = 0;

		npc_talk[0] = 0;

		npc_template[0] = 0;

		func_type[0] = 0;

		link_node[0] = 0;

		belong_to[0] = 0;

		player_talk[0] = 0;

	}
	TalkNodeData& operator= (const TalkNodeData& src)
	{
		{
			size_t str_len = strnlen(src.node_type, sizeof(node_type) - 1);
			memcpy(node_type, src.node_type, str_len + 1);
			node_type[sizeof(node_type) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.node_id, sizeof(node_id) - 1);
			memcpy(node_id, src.node_id, str_len + 1);
			node_id[sizeof(node_id) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.display_text, sizeof(display_text) - 1);
			memcpy(display_text, src.display_text, str_len + 1);
			display_text[sizeof(display_text) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.connect_text, sizeof(connect_text) - 1);
			memcpy(connect_text, src.connect_text, str_len + 1);
			connect_text[sizeof(connect_text) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.npc_id, sizeof(npc_id) - 1);
			memcpy(npc_id, src.npc_id, str_len + 1);
			npc_id[sizeof(npc_id) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.npc_emotion, sizeof(npc_emotion) - 1);
			memcpy(npc_emotion, src.npc_emotion, str_len + 1);
			npc_emotion[sizeof(npc_emotion) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.npc_talk, sizeof(npc_talk) - 1);
			memcpy(npc_talk, src.npc_talk, str_len + 1);
			npc_talk[sizeof(npc_talk) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.npc_template, sizeof(npc_template) - 1);
			memcpy(npc_template, src.npc_template, str_len + 1);
			npc_template[sizeof(npc_template) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.func_type, sizeof(func_type) - 1);
			memcpy(func_type, src.func_type, str_len + 1);
			func_type[sizeof(func_type) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.link_node, sizeof(link_node) - 1);
			memcpy(link_node, src.link_node, str_len + 1);
			link_node[sizeof(link_node) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.belong_to, sizeof(belong_to) - 1);
			memcpy(belong_to, src.belong_to, str_len + 1);
			belong_to[sizeof(belong_to) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.player_talk, sizeof(player_talk) - 1);
			memcpy(player_talk, src.player_talk, str_len + 1);
			player_talk[sizeof(player_talk) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const TalkNodeData& src) const
	{
		if (strncmp(node_type, src.node_type, sizeof(node_type)-1))
		{
			return false;
		}

		if (strncmp(node_id, src.node_id, sizeof(node_id)-1))
		{
			return false;
		}

		if (strncmp(display_text, src.display_text, sizeof(display_text)-1))
		{
			return false;
		}

		if (strncmp(connect_text, src.connect_text, sizeof(connect_text)-1))
		{
			return false;
		}

		if (strncmp(npc_id, src.npc_id, sizeof(npc_id)-1))
		{
			return false;
		}

		if (strncmp(npc_emotion, src.npc_emotion, sizeof(npc_emotion)-1))
		{
			return false;
		}

		if (strncmp(npc_talk, src.npc_talk, sizeof(npc_talk)-1))
		{
			return false;
		}

		if (strncmp(npc_template, src.npc_template, sizeof(npc_template)-1))
		{
			return false;
		}

		if (strncmp(func_type, src.func_type, sizeof(func_type)-1))
		{
			return false;
		}

		if (strncmp(link_node, src.link_node, sizeof(link_node)-1))
		{
			return false;
		}

		if (strncmp(belong_to, src.belong_to, sizeof(belong_to)-1))
		{
			return false;
		}

		if (strncmp(player_talk, src.player_talk, sizeof(player_talk)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const TalkNodeData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TalkNodeData& data, JsonHandler* parent = nullptr)
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
                JsonDelString(m_data.node_type, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.node_id, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.display_text, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.connect_text, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.npc_id, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.npc_emotion, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.npc_talk, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.npc_template, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.func_type, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.link_node, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.belong_to, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.player_talk, str, length);
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
        TalkNodeData& m_data;
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

		json_data.AddString(u8"node_type", node_type, sizeof(node_type));

		json_data.AddString(u8"node_id", node_id, sizeof(node_id));

		json_data.AddString(u8"display_text", display_text, sizeof(display_text));

		json_data.AddString(u8"connect_text", connect_text, sizeof(connect_text));

		json_data.AddString(u8"npc_id", npc_id, sizeof(npc_id));

		json_data.AddString(u8"npc_emotion", npc_emotion, sizeof(npc_emotion));

		json_data.AddString(u8"npc_talk", npc_talk, sizeof(npc_talk));

		json_data.AddString(u8"npc_template", npc_template, sizeof(npc_template));

		json_data.AddString(u8"func_type", func_type, sizeof(func_type));

		json_data.AddString(u8"link_node", link_node, sizeof(link_node));

		json_data.AddString(u8"belong_to", belong_to, sizeof(belong_to));

		json_data.AddString(u8"player_talk", player_talk, sizeof(player_talk));

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
        
        TalkNodeData::Handler h(*this, nullptr);
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

struct QuestClientData{
	unsigned int              quest_id; //任务ID
	unsigned int              quest_type; //任务类型
	char                      quest_type_name[128]; //任务类型名称
	char                      quest_name[32]; //任务名称
	unsigned int              quest_color; //任务颜色
	unsigned char             quest_state; //任务状态
	unsigned char             display_location; //显示位置，0:显示在可接列表 1:显示在已接列表
	unsigned char             display_accept_tip; //是否发送接受任务提示
	unsigned char             display_accomplish_tip; //是否发送完成任务提示
	unsigned char             can_track; //任务追踪可见
	unsigned char             can_abandon; //任务是否可放弃
	unsigned int              accpet_time; //任务接受时间
	unsigned int              limit_time; //任务结束时间
	unsigned int              changed_time; //任务状态变更时间
	unsigned long long        accept_npc; //接任务目标NPC
	char                      accept_npc_info[256]; //任务目标NPC信息
	unsigned long long        commit_npc; //交任务目标NPC
	char                      commit_npc_info[256]; //交任务目标NPC信息
	unsigned short            quest_goal_type; //任务目标信息
	DataArray<unsigned char, unsigned short> quest_goal; //任务目标信息
	DataArray<unsigned char, unsigned short> quest_award; //任务奖励信息
	char                      track_desc[512]; //任务目标追踪信息
	char                      cant_accept_track_desc[512]; //不可接任务目标追踪信息
	char                      cant_accept_desc[512]; //不可接描述信息
	char                      can_accept_desc[512]; //可接描述信息
	char                      accepted_desc[512]; //已接描述信息
	unsigned char             is_display_cycle_num; //是否显示轮环数
	unsigned int              finish_cycle_num; //完成环数
	unsigned int              total_cycle_num; //总环数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(quest_id);

		net_data.AddIntegral(quest_type);

		net_data.AddString(quest_type_name, sizeof(quest_type_name));

		net_data.AddString(quest_name, sizeof(quest_name));

		net_data.AddIntegral(quest_color);

		net_data.AddIntegral(quest_state);

		net_data.AddIntegral(display_location);

		net_data.AddIntegral(display_accept_tip);

		net_data.AddIntegral(display_accomplish_tip);

		net_data.AddIntegral(can_track);

		net_data.AddIntegral(can_abandon);

		net_data.AddIntegral(accpet_time);

		net_data.AddIntegral(limit_time);

		net_data.AddIntegral(changed_time);

		net_data.AddIntegral(accept_npc);

		net_data.AddString(accept_npc_info, sizeof(accept_npc_info));

		net_data.AddIntegral(commit_npc);

		net_data.AddString(commit_npc_info, sizeof(commit_npc_info));

		net_data.AddIntegral(quest_goal_type);

		net_data.AddArray(quest_goal);

		net_data.AddArray(quest_award);

		net_data.AddString(track_desc, sizeof(track_desc));

		net_data.AddString(cant_accept_track_desc, sizeof(cant_accept_track_desc));

		net_data.AddString(cant_accept_desc, sizeof(cant_accept_desc));

		net_data.AddString(can_accept_desc, sizeof(can_accept_desc));

		net_data.AddString(accepted_desc, sizeof(accepted_desc));

		net_data.AddIntegral(is_display_cycle_num);

		net_data.AddIntegral(finish_cycle_num);

		net_data.AddIntegral(total_cycle_num);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(quest_id))
			return false;

		if (!net_data.DelIntegral(quest_type))
			return false;

		if (!net_data.DelString(quest_type_name, sizeof(quest_type_name)))
			return false;

		if (!net_data.DelString(quest_name, sizeof(quest_name)))
			return false;

		if (!net_data.DelIntegral(quest_color))
			return false;

		if (!net_data.DelIntegral(quest_state))
			return false;

		if (!net_data.DelIntegral(display_location))
			return false;

		if (!net_data.DelIntegral(display_accept_tip))
			return false;

		if (!net_data.DelIntegral(display_accomplish_tip))
			return false;

		if (!net_data.DelIntegral(can_track))
			return false;

		if (!net_data.DelIntegral(can_abandon))
			return false;

		if (!net_data.DelIntegral(accpet_time))
			return false;

		if (!net_data.DelIntegral(limit_time))
			return false;

		if (!net_data.DelIntegral(changed_time))
			return false;

		if (!net_data.DelIntegral(accept_npc))
			return false;

		if (!net_data.DelString(accept_npc_info, sizeof(accept_npc_info)))
			return false;

		if (!net_data.DelIntegral(commit_npc))
			return false;

		if (!net_data.DelString(commit_npc_info, sizeof(commit_npc_info)))
			return false;

		if (!net_data.DelIntegral(quest_goal_type))
			return false;

		if(!net_data.DelArray(quest_goal))
			return false;

		if(!net_data.DelArray(quest_award))
			return false;

		if (!net_data.DelString(track_desc, sizeof(track_desc)))
			return false;

		if (!net_data.DelString(cant_accept_track_desc, sizeof(cant_accept_track_desc)))
			return false;

		if (!net_data.DelString(cant_accept_desc, sizeof(cant_accept_desc)))
			return false;

		if (!net_data.DelString(can_accept_desc, sizeof(can_accept_desc)))
			return false;

		if (!net_data.DelString(accepted_desc, sizeof(accepted_desc)))
			return false;

		if (!net_data.DelIntegral(is_display_cycle_num))
			return false;

		if (!net_data.DelIntegral(finish_cycle_num))
			return false;

		if (!net_data.DelIntegral(total_cycle_num))
			return false;

		return true;
	}
	void Reset(void)
	{
		quest_id = 0;

		quest_type = 0;

		quest_type_name[0] = 0;

		quest_name[0] = 0;

		quest_color = 0;

		quest_state = 0;

		display_location = 0;

		display_accept_tip = 0;

		display_accomplish_tip = 0;

		can_track = 0;

		can_abandon = 0;

		accpet_time = 0;

		limit_time = 0;

		changed_time = 0;

		accept_npc = 0;

		accept_npc_info[0] = 0;

		commit_npc = 0;

		commit_npc_info[0] = 0;

		quest_goal_type = 0;

		quest_goal.clear();

		quest_award.clear();

		track_desc[0] = 0;

		cant_accept_track_desc[0] = 0;

		cant_accept_desc[0] = 0;

		can_accept_desc[0] = 0;

		accepted_desc[0] = 0;

		is_display_cycle_num = 0;

		finish_cycle_num = 0;

		total_cycle_num = 0;

	}
	QuestClientData& operator= (const QuestClientData& src)
	{
		quest_id = src.quest_id;

		quest_type = src.quest_type;

		{
			size_t str_len = strnlen(src.quest_type_name, sizeof(quest_type_name) - 1);
			memcpy(quest_type_name, src.quest_type_name, str_len + 1);
			quest_type_name[sizeof(quest_type_name) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.quest_name, sizeof(quest_name) - 1);
			memcpy(quest_name, src.quest_name, str_len + 1);
			quest_name[sizeof(quest_name) - 1] = 0;
		}

		quest_color = src.quest_color;

		quest_state = src.quest_state;

		display_location = src.display_location;

		display_accept_tip = src.display_accept_tip;

		display_accomplish_tip = src.display_accomplish_tip;

		can_track = src.can_track;

		can_abandon = src.can_abandon;

		accpet_time = src.accpet_time;

		limit_time = src.limit_time;

		changed_time = src.changed_time;

		accept_npc = src.accept_npc;

		{
			size_t str_len = strnlen(src.accept_npc_info, sizeof(accept_npc_info) - 1);
			memcpy(accept_npc_info, src.accept_npc_info, str_len + 1);
			accept_npc_info[sizeof(accept_npc_info) - 1] = 0;
		}

		commit_npc = src.commit_npc;

		{
			size_t str_len = strnlen(src.commit_npc_info, sizeof(commit_npc_info) - 1);
			memcpy(commit_npc_info, src.commit_npc_info, str_len + 1);
			commit_npc_info[sizeof(commit_npc_info) - 1] = 0;
		}

		quest_goal_type = src.quest_goal_type;

		quest_goal = src.quest_goal;

		quest_award = src.quest_award;

		{
			size_t str_len = strnlen(src.track_desc, sizeof(track_desc) - 1);
			memcpy(track_desc, src.track_desc, str_len + 1);
			track_desc[sizeof(track_desc) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.cant_accept_track_desc, sizeof(cant_accept_track_desc) - 1);
			memcpy(cant_accept_track_desc, src.cant_accept_track_desc, str_len + 1);
			cant_accept_track_desc[sizeof(cant_accept_track_desc) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.cant_accept_desc, sizeof(cant_accept_desc) - 1);
			memcpy(cant_accept_desc, src.cant_accept_desc, str_len + 1);
			cant_accept_desc[sizeof(cant_accept_desc) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.can_accept_desc, sizeof(can_accept_desc) - 1);
			memcpy(can_accept_desc, src.can_accept_desc, str_len + 1);
			can_accept_desc[sizeof(can_accept_desc) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.accepted_desc, sizeof(accepted_desc) - 1);
			memcpy(accepted_desc, src.accepted_desc, str_len + 1);
			accepted_desc[sizeof(accepted_desc) - 1] = 0;
		}

		is_display_cycle_num = src.is_display_cycle_num;

		finish_cycle_num = src.finish_cycle_num;

		total_cycle_num = src.total_cycle_num;

		return *this;
	}
	bool operator==(const QuestClientData& src) const
	{
		if (quest_id != src.quest_id)
		{
			return false;
		}

		if (quest_type != src.quest_type)
		{
			return false;
		}

		if (strncmp(quest_type_name, src.quest_type_name, sizeof(quest_type_name)-1))
		{
			return false;
		}

		if (strncmp(quest_name, src.quest_name, sizeof(quest_name)-1))
		{
			return false;
		}

		if (quest_color != src.quest_color)
		{
			return false;
		}

		if (quest_state != src.quest_state)
		{
			return false;
		}

		if (display_location != src.display_location)
		{
			return false;
		}

		if (display_accept_tip != src.display_accept_tip)
		{
			return false;
		}

		if (display_accomplish_tip != src.display_accomplish_tip)
		{
			return false;
		}

		if (can_track != src.can_track)
		{
			return false;
		}

		if (can_abandon != src.can_abandon)
		{
			return false;
		}

		if (accpet_time != src.accpet_time)
		{
			return false;
		}

		if (limit_time != src.limit_time)
		{
			return false;
		}

		if (changed_time != src.changed_time)
		{
			return false;
		}

		if (accept_npc != src.accept_npc)
		{
			return false;
		}

		if (strncmp(accept_npc_info, src.accept_npc_info, sizeof(accept_npc_info)-1))
		{
			return false;
		}

		if (commit_npc != src.commit_npc)
		{
			return false;
		}

		if (strncmp(commit_npc_info, src.commit_npc_info, sizeof(commit_npc_info)-1))
		{
			return false;
		}

		if (quest_goal_type != src.quest_goal_type)
		{
			return false;
		}

		if (quest_goal != src.quest_goal)
		{
			return false;
		}

		if (quest_award != src.quest_award)
		{
			return false;
		}

		if (strncmp(track_desc, src.track_desc, sizeof(track_desc)-1))
		{
			return false;
		}

		if (strncmp(cant_accept_track_desc, src.cant_accept_track_desc, sizeof(cant_accept_track_desc)-1))
		{
			return false;
		}

		if (strncmp(cant_accept_desc, src.cant_accept_desc, sizeof(cant_accept_desc)-1))
		{
			return false;
		}

		if (strncmp(can_accept_desc, src.can_accept_desc, sizeof(can_accept_desc)-1))
		{
			return false;
		}

		if (strncmp(accepted_desc, src.accepted_desc, sizeof(accepted_desc)-1))
		{
			return false;
		}

		if (is_display_cycle_num != src.is_display_cycle_num)
		{
			return false;
		}

		if (finish_cycle_num != src.finish_cycle_num)
		{
			return false;
		}

		if (total_cycle_num != src.total_cycle_num)
		{
			return false;
		}

		return true;
	}
	bool operator != (const QuestClientData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(QuestClientData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.quest_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.quest_type, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.quest_color, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.quest_state, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.display_location, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.display_accept_tip, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.display_accomplish_tip, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.can_track, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.can_abandon, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.accpet_time, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.limit_time, str, length);
            }
            break;
            case 13:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.changed_time, str, length);
            }
            break;
            case 14:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.accept_npc, str, length);
            }
            break;
            case 16:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.commit_npc, str, length);
            }
            break;
            case 18:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.quest_goal_type, str, length);
            }
            break;
            case 26:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.is_display_cycle_num, str, length);
            }
            break;
            case 27:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.finish_cycle_num, str, length);
            }
            break;
            case 28:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.total_cycle_num, str, length);
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
                JsonDelString(m_data.quest_type_name, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.quest_name, str, length);
            }
            break;
            case 15:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.accept_npc_info, str, length);
            }
            break;
            case 17:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.commit_npc_info, str, length);
            }
            break;
            case 19:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.quest_goal, this);
                handler.String(str, length, copy);
            }
            break;
            case 20:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.quest_award, this);
                handler.String(str, length, copy);
            }
            break;
            case 21:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.track_desc, str, length);
            }
            break;
            case 22:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.cant_accept_track_desc, str, length);
            }
            break;
            case 23:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.cant_accept_desc, str, length);
            }
            break;
            case 24:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.can_accept_desc, str, length);
            }
            break;
            case 25:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.accepted_desc, str, length);
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
        QuestClientData& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<29> m_assigned_bitset;

        static const std::bitset<29> template_assigned_bitset;
        static const char* m_key_list[29];
        static const char* m_alias_list[29];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"quest_id", quest_id);

		json_data.AddIntegral(u8"quest_type", quest_type);

		json_data.AddString(u8"quest_type_name", quest_type_name, sizeof(quest_type_name));

		json_data.AddString(u8"quest_name", quest_name, sizeof(quest_name));

		json_data.AddIntegral(u8"quest_color", quest_color);

		json_data.AddIntegral(u8"quest_state", quest_state);

		json_data.AddIntegral(u8"display_location", display_location);

		json_data.AddIntegral(u8"display_accept_tip", display_accept_tip);

		json_data.AddIntegral(u8"display_accomplish_tip", display_accomplish_tip);

		json_data.AddIntegral(u8"can_track", can_track);

		json_data.AddIntegral(u8"can_abandon", can_abandon);

		json_data.AddIntegral(u8"accpet_time", accpet_time);

		json_data.AddIntegral(u8"limit_time", limit_time);

		json_data.AddIntegral(u8"changed_time", changed_time);

		json_data.AddIntegral(u8"accept_npc", accept_npc);

		json_data.AddString(u8"accept_npc_info", accept_npc_info, sizeof(accept_npc_info));

		json_data.AddIntegral(u8"commit_npc", commit_npc);

		json_data.AddString(u8"commit_npc_info", commit_npc_info, sizeof(commit_npc_info));

		json_data.AddIntegral(u8"quest_goal_type", quest_goal_type);

		json_data.Writer().Key(u8"quest_goal", sizeof(u8"quest_goal") - 1);
		json_data.AddArray(quest_goal);

		json_data.Writer().Key(u8"quest_award", sizeof(u8"quest_award") - 1);
		json_data.AddArray(quest_award);

		json_data.AddString(u8"track_desc", track_desc, sizeof(track_desc));

		json_data.AddString(u8"cant_accept_track_desc", cant_accept_track_desc, sizeof(cant_accept_track_desc));

		json_data.AddString(u8"cant_accept_desc", cant_accept_desc, sizeof(cant_accept_desc));

		json_data.AddString(u8"can_accept_desc", can_accept_desc, sizeof(can_accept_desc));

		json_data.AddString(u8"accepted_desc", accepted_desc, sizeof(accepted_desc));

		json_data.AddIntegral(u8"is_display_cycle_num", is_display_cycle_num);

		json_data.AddIntegral(u8"finish_cycle_num", finish_cycle_num);

		json_data.AddIntegral(u8"total_cycle_num", total_cycle_num);

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
        
        QuestClientData::Handler h(*this, nullptr);
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

struct QuestTalkData{
	unsigned int              npc_id; //NPC头像ID
	unsigned int              node_id; //对话节点ID
	DataArray<unsigned char, unsigned short> talk_text; //对话内容
	DataArray<OptionData, unsigned char> options; //对话选项
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(npc_id);

		net_data.AddIntegral(node_id);

		net_data.AddArray(talk_text);

		net_data.AddArray(options);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(npc_id))
			return false;

		if (!net_data.DelIntegral(node_id))
			return false;

		if(!net_data.DelArray(talk_text))
			return false;

		if(!net_data.DelArray(options))
			return false;

		return true;
	}
	void Reset(void)
	{
		npc_id = 0;

		node_id = 0;

		talk_text.clear();

		options.clear();

	}
	QuestTalkData& operator= (const QuestTalkData& src)
	{
		npc_id = src.npc_id;

		node_id = src.node_id;

		talk_text = src.talk_text;

		options = src.options;

		return *this;
	}
	bool operator==(const QuestTalkData& src) const
	{
		if (npc_id != src.npc_id)
		{
			return false;
		}

		if (node_id != src.node_id)
		{
			return false;
		}

		if (talk_text != src.talk_text)
		{
			return false;
		}

		if (options != src.options)
		{
			return false;
		}

		return true;
	}
	bool operator != (const QuestTalkData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(QuestTalkData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_options_handler(m_data.options, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_options_handler.ResetState();
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
                JsonDelIntegral(m_data.npc_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.node_id, str, length);
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.talk_text, this);
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
            switch (m_state)
            {
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_options_handler;
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

            if (!m_options_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        QuestTalkData& m_data;
        DataArrayHandler<OptionData, unsigned char, OptionData::Handler> m_options_handler;
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

		json_data.AddIntegral(u8"npc_id", npc_id);

		json_data.AddIntegral(u8"node_id", node_id);

		json_data.Writer().Key(u8"talk_text", sizeof(u8"talk_text") - 1);
		json_data.AddArray(talk_text);

		json_data.Writer().Key(u8"options", sizeof(u8"options") - 1);
		json_data.AddArray(options);

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
        
        QuestTalkData::Handler h(*this, nullptr);
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

struct IDCountData{
	unsigned int              id; //ID
	unsigned short            amount; //数量
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(id);

		net_data.AddIntegral(amount);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(amount))
			return false;

		return true;
	}
	void Reset(void)
	{
		id = 0;

		amount = 0;

	}
	IDCountData& operator= (const IDCountData& src)
	{
		id = src.id;

		amount = src.amount;

		return *this;
	}
	bool operator==(const IDCountData& src) const
	{
		if (id != src.id)
		{
			return false;
		}

		if (amount != src.amount)
		{
			return false;
		}

		return true;
	}
	bool operator != (const IDCountData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(IDCountData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.amount, str, length);
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
        IDCountData& m_data;
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

		json_data.AddIntegral(u8"id", id);

		json_data.AddIntegral(u8"amount", amount);

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
        
        IDCountData::Handler h(*this, nullptr);
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

struct GUIDCountData{
	unsigned long long        guid; //GUID
	unsigned short            amount; //数量
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(amount);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(amount))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		amount = 0;

	}
	GUIDCountData& operator= (const GUIDCountData& src)
	{
		guid = src.guid;

		amount = src.amount;

		return *this;
	}
	bool operator==(const GUIDCountData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (amount != src.amount)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GUIDCountData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GUIDCountData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.amount, str, length);
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
        GUIDCountData& m_data;
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

		json_data.AddIntegral(u8"amount", amount);

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
        
        GUIDCountData::Handler h(*this, nullptr);
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

struct SkillTipsData{
	unsigned char             name; //tips名字
	unsigned short            attr; //属性名
	signed long long          value; //属性值
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(name);

		net_data.AddIntegral(attr);

		net_data.AddIntegral(value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(name))
			return false;

		if (!net_data.DelIntegral(attr))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		return true;
	}
	void Reset(void)
	{
		name = 0;

		attr = 0;

		value = 0;

	}
	SkillTipsData& operator= (const SkillTipsData& src)
	{
		name = src.name;

		attr = src.attr;

		value = src.value;

		return *this;
	}
	bool operator==(const SkillTipsData& src) const
	{
		if (name != src.name)
		{
			return false;
		}

		if (attr != src.attr)
		{
			return false;
		}

		if (value != src.value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SkillTipsData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SkillTipsData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.name, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.attr, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.value, str, length);
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
        SkillTipsData& m_data;
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

		json_data.AddIntegral(u8"name", name);

		json_data.AddIntegral(u8"attr", attr);

		json_data.AddIntegral(u8"value", value);

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
        
        SkillTipsData::Handler h(*this, nullptr);
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

struct GoalVisitNpcData{
	unsigned int              npc_id; //NPC ID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(npc_id);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(npc_id))
			return false;

		return true;
	}
	void Reset(void)
	{
		npc_id = 0;

	}
	GoalVisitNpcData& operator= (const GoalVisitNpcData& src)
	{
		npc_id = src.npc_id;

		return *this;
	}
	bool operator==(const GoalVisitNpcData& src) const
	{
		if (npc_id != src.npc_id)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalVisitNpcData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalVisitNpcData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.npc_id, str, length);
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
        GoalVisitNpcData& m_data;
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

		json_data.AddIntegral(u8"npc_id", npc_id);

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
        
        GoalVisitNpcData::Handler h(*this, nullptr);
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

struct GoalKillBossData{
	unsigned int              boss_id; //BOSS ID
	unsigned short            map_id; //地图
	unsigned short            loc_x; //x坐标
	unsigned short            loc_y; //y坐标
	unsigned short            kill_count; //杀死次数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(boss_id);

		net_data.AddIntegral(map_id);

		net_data.AddIntegral(loc_x);

		net_data.AddIntegral(loc_y);

		net_data.AddIntegral(kill_count);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(boss_id))
			return false;

		if (!net_data.DelIntegral(map_id))
			return false;

		if (!net_data.DelIntegral(loc_x))
			return false;

		if (!net_data.DelIntegral(loc_y))
			return false;

		if (!net_data.DelIntegral(kill_count))
			return false;

		return true;
	}
	void Reset(void)
	{
		boss_id = 0;

		map_id = 0;

		loc_x = 0;

		loc_y = 0;

		kill_count = 0;

	}
	GoalKillBossData& operator= (const GoalKillBossData& src)
	{
		boss_id = src.boss_id;

		map_id = src.map_id;

		loc_x = src.loc_x;

		loc_y = src.loc_y;

		kill_count = src.kill_count;

		return *this;
	}
	bool operator==(const GoalKillBossData& src) const
	{
		if (boss_id != src.boss_id)
		{
			return false;
		}

		if (map_id != src.map_id)
		{
			return false;
		}

		if (loc_x != src.loc_x)
		{
			return false;
		}

		if (loc_y != src.loc_y)
		{
			return false;
		}

		if (kill_count != src.kill_count)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalKillBossData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalKillBossData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.boss_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.map_id, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.loc_x, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.loc_y, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.kill_count, str, length);
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
        GoalKillBossData& m_data;
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

		json_data.AddIntegral(u8"boss_id", boss_id);

		json_data.AddIntegral(u8"map_id", map_id);

		json_data.AddIntegral(u8"loc_x", loc_x);

		json_data.AddIntegral(u8"loc_y", loc_y);

		json_data.AddIntegral(u8"kill_count", kill_count);

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
        
        GoalKillBossData::Handler h(*this, nullptr);
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

struct GoalCommitItemData{
	unsigned int              item_id; //物品ID
	unsigned int              item_count; //物品数量
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(item_id);

		net_data.AddIntegral(item_count);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(item_id))
			return false;

		if (!net_data.DelIntegral(item_count))
			return false;

		return true;
	}
	void Reset(void)
	{
		item_id = 0;

		item_count = 0;

	}
	GoalCommitItemData& operator= (const GoalCommitItemData& src)
	{
		item_id = src.item_id;

		item_count = src.item_count;

		return *this;
	}
	bool operator==(const GoalCommitItemData& src) const
	{
		if (item_id != src.item_id)
		{
			return false;
		}

		if (item_count != src.item_count)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalCommitItemData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalCommitItemData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.item_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.item_count, str, length);
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
        GoalCommitItemData& m_data;
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

		json_data.AddIntegral(u8"item_id", item_id);

		json_data.AddIntegral(u8"item_count", item_count);

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
        
        GoalCommitItemData::Handler h(*this, nullptr);
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

struct GoalCommitPetData{
	unsigned int              pet_id; //宠物ID
	unsigned int              pet_count; //宠物数量
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(pet_id);

		net_data.AddIntegral(pet_count);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(pet_id))
			return false;

		if (!net_data.DelIntegral(pet_count))
			return false;

		return true;
	}
	void Reset(void)
	{
		pet_id = 0;

		pet_count = 0;

	}
	GoalCommitPetData& operator= (const GoalCommitPetData& src)
	{
		pet_id = src.pet_id;

		pet_count = src.pet_count;

		return *this;
	}
	bool operator==(const GoalCommitPetData& src) const
	{
		if (pet_id != src.pet_id)
		{
			return false;
		}

		if (pet_count != src.pet_count)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalCommitPetData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalCommitPetData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.pet_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.pet_count, str, length);
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
        GoalCommitPetData& m_data;
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

		json_data.AddIntegral(u8"pet_id", pet_id);

		json_data.AddIntegral(u8"pet_count", pet_count);

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
        
        GoalCommitPetData::Handler h(*this, nullptr);
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

struct GoalEscortNpcData{
	unsigned int              npc_id; //NPC ID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(npc_id);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(npc_id))
			return false;

		return true;
	}
	void Reset(void)
	{
		npc_id = 0;

	}
	GoalEscortNpcData& operator= (const GoalEscortNpcData& src)
	{
		npc_id = src.npc_id;

		return *this;
	}
	bool operator==(const GoalEscortNpcData& src) const
	{
		if (npc_id != src.npc_id)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalEscortNpcData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalEscortNpcData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.npc_id, str, length);
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
        GoalEscortNpcData& m_data;
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

		json_data.AddIntegral(u8"npc_id", npc_id);

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
        
        GoalEscortNpcData::Handler h(*this, nullptr);
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

struct GoalArriveAddrData{
	unsigned short            map_id; //地图ID
	unsigned short            loc_x; //x坐标
	unsigned short            loc_y; //y坐标
	unsigned short            range; //范围
	unsigned int              item_id; //物品ID
	unsigned int              npc_id; //NPC ID
	char                      desc[256]; //描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(map_id);

		net_data.AddIntegral(loc_x);

		net_data.AddIntegral(loc_y);

		net_data.AddIntegral(range);

		net_data.AddIntegral(item_id);

		net_data.AddIntegral(npc_id);

		net_data.AddString(desc, sizeof(desc));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(map_id))
			return false;

		if (!net_data.DelIntegral(loc_x))
			return false;

		if (!net_data.DelIntegral(loc_y))
			return false;

		if (!net_data.DelIntegral(range))
			return false;

		if (!net_data.DelIntegral(item_id))
			return false;

		if (!net_data.DelIntegral(npc_id))
			return false;

		if (!net_data.DelString(desc, sizeof(desc)))
			return false;

		return true;
	}
	void Reset(void)
	{
		map_id = 0;

		loc_x = 0;

		loc_y = 0;

		range = 0;

		item_id = 0;

		npc_id = 0;

		desc[0] = 0;

	}
	GoalArriveAddrData& operator= (const GoalArriveAddrData& src)
	{
		map_id = src.map_id;

		loc_x = src.loc_x;

		loc_y = src.loc_y;

		range = src.range;

		item_id = src.item_id;

		npc_id = src.npc_id;

		{
			size_t str_len = strnlen(src.desc, sizeof(desc) - 1);
			memcpy(desc, src.desc, str_len + 1);
			desc[sizeof(desc) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const GoalArriveAddrData& src) const
	{
		if (map_id != src.map_id)
		{
			return false;
		}

		if (loc_x != src.loc_x)
		{
			return false;
		}

		if (loc_y != src.loc_y)
		{
			return false;
		}

		if (range != src.range)
		{
			return false;
		}

		if (item_id != src.item_id)
		{
			return false;
		}

		if (npc_id != src.npc_id)
		{
			return false;
		}

		if (strncmp(desc, src.desc, sizeof(desc)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalArriveAddrData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalArriveAddrData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.map_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.loc_x, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.loc_y, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.range, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.item_id, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.npc_id, str, length);
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
        GoalArriveAddrData& m_data;
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

		json_data.AddIntegral(u8"map_id", map_id);

		json_data.AddIntegral(u8"loc_x", loc_x);

		json_data.AddIntegral(u8"loc_y", loc_y);

		json_data.AddIntegral(u8"range", range);

		json_data.AddIntegral(u8"item_id", item_id);

		json_data.AddIntegral(u8"npc_id", npc_id);

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
        
        GoalArriveAddrData::Handler h(*this, nullptr);
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

struct GoalFightMapData{
	unsigned short            map_id; //地图ID
	unsigned short            count; //次数
	unsigned short            count_required; //要求次数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(map_id);

		net_data.AddIntegral(count);

		net_data.AddIntegral(count_required);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(map_id))
			return false;

		if (!net_data.DelIntegral(count))
			return false;

		if (!net_data.DelIntegral(count_required))
			return false;

		return true;
	}
	void Reset(void)
	{
		map_id = 0;

		count = 0;

		count_required = 0;

	}
	GoalFightMapData& operator= (const GoalFightMapData& src)
	{
		map_id = src.map_id;

		count = src.count;

		count_required = src.count_required;

		return *this;
	}
	bool operator==(const GoalFightMapData& src) const
	{
		if (map_id != src.map_id)
		{
			return false;
		}

		if (count != src.count)
		{
			return false;
		}

		if (count_required != src.count_required)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalFightMapData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalFightMapData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.map_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.count, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.count_required, str, length);
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
        GoalFightMapData& m_data;
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

		json_data.AddIntegral(u8"map_id", map_id);

		json_data.AddIntegral(u8"count", count);

		json_data.AddIntegral(u8"count_required", count_required);

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
        
        GoalFightMapData::Handler h(*this, nullptr);
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

struct GoalFightMapCountData{
	DataArray<GoalFightMapData, unsigned char> fight_map; //地图战斗
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(fight_map);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(fight_map))
			return false;

		return true;
	}
	void Reset(void)
	{
		fight_map.clear();

	}
	GoalFightMapCountData& operator= (const GoalFightMapCountData& src)
	{
		fight_map = src.fight_map;

		return *this;
	}
	bool operator==(const GoalFightMapCountData& src) const
	{
		if (fight_map != src.fight_map)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalFightMapCountData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalFightMapCountData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_fight_map_handler(m_data.fight_map, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_fight_map_handler.ResetState();
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
                return &m_fight_map_handler;
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

            if (!m_fight_map_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        GoalFightMapCountData& m_data;
        DataArrayHandler<GoalFightMapData, unsigned char, GoalFightMapData::Handler> m_fight_map_handler;
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

		json_data.Writer().Key(u8"fight_map", sizeof(u8"fight_map") - 1);
		json_data.AddArray(fight_map);

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
        
        GoalFightMapCountData::Handler h(*this, nullptr);
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

struct GoalKillMonsterData{
	unsigned int              monster_id; //怪物ID
	unsigned short            count; //次数
	unsigned short            count_required; //要求次数
	unsigned short            map_id; //地图ID
	unsigned short            left; //左
	unsigned short            top; //上
	unsigned short            width; //宽
	unsigned short            height; //高
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(monster_id);

		net_data.AddIntegral(count);

		net_data.AddIntegral(count_required);

		net_data.AddIntegral(map_id);

		net_data.AddIntegral(left);

		net_data.AddIntegral(top);

		net_data.AddIntegral(width);

		net_data.AddIntegral(height);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(monster_id))
			return false;

		if (!net_data.DelIntegral(count))
			return false;

		if (!net_data.DelIntegral(count_required))
			return false;

		if (!net_data.DelIntegral(map_id))
			return false;

		if (!net_data.DelIntegral(left))
			return false;

		if (!net_data.DelIntegral(top))
			return false;

		if (!net_data.DelIntegral(width))
			return false;

		if (!net_data.DelIntegral(height))
			return false;

		return true;
	}
	void Reset(void)
	{
		monster_id = 0;

		count = 0;

		count_required = 0;

		map_id = 0;

		left = 0;

		top = 0;

		width = 0;

		height = 0;

	}
	GoalKillMonsterData& operator= (const GoalKillMonsterData& src)
	{
		monster_id = src.monster_id;

		count = src.count;

		count_required = src.count_required;

		map_id = src.map_id;

		left = src.left;

		top = src.top;

		width = src.width;

		height = src.height;

		return *this;
	}
	bool operator==(const GoalKillMonsterData& src) const
	{
		if (monster_id != src.monster_id)
		{
			return false;
		}

		if (count != src.count)
		{
			return false;
		}

		if (count_required != src.count_required)
		{
			return false;
		}

		if (map_id != src.map_id)
		{
			return false;
		}

		if (left != src.left)
		{
			return false;
		}

		if (top != src.top)
		{
			return false;
		}

		if (width != src.width)
		{
			return false;
		}

		if (height != src.height)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalKillMonsterData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalKillMonsterData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.monster_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.count, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.count_required, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.map_id, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.left, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.top, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.width, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.height, str, length);
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
        GoalKillMonsterData& m_data;
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

		json_data.AddIntegral(u8"monster_id", monster_id);

		json_data.AddIntegral(u8"count", count);

		json_data.AddIntegral(u8"count_required", count_required);

		json_data.AddIntegral(u8"map_id", map_id);

		json_data.AddIntegral(u8"left", left);

		json_data.AddIntegral(u8"top", top);

		json_data.AddIntegral(u8"width", width);

		json_data.AddIntegral(u8"height", height);

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
        
        GoalKillMonsterData::Handler h(*this, nullptr);
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

struct GoalKillMonsterCountData{
	DataArray<GoalKillMonsterData, unsigned char> kill_monster; //杀怪
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(kill_monster);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(kill_monster))
			return false;

		return true;
	}
	void Reset(void)
	{
		kill_monster.clear();

	}
	GoalKillMonsterCountData& operator= (const GoalKillMonsterCountData& src)
	{
		kill_monster = src.kill_monster;

		return *this;
	}
	bool operator==(const GoalKillMonsterCountData& src) const
	{
		if (kill_monster != src.kill_monster)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalKillMonsterCountData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalKillMonsterCountData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_kill_monster_handler(m_data.kill_monster, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_kill_monster_handler.ResetState();
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
                return &m_kill_monster_handler;
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

            if (!m_kill_monster_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        GoalKillMonsterCountData& m_data;
        DataArrayHandler<GoalKillMonsterData, unsigned char, GoalKillMonsterData::Handler> m_kill_monster_handler;
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

		json_data.Writer().Key(u8"kill_monster", sizeof(u8"kill_monster") - 1);
		json_data.AddArray(kill_monster);

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
        
        GoalKillMonsterCountData::Handler h(*this, nullptr);
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

struct GoalEncounterFightData{
	unsigned char             cur_num; //次数
	unsigned char             max_num; //最大次数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(cur_num);

		net_data.AddIntegral(max_num);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(cur_num))
			return false;

		if (!net_data.DelIntegral(max_num))
			return false;

		return true;
	}
	void Reset(void)
	{
		cur_num = 0;

		max_num = 0;

	}
	GoalEncounterFightData& operator= (const GoalEncounterFightData& src)
	{
		cur_num = src.cur_num;

		max_num = src.max_num;

		return *this;
	}
	bool operator==(const GoalEncounterFightData& src) const
	{
		if (cur_num != src.cur_num)
		{
			return false;
		}

		if (max_num != src.max_num)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalEncounterFightData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalEncounterFightData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.cur_num, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.max_num, str, length);
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
        GoalEncounterFightData& m_data;
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

		json_data.AddIntegral(u8"cur_num", cur_num);

		json_data.AddIntegral(u8"max_num", max_num);

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
        
        GoalEncounterFightData::Handler h(*this, nullptr);
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

struct GoalUseItemData{
	unsigned short            map_id; //地图ID
	unsigned short            loc_x; //x坐标
	unsigned short            loc_y; //y坐标
	unsigned short            range; //范围
	unsigned int              item_id; //物品ID
	unsigned int              npc_id; //NPC ID
	char                      desc[256]; //描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(map_id);

		net_data.AddIntegral(loc_x);

		net_data.AddIntegral(loc_y);

		net_data.AddIntegral(range);

		net_data.AddIntegral(item_id);

		net_data.AddIntegral(npc_id);

		net_data.AddString(desc, sizeof(desc));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(map_id))
			return false;

		if (!net_data.DelIntegral(loc_x))
			return false;

		if (!net_data.DelIntegral(loc_y))
			return false;

		if (!net_data.DelIntegral(range))
			return false;

		if (!net_data.DelIntegral(item_id))
			return false;

		if (!net_data.DelIntegral(npc_id))
			return false;

		if (!net_data.DelString(desc, sizeof(desc)))
			return false;

		return true;
	}
	void Reset(void)
	{
		map_id = 0;

		loc_x = 0;

		loc_y = 0;

		range = 0;

		item_id = 0;

		npc_id = 0;

		desc[0] = 0;

	}
	GoalUseItemData& operator= (const GoalUseItemData& src)
	{
		map_id = src.map_id;

		loc_x = src.loc_x;

		loc_y = src.loc_y;

		range = src.range;

		item_id = src.item_id;

		npc_id = src.npc_id;

		{
			size_t str_len = strnlen(src.desc, sizeof(desc) - 1);
			memcpy(desc, src.desc, str_len + 1);
			desc[sizeof(desc) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const GoalUseItemData& src) const
	{
		if (map_id != src.map_id)
		{
			return false;
		}

		if (loc_x != src.loc_x)
		{
			return false;
		}

		if (loc_y != src.loc_y)
		{
			return false;
		}

		if (range != src.range)
		{
			return false;
		}

		if (item_id != src.item_id)
		{
			return false;
		}

		if (npc_id != src.npc_id)
		{
			return false;
		}

		if (strncmp(desc, src.desc, sizeof(desc)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalUseItemData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalUseItemData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.map_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.loc_x, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.loc_y, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.range, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.item_id, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.npc_id, str, length);
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
        GoalUseItemData& m_data;
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

		json_data.AddIntegral(u8"map_id", map_id);

		json_data.AddIntegral(u8"loc_x", loc_x);

		json_data.AddIntegral(u8"loc_y", loc_y);

		json_data.AddIntegral(u8"range", range);

		json_data.AddIntegral(u8"item_id", item_id);

		json_data.AddIntegral(u8"npc_id", npc_id);

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
        
        GoalUseItemData::Handler h(*this, nullptr);
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

struct GoalCapturePetData{
	unsigned short            pet_id; //宠物ID
	unsigned short            count; //次数
	unsigned short            count_required; //要求次数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(pet_id);

		net_data.AddIntegral(count);

		net_data.AddIntegral(count_required);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(pet_id))
			return false;

		if (!net_data.DelIntegral(count))
			return false;

		if (!net_data.DelIntegral(count_required))
			return false;

		return true;
	}
	void Reset(void)
	{
		pet_id = 0;

		count = 0;

		count_required = 0;

	}
	GoalCapturePetData& operator= (const GoalCapturePetData& src)
	{
		pet_id = src.pet_id;

		count = src.count;

		count_required = src.count_required;

		return *this;
	}
	bool operator==(const GoalCapturePetData& src) const
	{
		if (pet_id != src.pet_id)
		{
			return false;
		}

		if (count != src.count)
		{
			return false;
		}

		if (count_required != src.count_required)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalCapturePetData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalCapturePetData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.pet_id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.count, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.count_required, str, length);
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
        GoalCapturePetData& m_data;
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

		json_data.AddIntegral(u8"pet_id", pet_id);

		json_data.AddIntegral(u8"count", count);

		json_data.AddIntegral(u8"count_required", count_required);

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
        
        GoalCapturePetData::Handler h(*this, nullptr);
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

struct GoalPVEData{
	unsigned int              win_cur; //胜利次数
	unsigned int              win_required; //要求胜利次数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(win_cur);

		net_data.AddIntegral(win_required);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(win_cur))
			return false;

		if (!net_data.DelIntegral(win_required))
			return false;

		return true;
	}
	void Reset(void)
	{
		win_cur = 0;

		win_required = 0;

	}
	GoalPVEData& operator= (const GoalPVEData& src)
	{
		win_cur = src.win_cur;

		win_required = src.win_required;

		return *this;
	}
	bool operator==(const GoalPVEData& src) const
	{
		if (win_cur != src.win_cur)
		{
			return false;
		}

		if (win_required != src.win_required)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GoalPVEData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GoalPVEData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.win_cur, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.win_required, str, length);
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
        GoalPVEData& m_data;
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

		json_data.AddIntegral(u8"win_cur", win_cur);

		json_data.AddIntegral(u8"win_required", win_required);

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
        
        GoalPVEData::Handler h(*this, nullptr);
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

struct IPConfig{
	unsigned char             type; //服务器类型
	unsigned short            port; //服务器端口
	unsigned int              recv_buf; //接收缓冲大小
	unsigned int              send_buf; //发送缓冲大小
	char                      ip[256]; //服务器IP
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(port);

		net_data.AddIntegral(recv_buf);

		net_data.AddIntegral(send_buf);

		net_data.AddString(ip, sizeof(ip));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(port))
			return false;

		if (!net_data.DelIntegral(recv_buf))
			return false;

		if (!net_data.DelIntegral(send_buf))
			return false;

		if (!net_data.DelString(ip, sizeof(ip)))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		port = 0;

		recv_buf = 0;

		send_buf = 0;

		ip[0] = 0;

	}
	IPConfig& operator= (const IPConfig& src)
	{
		type = src.type;

		port = src.port;

		recv_buf = src.recv_buf;

		send_buf = src.send_buf;

		{
			size_t str_len = strnlen(src.ip, sizeof(ip) - 1);
			memcpy(ip, src.ip, str_len + 1);
			ip[sizeof(ip) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const IPConfig& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (port != src.port)
		{
			return false;
		}

		if (recv_buf != src.recv_buf)
		{
			return false;
		}

		if (send_buf != src.send_buf)
		{
			return false;
		}

		if (strncmp(ip, src.ip, sizeof(ip)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const IPConfig& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(IPConfig& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.port, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.recv_buf, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.send_buf, str, length);
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
                JsonDelString(m_data.ip, str, length);
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
        IPConfig& m_data;
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

		json_data.AddIntegral(u8"type", type);

		json_data.AddIntegral(u8"port", port);

		json_data.AddIntegral(u8"recv_buf", recv_buf);

		json_data.AddIntegral(u8"send_buf", send_buf);

		json_data.AddString(u8"ip", ip, sizeof(ip));

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
        
        IPConfig::Handler h(*this, nullptr);
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

struct ActivityData{
	unsigned short            id; //活动id
	unsigned char             state; //活动状态 0:未开启 1:进行中 2:已过期
	unsigned char             today; //是否今天的活动
	DataArray<unsigned char, unsigned short> custom; //脚本填充的自定义数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(id);

		net_data.AddIntegral(state);

		net_data.AddIntegral(today);

		net_data.AddArray(custom);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(state))
			return false;

		if (!net_data.DelIntegral(today))
			return false;

		if(!net_data.DelArray(custom))
			return false;

		return true;
	}
	void Reset(void)
	{
		id = 0;

		state = 0;

		today = 0;

		custom.clear();

	}
	ActivityData& operator= (const ActivityData& src)
	{
		id = src.id;

		state = src.state;

		today = src.today;

		custom = src.custom;

		return *this;
	}
	bool operator==(const ActivityData& src) const
	{
		if (id != src.id)
		{
			return false;
		}

		if (state != src.state)
		{
			return false;
		}

		if (today != src.today)
		{
			return false;
		}

		if (custom != src.custom)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ActivityData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ActivityData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.id, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.state, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.today, str, length);
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.custom, this);
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
        ActivityData& m_data;
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

		json_data.AddIntegral(u8"id", id);

		json_data.AddIntegral(u8"state", state);

		json_data.AddIntegral(u8"today", today);

		json_data.Writer().Key(u8"custom", sizeof(u8"custom") - 1);
		json_data.AddArray(custom);

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
        
        ActivityData::Handler h(*this, nullptr);
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

struct StringData{
	DataArray<unsigned char, unsigned short> str; //字符串
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(str);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(str))
			return false;

		return true;
	}
	void Reset(void)
	{
		str.clear();

	}
	StringData& operator= (const StringData& src)
	{
		str = src.str;

		return *this;
	}
	bool operator==(const StringData& src) const
	{
		if (str != src.str)
		{
			return false;
		}

		return true;
	}
	bool operator != (const StringData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(StringData& data, JsonHandler* parent = nullptr)
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.str, this);
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
        StringData& m_data;
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

		json_data.Writer().Key(u8"str", sizeof(u8"str") - 1);
		json_data.AddArray(str);

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
        
        StringData::Handler h(*this, nullptr);
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

struct GUIDData{
	unsigned long long        guid; //guids
	bool EnCode(NetEnCode& net_data)
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
	void Reset(void)
	{
		guid = 0;

	}
	GUIDData& operator= (const GUIDData& src)
	{
		guid = src.guid;

		return *this;
	}
	bool operator==(const GUIDData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GUIDData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GUIDData& data, JsonHandler* parent = nullptr)
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
        GUIDData& m_data;
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
        
        GUIDData::Handler h(*this, nullptr);
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

struct AuctionObjectData{
	unsigned long long        guid; //拍卖品guid
	unsigned short            id; //道具ID或宠物ID
	unsigned short            type; //1代表道具 2代表宠物
	int                       custom1; //脚本自定义分类1
	int                       custom2; //脚本自定义分类2
	int                       custom3; //脚本自定义分类3
	unsigned short            level; //道具:无 宠物:等级
	int                       fight_value; //道具:无 宠物:战力
	unsigned short            coin_type; //attr表里的货币属性名
	signed long long          coin_value; //货币值
	unsigned short            amount; //堆叠数量
	unsigned char             state; //是否上架 0:公示中(预留) 1:上架中 2:已下架
	unsigned int              puton_times; //上架时间
	unsigned int              pulloff_times; //下架时间
	unsigned long long        seller_guid; //出售者guid
	char                      seller_name[32]; //出售者姓名
	ItemData                  item_data; //item数据(拍卖品为道具)
	PetData                   pet_data; //pet数据(拍卖品为宠物)
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(id);

		net_data.AddIntegral(type);

		net_data.AddIntegral(custom1);

		net_data.AddIntegral(custom2);

		net_data.AddIntegral(custom3);

		net_data.AddIntegral(level);

		net_data.AddIntegral(fight_value);

		net_data.AddIntegral(coin_type);

		net_data.AddIntegral(coin_value);

		net_data.AddIntegral(amount);

		net_data.AddIntegral(state);

		net_data.AddIntegral(puton_times);

		net_data.AddIntegral(pulloff_times);

		net_data.AddIntegral(seller_guid);

		net_data.AddString(seller_name, sizeof(seller_name));

		item_data.EnCode(net_data);

		pet_data.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(id))
			return false;

		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(custom1))
			return false;

		if (!net_data.DelIntegral(custom2))
			return false;

		if (!net_data.DelIntegral(custom3))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(fight_value))
			return false;

		if (!net_data.DelIntegral(coin_type))
			return false;

		if (!net_data.DelIntegral(coin_value))
			return false;

		if (!net_data.DelIntegral(amount))
			return false;

		if (!net_data.DelIntegral(state))
			return false;

		if (!net_data.DelIntegral(puton_times))
			return false;

		if (!net_data.DelIntegral(pulloff_times))
			return false;

		if (!net_data.DelIntegral(seller_guid))
			return false;

		if (!net_data.DelString(seller_name, sizeof(seller_name)))
			return false;

		if (!item_data.DeCode(net_data))
			return false;

		if (!pet_data.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		id = 0;

		type = 0;

		custom1 = 0;

		custom2 = 0;

		custom3 = 0;

		level = 0;

		fight_value = 0;

		coin_type = 0;

		coin_value = 0;

		amount = 0;

		state = 0;

		puton_times = 0;

		pulloff_times = 0;

		seller_guid = 0;

		seller_name[0] = 0;

		item_data.Reset();

		pet_data.Reset();

	}
	AuctionObjectData& operator= (const AuctionObjectData& src)
	{
		guid = src.guid;

		id = src.id;

		type = src.type;

		custom1 = src.custom1;

		custom2 = src.custom2;

		custom3 = src.custom3;

		level = src.level;

		fight_value = src.fight_value;

		coin_type = src.coin_type;

		coin_value = src.coin_value;

		amount = src.amount;

		state = src.state;

		puton_times = src.puton_times;

		pulloff_times = src.pulloff_times;

		seller_guid = src.seller_guid;

		{
			size_t str_len = strnlen(src.seller_name, sizeof(seller_name) - 1);
			memcpy(seller_name, src.seller_name, str_len + 1);
			seller_name[sizeof(seller_name) - 1] = 0;
		}

		item_data = src.item_data;

		pet_data = src.pet_data;

		return *this;
	}
	bool operator==(const AuctionObjectData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (id != src.id)
		{
			return false;
		}

		if (type != src.type)
		{
			return false;
		}

		if (custom1 != src.custom1)
		{
			return false;
		}

		if (custom2 != src.custom2)
		{
			return false;
		}

		if (custom3 != src.custom3)
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (fight_value != src.fight_value)
		{
			return false;
		}

		if (coin_type != src.coin_type)
		{
			return false;
		}

		if (coin_value != src.coin_value)
		{
			return false;
		}

		if (amount != src.amount)
		{
			return false;
		}

		if (state != src.state)
		{
			return false;
		}

		if (puton_times != src.puton_times)
		{
			return false;
		}

		if (pulloff_times != src.pulloff_times)
		{
			return false;
		}

		if (seller_guid != src.seller_guid)
		{
			return false;
		}

		if (strncmp(seller_name, src.seller_name, sizeof(seller_name)-1))
		{
			return false;
		}

		if (item_data != src.item_data)
		{
			return false;
		}

		if (pet_data != src.pet_data)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AuctionObjectData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AuctionObjectData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_item_data_handler(m_data.item_data, this),
            m_pet_data_handler(m_data.pet_data, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_item_data_handler.ResetState();
            m_pet_data_handler.ResetState();
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
                JsonDelIntegral(m_data.id, str, length);
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
                JsonDelIntegral(m_data.custom1, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.custom2, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.custom3, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.fight_value, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.coin_type, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.coin_value, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.amount, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.state, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.puton_times, str, length);
            }
            break;
            case 13:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.pulloff_times, str, length);
            }
            break;
            case 14:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.seller_guid, str, length);
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
            case 15:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.seller_name, str, length);
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
            case 16:
            {
                m_assigned_bitset.set(m_state);
                return &m_item_data_handler;
            }
            break;
            case 17:
            {
                m_assigned_bitset.set(m_state);
                return &m_pet_data_handler;
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

            if (!m_item_data_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_pet_data_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        AuctionObjectData& m_data;
        ItemData::Handler m_item_data_handler;
        PetData::Handler m_pet_data_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<18> m_assigned_bitset;

        static const std::bitset<18> template_assigned_bitset;
        static const char* m_key_list[18];
        static const char* m_alias_list[18];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"id", id);

		json_data.AddIntegral(u8"type", type);

		json_data.AddIntegral(u8"custom1", custom1);

		json_data.AddIntegral(u8"custom2", custom2);

		json_data.AddIntegral(u8"custom3", custom3);

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"fight_value", fight_value);

		json_data.AddIntegral(u8"coin_type", coin_type);

		json_data.AddIntegral(u8"coin_value", coin_value);

		json_data.AddIntegral(u8"amount", amount);

		json_data.AddIntegral(u8"state", state);

		json_data.AddIntegral(u8"puton_times", puton_times);

		json_data.AddIntegral(u8"pulloff_times", pulloff_times);

		json_data.AddIntegral(u8"seller_guid", seller_guid);

		json_data.AddString(u8"seller_name", seller_name, sizeof(seller_name));

		json_data.Writer().Key(u8"item_data", sizeof(u8"item_data") - 1);
		item_data.ToJson(json_data);

		json_data.Writer().Key(u8"pet_data", sizeof(u8"pet_data") - 1);
		pet_data.ToJson(json_data);

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
        
        AuctionObjectData::Handler h(*this, nullptr);
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

struct AuctionCookieData{
	unsigned long long        guid; //玩家guid
	DataArray<AttrData, unsigned short> coins; //各种货币
	DataArray<GUIDData, unsigned char> favorites; //收藏的拍卖品
	DataArray<StringData, unsigned char> records; //交易记录
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddArray(coins);

		net_data.AddArray(favorites);

		net_data.AddArray(records);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if(!net_data.DelArray(coins))
			return false;

		if(!net_data.DelArray(favorites))
			return false;

		if(!net_data.DelArray(records))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		coins.clear();

		favorites.clear();

		records.clear();

	}
	AuctionCookieData& operator= (const AuctionCookieData& src)
	{
		guid = src.guid;

		coins = src.coins;

		favorites = src.favorites;

		records = src.records;

		return *this;
	}
	bool operator==(const AuctionCookieData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (coins != src.coins)
		{
			return false;
		}

		if (favorites != src.favorites)
		{
			return false;
		}

		if (records != src.records)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AuctionCookieData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AuctionCookieData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_coins_handler(m_data.coins, this),
            m_favorites_handler(m_data.favorites, this),
            m_records_handler(m_data.records, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_coins_handler.ResetState();
            m_favorites_handler.ResetState();
            m_records_handler.ResetState();
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
                return &m_coins_handler;
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_favorites_handler;
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_records_handler;
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

            if (!m_coins_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_favorites_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_records_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        AuctionCookieData& m_data;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_coins_handler;
        DataArrayHandler<GUIDData, unsigned char, GUIDData::Handler> m_favorites_handler;
        DataArrayHandler<StringData, unsigned char, StringData::Handler> m_records_handler;
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

		json_data.Writer().Key(u8"coins", sizeof(u8"coins") - 1);
		json_data.AddArray(coins);

		json_data.Writer().Key(u8"favorites", sizeof(u8"favorites") - 1);
		json_data.AddArray(favorites);

		json_data.Writer().Key(u8"records", sizeof(u8"records") - 1);
		json_data.AddArray(records);

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
        
        AuctionCookieData::Handler h(*this, nullptr);
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

struct GuildMemberBriefData{
	unsigned long long        player_guid; //玩家guid
	char                      player_name[32]; //玩家姓名
	unsigned int              level; //等级
	unsigned char             role; //角色
	unsigned char             guild_job; //成员职位
	unsigned int              total_contrb; //历史贡献度
	unsigned int              join_time; //加入帮派时间
	unsigned int              last_login_time; //上次登陆时间
	unsigned int              last_logout_time; //上次离线时间
	unsigned int              status; //成员玩家状态(参见logic_def的枚举guild_member_status)
	unsigned int              vip; //vip等级
	unsigned int              reincarnation; //转生等级
	unsigned char             job; //职业
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(player_guid);

		net_data.AddString(player_name, sizeof(player_name));

		net_data.AddIntegral(level);

		net_data.AddIntegral(role);

		net_data.AddIntegral(guild_job);

		net_data.AddIntegral(total_contrb);

		net_data.AddIntegral(join_time);

		net_data.AddIntegral(last_login_time);

		net_data.AddIntegral(last_logout_time);

		net_data.AddIntegral(status);

		net_data.AddIntegral(vip);

		net_data.AddIntegral(reincarnation);

		net_data.AddIntegral(job);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelString(player_name, sizeof(player_name)))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(role))
			return false;

		if (!net_data.DelIntegral(guild_job))
			return false;

		if (!net_data.DelIntegral(total_contrb))
			return false;

		if (!net_data.DelIntegral(join_time))
			return false;

		if (!net_data.DelIntegral(last_login_time))
			return false;

		if (!net_data.DelIntegral(last_logout_time))
			return false;

		if (!net_data.DelIntegral(status))
			return false;

		if (!net_data.DelIntegral(vip))
			return false;

		if (!net_data.DelIntegral(reincarnation))
			return false;

		if (!net_data.DelIntegral(job))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		player_guid = 0;

		player_name[0] = 0;

		level = 0;

		role = 0;

		guild_job = 0;

		total_contrb = 0;

		join_time = 0;

		last_login_time = 0;

		last_logout_time = 0;

		status = 0;

		vip = 0;

		reincarnation = 0;

		job = 0;

		customs.Reset();

	}
	GuildMemberBriefData& operator= (const GuildMemberBriefData& src)
	{
		player_guid = src.player_guid;

		{
			size_t str_len = strnlen(src.player_name, sizeof(player_name) - 1);
			memcpy(player_name, src.player_name, str_len + 1);
			player_name[sizeof(player_name) - 1] = 0;
		}

		level = src.level;

		role = src.role;

		guild_job = src.guild_job;

		total_contrb = src.total_contrb;

		join_time = src.join_time;

		last_login_time = src.last_login_time;

		last_logout_time = src.last_logout_time;

		status = src.status;

		vip = src.vip;

		reincarnation = src.reincarnation;

		job = src.job;

		customs = src.customs;

		return *this;
	}
	bool operator==(const GuildMemberBriefData& src) const
	{
		if (player_guid != src.player_guid)
		{
			return false;
		}

		if (strncmp(player_name, src.player_name, sizeof(player_name)-1))
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (role != src.role)
		{
			return false;
		}

		if (guild_job != src.guild_job)
		{
			return false;
		}

		if (total_contrb != src.total_contrb)
		{
			return false;
		}

		if (join_time != src.join_time)
		{
			return false;
		}

		if (last_login_time != src.last_login_time)
		{
			return false;
		}

		if (last_logout_time != src.last_logout_time)
		{
			return false;
		}

		if (status != src.status)
		{
			return false;
		}

		if (vip != src.vip)
		{
			return false;
		}

		if (reincarnation != src.reincarnation)
		{
			return false;
		}

		if (job != src.job)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GuildMemberBriefData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GuildMemberBriefData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.role, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.guild_job, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.total_contrb, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.join_time, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_login_time, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_logout_time, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.status, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.vip, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.reincarnation, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.job, str, length);
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
                JsonDelString(m_data.player_name, str, length);
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
            case 13:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        GuildMemberBriefData& m_data;
        CustomData::Handler m_customs_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<14> m_assigned_bitset;

        static const std::bitset<14> template_assigned_bitset;
        static const char* m_key_list[14];
        static const char* m_alias_list[14];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"player_guid", player_guid);

		json_data.AddString(u8"player_name", player_name, sizeof(player_name));

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"role", role);

		json_data.AddIntegral(u8"guild_job", guild_job);

		json_data.AddIntegral(u8"total_contrb", total_contrb);

		json_data.AddIntegral(u8"join_time", join_time);

		json_data.AddIntegral(u8"last_login_time", last_login_time);

		json_data.AddIntegral(u8"last_logout_time", last_logout_time);

		json_data.AddIntegral(u8"status", status);

		json_data.AddIntegral(u8"vip", vip);

		json_data.AddIntegral(u8"reincarnation", reincarnation);

		json_data.AddIntegral(u8"job", job);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

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
        
        GuildMemberBriefData::Handler h(*this, nullptr);
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

struct GuildMemberData{
	unsigned long long        player_guid; //玩家guid
	char                      player_name[32]; //玩家姓名
	unsigned int              level; //等级
	unsigned char             role; //角色
	unsigned char             guild_job; //成员职位
	unsigned int              total_contrb; //历史贡献度
	unsigned int              surplus_contrb; //剩余贡献度
	unsigned int              last_week_contrb; //上周贡献度
	unsigned int              this_week_contrb; //本周贡献度
	unsigned int              permission; //帮派权限(参见logic_def的枚举guild_permission)
	unsigned int              join_time; //加入帮派时间
	unsigned int              donate_bind_gold; //捐赠金币
	unsigned int              last_donate_time; //上次捐赠时间
	unsigned int              last_dividend_time; //上次分红时间
	unsigned int              last_login_time; //上次登陆时间
	unsigned int              last_logout_time; //上次离线时间
	unsigned int              forbid_end_time; //禁言结束时间
	unsigned int              status; //成员玩家状态(参见logic_def的枚举guild_member_status)
	unsigned int              vip; //vip等级
	unsigned int              reincarnation; //转生等级
	CustomData                customs; //自定义
	unsigned char             job; //职业
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(player_guid);

		net_data.AddString(player_name, sizeof(player_name));

		net_data.AddIntegral(level);

		net_data.AddIntegral(role);

		net_data.AddIntegral(guild_job);

		net_data.AddIntegral(total_contrb);

		net_data.AddIntegral(surplus_contrb);

		net_data.AddIntegral(last_week_contrb);

		net_data.AddIntegral(this_week_contrb);

		net_data.AddIntegral(permission);

		net_data.AddIntegral(join_time);

		net_data.AddIntegral(donate_bind_gold);

		net_data.AddIntegral(last_donate_time);

		net_data.AddIntegral(last_dividend_time);

		net_data.AddIntegral(last_login_time);

		net_data.AddIntegral(last_logout_time);

		net_data.AddIntegral(forbid_end_time);

		net_data.AddIntegral(status);

		net_data.AddIntegral(vip);

		net_data.AddIntegral(reincarnation);

		customs.EnCode(net_data);

		net_data.AddIntegral(job);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelString(player_name, sizeof(player_name)))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(role))
			return false;

		if (!net_data.DelIntegral(guild_job))
			return false;

		if (!net_data.DelIntegral(total_contrb))
			return false;

		if (!net_data.DelIntegral(surplus_contrb))
			return false;

		if (!net_data.DelIntegral(last_week_contrb))
			return false;

		if (!net_data.DelIntegral(this_week_contrb))
			return false;

		if (!net_data.DelIntegral(permission))
			return false;

		if (!net_data.DelIntegral(join_time))
			return false;

		if (!net_data.DelIntegral(donate_bind_gold))
			return false;

		if (!net_data.DelIntegral(last_donate_time))
			return false;

		if (!net_data.DelIntegral(last_dividend_time))
			return false;

		if (!net_data.DelIntegral(last_login_time))
			return false;

		if (!net_data.DelIntegral(last_logout_time))
			return false;

		if (!net_data.DelIntegral(forbid_end_time))
			return false;

		if (!net_data.DelIntegral(status))
			return false;

		if (!net_data.DelIntegral(vip))
			return false;

		if (!net_data.DelIntegral(reincarnation))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(job))
			return false;

		return true;
	}
	void Reset(void)
	{
		player_guid = 0;

		player_name[0] = 0;

		level = 0;

		role = 0;

		guild_job = 0;

		total_contrb = 0;

		surplus_contrb = 0;

		last_week_contrb = 0;

		this_week_contrb = 0;

		permission = 0;

		join_time = 0;

		donate_bind_gold = 0;

		last_donate_time = 0;

		last_dividend_time = 0;

		last_login_time = 0;

		last_logout_time = 0;

		forbid_end_time = 0;

		status = 0;

		vip = 0;

		reincarnation = 0;

		customs.Reset();

		job = 0;

	}
	GuildMemberData& operator= (const GuildMemberData& src)
	{
		player_guid = src.player_guid;

		{
			size_t str_len = strnlen(src.player_name, sizeof(player_name) - 1);
			memcpy(player_name, src.player_name, str_len + 1);
			player_name[sizeof(player_name) - 1] = 0;
		}

		level = src.level;

		role = src.role;

		guild_job = src.guild_job;

		total_contrb = src.total_contrb;

		surplus_contrb = src.surplus_contrb;

		last_week_contrb = src.last_week_contrb;

		this_week_contrb = src.this_week_contrb;

		permission = src.permission;

		join_time = src.join_time;

		donate_bind_gold = src.donate_bind_gold;

		last_donate_time = src.last_donate_time;

		last_dividend_time = src.last_dividend_time;

		last_login_time = src.last_login_time;

		last_logout_time = src.last_logout_time;

		forbid_end_time = src.forbid_end_time;

		status = src.status;

		vip = src.vip;

		reincarnation = src.reincarnation;

		customs = src.customs;

		job = src.job;

		return *this;
	}
	bool operator==(const GuildMemberData& src) const
	{
		if (player_guid != src.player_guid)
		{
			return false;
		}

		if (strncmp(player_name, src.player_name, sizeof(player_name)-1))
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (role != src.role)
		{
			return false;
		}

		if (guild_job != src.guild_job)
		{
			return false;
		}

		if (total_contrb != src.total_contrb)
		{
			return false;
		}

		if (surplus_contrb != src.surplus_contrb)
		{
			return false;
		}

		if (last_week_contrb != src.last_week_contrb)
		{
			return false;
		}

		if (this_week_contrb != src.this_week_contrb)
		{
			return false;
		}

		if (permission != src.permission)
		{
			return false;
		}

		if (join_time != src.join_time)
		{
			return false;
		}

		if (donate_bind_gold != src.donate_bind_gold)
		{
			return false;
		}

		if (last_donate_time != src.last_donate_time)
		{
			return false;
		}

		if (last_dividend_time != src.last_dividend_time)
		{
			return false;
		}

		if (last_login_time != src.last_login_time)
		{
			return false;
		}

		if (last_logout_time != src.last_logout_time)
		{
			return false;
		}

		if (forbid_end_time != src.forbid_end_time)
		{
			return false;
		}

		if (status != src.status)
		{
			return false;
		}

		if (vip != src.vip)
		{
			return false;
		}

		if (reincarnation != src.reincarnation)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		if (job != src.job)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GuildMemberData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GuildMemberData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.role, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.guild_job, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.total_contrb, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.surplus_contrb, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_week_contrb, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.this_week_contrb, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.permission, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.join_time, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.donate_bind_gold, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_donate_time, str, length);
            }
            break;
            case 13:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_dividend_time, str, length);
            }
            break;
            case 14:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_login_time, str, length);
            }
            break;
            case 15:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_logout_time, str, length);
            }
            break;
            case 16:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.forbid_end_time, str, length);
            }
            break;
            case 17:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.status, str, length);
            }
            break;
            case 18:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.vip, str, length);
            }
            break;
            case 19:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.reincarnation, str, length);
            }
            break;
            case 21:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.job, str, length);
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
                JsonDelString(m_data.player_name, str, length);
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
            case 20:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        GuildMemberData& m_data;
        CustomData::Handler m_customs_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<22> m_assigned_bitset;

        static const std::bitset<22> template_assigned_bitset;
        static const char* m_key_list[22];
        static const char* m_alias_list[22];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"player_guid", player_guid);

		json_data.AddString(u8"player_name", player_name, sizeof(player_name));

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"role", role);

		json_data.AddIntegral(u8"guild_job", guild_job);

		json_data.AddIntegral(u8"total_contrb", total_contrb);

		json_data.AddIntegral(u8"surplus_contrb", surplus_contrb);

		json_data.AddIntegral(u8"last_week_contrb", last_week_contrb);

		json_data.AddIntegral(u8"this_week_contrb", this_week_contrb);

		json_data.AddIntegral(u8"permission", permission);

		json_data.AddIntegral(u8"join_time", join_time);

		json_data.AddIntegral(u8"donate_bind_gold", donate_bind_gold);

		json_data.AddIntegral(u8"last_donate_time", last_donate_time);

		json_data.AddIntegral(u8"last_dividend_time", last_dividend_time);

		json_data.AddIntegral(u8"last_login_time", last_login_time);

		json_data.AddIntegral(u8"last_logout_time", last_logout_time);

		json_data.AddIntegral(u8"forbid_end_time", forbid_end_time);

		json_data.AddIntegral(u8"status", status);

		json_data.AddIntegral(u8"vip", vip);

		json_data.AddIntegral(u8"reincarnation", reincarnation);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

		json_data.AddIntegral(u8"job", job);

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
        
        GuildMemberData::Handler h(*this, nullptr);
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

struct GuildApplicantData{
	unsigned long long        guid; //申请人guid
	char                      name[32]; //申请人姓名
	unsigned int              level; //申请人等级
	unsigned char             role; //申请人角色
	unsigned int              sn; //申请人串号
	unsigned int              guild_contrb; //帮贡
	unsigned int              last_apply_time; //申请截止时间
	unsigned int              vip; //vip等级
	unsigned int              reincarnation; //转生等级
	unsigned char             job; //职业
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(level);

		net_data.AddIntegral(role);

		net_data.AddIntegral(sn);

		net_data.AddIntegral(guild_contrb);

		net_data.AddIntegral(last_apply_time);

		net_data.AddIntegral(vip);

		net_data.AddIntegral(reincarnation);

		net_data.AddIntegral(job);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(role))
			return false;

		if (!net_data.DelIntegral(sn))
			return false;

		if (!net_data.DelIntegral(guild_contrb))
			return false;

		if (!net_data.DelIntegral(last_apply_time))
			return false;

		if (!net_data.DelIntegral(vip))
			return false;

		if (!net_data.DelIntegral(reincarnation))
			return false;

		if (!net_data.DelIntegral(job))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		name[0] = 0;

		level = 0;

		role = 0;

		sn = 0;

		guild_contrb = 0;

		last_apply_time = 0;

		vip = 0;

		reincarnation = 0;

		job = 0;

	}
	GuildApplicantData& operator= (const GuildApplicantData& src)
	{
		guid = src.guid;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		level = src.level;

		role = src.role;

		sn = src.sn;

		guild_contrb = src.guild_contrb;

		last_apply_time = src.last_apply_time;

		vip = src.vip;

		reincarnation = src.reincarnation;

		job = src.job;

		return *this;
	}
	bool operator==(const GuildApplicantData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (role != src.role)
		{
			return false;
		}

		if (sn != src.sn)
		{
			return false;
		}

		if (guild_contrb != src.guild_contrb)
		{
			return false;
		}

		if (last_apply_time != src.last_apply_time)
		{
			return false;
		}

		if (vip != src.vip)
		{
			return false;
		}

		if (reincarnation != src.reincarnation)
		{
			return false;
		}

		if (job != src.job)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GuildApplicantData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GuildApplicantData& data, JsonHandler* parent = nullptr)
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.role, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.sn, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.guild_contrb, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_apply_time, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.vip, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.reincarnation, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.job, str, length);
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
        GuildApplicantData& m_data;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<10> m_assigned_bitset;

        static const std::bitset<10> template_assigned_bitset;
        static const char* m_key_list[10];
        static const char* m_alias_list[10];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"role", role);

		json_data.AddIntegral(u8"sn", sn);

		json_data.AddIntegral(u8"guild_contrb", guild_contrb);

		json_data.AddIntegral(u8"last_apply_time", last_apply_time);

		json_data.AddIntegral(u8"vip", vip);

		json_data.AddIntegral(u8"reincarnation", reincarnation);

		json_data.AddIntegral(u8"job", job);

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
        
        GuildApplicantData::Handler h(*this, nullptr);
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

struct GuildEventData{
	unsigned int              event_type; //(参见logic_def的枚举guild_event_type)
	unsigned int              event_time; //时间时间
	DataArray<unsigned char, unsigned short> event_msg; //事件消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(event_type);

		net_data.AddIntegral(event_time);

		net_data.AddArray(event_msg);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(event_type))
			return false;

		if (!net_data.DelIntegral(event_time))
			return false;

		if(!net_data.DelArray(event_msg))
			return false;

		return true;
	}
	void Reset(void)
	{
		event_type = 0;

		event_time = 0;

		event_msg.clear();

	}
	GuildEventData& operator= (const GuildEventData& src)
	{
		event_type = src.event_type;

		event_time = src.event_time;

		event_msg = src.event_msg;

		return *this;
	}
	bool operator==(const GuildEventData& src) const
	{
		if (event_type != src.event_type)
		{
			return false;
		}

		if (event_time != src.event_time)
		{
			return false;
		}

		if (event_msg != src.event_msg)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GuildEventData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GuildEventData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.event_type, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.event_time, str, length);
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.event_msg, this);
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
        GuildEventData& m_data;
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

		json_data.AddIntegral(u8"event_type", event_type);

		json_data.AddIntegral(u8"event_time", event_time);

		json_data.Writer().Key(u8"event_msg", sizeof(u8"event_msg") - 1);
		json_data.AddArray(event_msg);

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
        
        GuildEventData::Handler h(*this, nullptr);
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

struct GuildBriefData{
	unsigned long long        guid; //帮派guid
	unsigned int              guild_id; //帮派id
	char                      name[32]; //帮派名称
	unsigned long long        leader_guid; //帮主guid
	char                      leader_name[32]; //帮主名称
	unsigned int              leader_level; //帮主等级
	unsigned char             leader_role; //帮主角色
	unsigned int              level; //帮派等级
	unsigned short            member_count; //帮派成员数量
	unsigned short            max_member_count; //最大成员数量(对应当前等级)
	char                      declaration[256]; //帮派宣言
	unsigned char             applyed; //是否申请：1是已申请， 0是未申请
	unsigned int              leader_school; //帮主门派ID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(guild_id);

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(leader_guid);

		net_data.AddString(leader_name, sizeof(leader_name));

		net_data.AddIntegral(leader_level);

		net_data.AddIntegral(leader_role);

		net_data.AddIntegral(level);

		net_data.AddIntegral(member_count);

		net_data.AddIntegral(max_member_count);

		net_data.AddString(declaration, sizeof(declaration));

		net_data.AddIntegral(applyed);

		net_data.AddIntegral(leader_school);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(guild_id))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(leader_guid))
			return false;

		if (!net_data.DelString(leader_name, sizeof(leader_name)))
			return false;

		if (!net_data.DelIntegral(leader_level))
			return false;

		if (!net_data.DelIntegral(leader_role))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(member_count))
			return false;

		if (!net_data.DelIntegral(max_member_count))
			return false;

		if (!net_data.DelString(declaration, sizeof(declaration)))
			return false;

		if (!net_data.DelIntegral(applyed))
			return false;

		if (!net_data.DelIntegral(leader_school))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		guild_id = 0;

		name[0] = 0;

		leader_guid = 0;

		leader_name[0] = 0;

		leader_level = 0;

		leader_role = 0;

		level = 0;

		member_count = 0;

		max_member_count = 0;

		declaration[0] = 0;

		applyed = 0;

		leader_school = 0;

	}
	GuildBriefData& operator= (const GuildBriefData& src)
	{
		guid = src.guid;

		guild_id = src.guild_id;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		leader_guid = src.leader_guid;

		{
			size_t str_len = strnlen(src.leader_name, sizeof(leader_name) - 1);
			memcpy(leader_name, src.leader_name, str_len + 1);
			leader_name[sizeof(leader_name) - 1] = 0;
		}

		leader_level = src.leader_level;

		leader_role = src.leader_role;

		level = src.level;

		member_count = src.member_count;

		max_member_count = src.max_member_count;

		{
			size_t str_len = strnlen(src.declaration, sizeof(declaration) - 1);
			memcpy(declaration, src.declaration, str_len + 1);
			declaration[sizeof(declaration) - 1] = 0;
		}

		applyed = src.applyed;

		leader_school = src.leader_school;

		return *this;
	}
	bool operator==(const GuildBriefData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (guild_id != src.guild_id)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (leader_guid != src.leader_guid)
		{
			return false;
		}

		if (strncmp(leader_name, src.leader_name, sizeof(leader_name)-1))
		{
			return false;
		}

		if (leader_level != src.leader_level)
		{
			return false;
		}

		if (leader_role != src.leader_role)
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (member_count != src.member_count)
		{
			return false;
		}

		if (max_member_count != src.max_member_count)
		{
			return false;
		}

		if (strncmp(declaration, src.declaration, sizeof(declaration)-1))
		{
			return false;
		}

		if (applyed != src.applyed)
		{
			return false;
		}

		if (leader_school != src.leader_school)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GuildBriefData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GuildBriefData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.guild_id, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.leader_guid, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.leader_level, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.leader_role, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.member_count, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.max_member_count, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.applyed, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.leader_school, str, length);
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
                JsonDelString(m_data.name, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.leader_name, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.declaration, str, length);
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
        GuildBriefData& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"guild_id", guild_id);

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddIntegral(u8"leader_guid", leader_guid);

		json_data.AddString(u8"leader_name", leader_name, sizeof(leader_name));

		json_data.AddIntegral(u8"leader_level", leader_level);

		json_data.AddIntegral(u8"leader_role", leader_role);

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"member_count", member_count);

		json_data.AddIntegral(u8"max_member_count", max_member_count);

		json_data.AddString(u8"declaration", declaration, sizeof(declaration));

		json_data.AddIntegral(u8"applyed", applyed);

		json_data.AddIntegral(u8"leader_school", leader_school);

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
        
        GuildBriefData::Handler h(*this, nullptr);
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

struct GuildData{
	unsigned long long        guid; //帮派guid
	unsigned int              guild_id; //帮派id
	char                      name[32]; //帮派名称
	unsigned long long        leader_guid; //帮主GUID
	char                      leader_name[32]; //帮主名称
	unsigned int              level; //帮派等级
	unsigned int              base_level; //帮派忠义堂等级
	unsigned int              wing_room_level; //帮派厢房/赏功堂等级
	unsigned int              vault_level; //帮派金库/帮派商店等级
	unsigned int              academy_level; //帮派书院/堂口等级
	unsigned int              pharmacy_level; //帮派宝阁等级
	unsigned int              upgrade_cooldown; //帮派建筑升级冷却时间
	unsigned short            member_count; //帮派成员数量
	unsigned int              max_member_count; //最大成员数量(对应当前等级)
	unsigned short            online_count; //在线成员数量
	unsigned int              fund; //帮派资金
	unsigned int              liveness; //活跃度
	unsigned int              build_degree; //帮派建设度
	unsigned int              prestige; //威望
	unsigned int              fight_score; //战绩
	unsigned int              config; //帮派设置
	unsigned int              create_time; //创建时间
	unsigned int              dismissed_time; //解散时间(为0表示不会解散，不为0表示解散最后时限)
	unsigned int              impeach_time; //弹劾帮主开始时间
	unsigned long long        impeach_guid; //弹劾帮主者的GUID
	char                      declaration[256]; //帮派宣言
	DataArray<GuildEventData, unsigned short> events; //帮派事件消息
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(guild_id);

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(leader_guid);

		net_data.AddString(leader_name, sizeof(leader_name));

		net_data.AddIntegral(level);

		net_data.AddIntegral(base_level);

		net_data.AddIntegral(wing_room_level);

		net_data.AddIntegral(vault_level);

		net_data.AddIntegral(academy_level);

		net_data.AddIntegral(pharmacy_level);

		net_data.AddIntegral(upgrade_cooldown);

		net_data.AddIntegral(member_count);

		net_data.AddIntegral(max_member_count);

		net_data.AddIntegral(online_count);

		net_data.AddIntegral(fund);

		net_data.AddIntegral(liveness);

		net_data.AddIntegral(build_degree);

		net_data.AddIntegral(prestige);

		net_data.AddIntegral(fight_score);

		net_data.AddIntegral(config);

		net_data.AddIntegral(create_time);

		net_data.AddIntegral(dismissed_time);

		net_data.AddIntegral(impeach_time);

		net_data.AddIntegral(impeach_guid);

		net_data.AddString(declaration, sizeof(declaration));

		net_data.AddArray(events);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(guild_id))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(leader_guid))
			return false;

		if (!net_data.DelString(leader_name, sizeof(leader_name)))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(base_level))
			return false;

		if (!net_data.DelIntegral(wing_room_level))
			return false;

		if (!net_data.DelIntegral(vault_level))
			return false;

		if (!net_data.DelIntegral(academy_level))
			return false;

		if (!net_data.DelIntegral(pharmacy_level))
			return false;

		if (!net_data.DelIntegral(upgrade_cooldown))
			return false;

		if (!net_data.DelIntegral(member_count))
			return false;

		if (!net_data.DelIntegral(max_member_count))
			return false;

		if (!net_data.DelIntegral(online_count))
			return false;

		if (!net_data.DelIntegral(fund))
			return false;

		if (!net_data.DelIntegral(liveness))
			return false;

		if (!net_data.DelIntegral(build_degree))
			return false;

		if (!net_data.DelIntegral(prestige))
			return false;

		if (!net_data.DelIntegral(fight_score))
			return false;

		if (!net_data.DelIntegral(config))
			return false;

		if (!net_data.DelIntegral(create_time))
			return false;

		if (!net_data.DelIntegral(dismissed_time))
			return false;

		if (!net_data.DelIntegral(impeach_time))
			return false;

		if (!net_data.DelIntegral(impeach_guid))
			return false;

		if (!net_data.DelString(declaration, sizeof(declaration)))
			return false;

		if(!net_data.DelArray(events))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		guild_id = 0;

		name[0] = 0;

		leader_guid = 0;

		leader_name[0] = 0;

		level = 0;

		base_level = 0;

		wing_room_level = 0;

		vault_level = 0;

		academy_level = 0;

		pharmacy_level = 0;

		upgrade_cooldown = 0;

		member_count = 0;

		max_member_count = 0;

		online_count = 0;

		fund = 0;

		liveness = 0;

		build_degree = 0;

		prestige = 0;

		fight_score = 0;

		config = 0;

		create_time = 0;

		dismissed_time = 0;

		impeach_time = 0;

		impeach_guid = 0;

		declaration[0] = 0;

		events.clear();

		customs.Reset();

	}
	GuildData& operator= (const GuildData& src)
	{
		guid = src.guid;

		guild_id = src.guild_id;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		leader_guid = src.leader_guid;

		{
			size_t str_len = strnlen(src.leader_name, sizeof(leader_name) - 1);
			memcpy(leader_name, src.leader_name, str_len + 1);
			leader_name[sizeof(leader_name) - 1] = 0;
		}

		level = src.level;

		base_level = src.base_level;

		wing_room_level = src.wing_room_level;

		vault_level = src.vault_level;

		academy_level = src.academy_level;

		pharmacy_level = src.pharmacy_level;

		upgrade_cooldown = src.upgrade_cooldown;

		member_count = src.member_count;

		max_member_count = src.max_member_count;

		online_count = src.online_count;

		fund = src.fund;

		liveness = src.liveness;

		build_degree = src.build_degree;

		prestige = src.prestige;

		fight_score = src.fight_score;

		config = src.config;

		create_time = src.create_time;

		dismissed_time = src.dismissed_time;

		impeach_time = src.impeach_time;

		impeach_guid = src.impeach_guid;

		{
			size_t str_len = strnlen(src.declaration, sizeof(declaration) - 1);
			memcpy(declaration, src.declaration, str_len + 1);
			declaration[sizeof(declaration) - 1] = 0;
		}

		events = src.events;

		customs = src.customs;

		return *this;
	}
	bool operator==(const GuildData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (guild_id != src.guild_id)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (leader_guid != src.leader_guid)
		{
			return false;
		}

		if (strncmp(leader_name, src.leader_name, sizeof(leader_name)-1))
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (base_level != src.base_level)
		{
			return false;
		}

		if (wing_room_level != src.wing_room_level)
		{
			return false;
		}

		if (vault_level != src.vault_level)
		{
			return false;
		}

		if (academy_level != src.academy_level)
		{
			return false;
		}

		if (pharmacy_level != src.pharmacy_level)
		{
			return false;
		}

		if (upgrade_cooldown != src.upgrade_cooldown)
		{
			return false;
		}

		if (member_count != src.member_count)
		{
			return false;
		}

		if (max_member_count != src.max_member_count)
		{
			return false;
		}

		if (online_count != src.online_count)
		{
			return false;
		}

		if (fund != src.fund)
		{
			return false;
		}

		if (liveness != src.liveness)
		{
			return false;
		}

		if (build_degree != src.build_degree)
		{
			return false;
		}

		if (prestige != src.prestige)
		{
			return false;
		}

		if (fight_score != src.fight_score)
		{
			return false;
		}

		if (config != src.config)
		{
			return false;
		}

		if (create_time != src.create_time)
		{
			return false;
		}

		if (dismissed_time != src.dismissed_time)
		{
			return false;
		}

		if (impeach_time != src.impeach_time)
		{
			return false;
		}

		if (impeach_guid != src.impeach_guid)
		{
			return false;
		}

		if (strncmp(declaration, src.declaration, sizeof(declaration)-1))
		{
			return false;
		}

		if (events != src.events)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const GuildData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(GuildData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_events_handler(m_data.events, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
            m_events_handler.ResetState();
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
                JsonDelIntegral(m_data.guild_id, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.leader_guid, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.base_level, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.wing_room_level, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.vault_level, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.academy_level, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.pharmacy_level, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.upgrade_cooldown, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.member_count, str, length);
            }
            break;
            case 13:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.max_member_count, str, length);
            }
            break;
            case 14:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.online_count, str, length);
            }
            break;
            case 15:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.fund, str, length);
            }
            break;
            case 16:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.liveness, str, length);
            }
            break;
            case 17:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.build_degree, str, length);
            }
            break;
            case 18:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.prestige, str, length);
            }
            break;
            case 19:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.fight_score, str, length);
            }
            break;
            case 20:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.config, str, length);
            }
            break;
            case 21:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.create_time, str, length);
            }
            break;
            case 22:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.dismissed_time, str, length);
            }
            break;
            case 23:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.impeach_time, str, length);
            }
            break;
            case 24:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.impeach_guid, str, length);
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
                JsonDelString(m_data.name, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.leader_name, str, length);
            }
            break;
            case 25:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.declaration, str, length);
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
            case 27:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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
            switch (m_state)
            {
            case 26:
            {
                m_assigned_bitset.set(m_state);
                return &m_events_handler;
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

            if (!m_events_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        GuildData& m_data;
        CustomData::Handler m_customs_handler;
        DataArrayHandler<GuildEventData, unsigned short, GuildEventData::Handler> m_events_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<28> m_assigned_bitset;

        static const std::bitset<28> template_assigned_bitset;
        static const char* m_key_list[28];
        static const char* m_alias_list[28];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"guild_id", guild_id);

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddIntegral(u8"leader_guid", leader_guid);

		json_data.AddString(u8"leader_name", leader_name, sizeof(leader_name));

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"base_level", base_level);

		json_data.AddIntegral(u8"wing_room_level", wing_room_level);

		json_data.AddIntegral(u8"vault_level", vault_level);

		json_data.AddIntegral(u8"academy_level", academy_level);

		json_data.AddIntegral(u8"pharmacy_level", pharmacy_level);

		json_data.AddIntegral(u8"upgrade_cooldown", upgrade_cooldown);

		json_data.AddIntegral(u8"member_count", member_count);

		json_data.AddIntegral(u8"max_member_count", max_member_count);

		json_data.AddIntegral(u8"online_count", online_count);

		json_data.AddIntegral(u8"fund", fund);

		json_data.AddIntegral(u8"liveness", liveness);

		json_data.AddIntegral(u8"build_degree", build_degree);

		json_data.AddIntegral(u8"prestige", prestige);

		json_data.AddIntegral(u8"fight_score", fight_score);

		json_data.AddIntegral(u8"config", config);

		json_data.AddIntegral(u8"create_time", create_time);

		json_data.AddIntegral(u8"dismissed_time", dismissed_time);

		json_data.AddIntegral(u8"impeach_time", impeach_time);

		json_data.AddIntegral(u8"impeach_guid", impeach_guid);

		json_data.AddString(u8"declaration", declaration, sizeof(declaration));

		json_data.Writer().Key(u8"events", sizeof(u8"events") - 1);
		json_data.AddArray(events);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

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
        
        GuildData::Handler h(*this, nullptr);
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

struct NoticeData{
	unsigned int              notice_time; //发送通知时间
	char                      notice[1024]; //通知内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(notice_time);

		net_data.AddString(notice, sizeof(notice));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(notice_time))
			return false;

		if (!net_data.DelString(notice, sizeof(notice)))
			return false;

		return true;
	}
	void Reset(void)
	{
		notice_time = 0;

		notice[0] = 0;

	}
	NoticeData& operator= (const NoticeData& src)
	{
		notice_time = src.notice_time;

		{
			size_t str_len = strnlen(src.notice, sizeof(notice) - 1);
			memcpy(notice, src.notice, str_len + 1);
			notice[sizeof(notice) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const NoticeData& src) const
	{
		if (notice_time != src.notice_time)
		{
			return false;
		}

		if (strncmp(notice, src.notice, sizeof(notice)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const NoticeData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(NoticeData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.notice_time, str, length);
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
                JsonDelString(m_data.notice, str, length);
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
        NoticeData& m_data;
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

		json_data.AddIntegral(u8"notice_time", notice_time);

		json_data.AddString(u8"notice", notice, sizeof(notice));

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
        
        NoticeData::Handler h(*this, nullptr);
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

struct DBGuildData{
	GuildData                 guild; //帮派数据
	NoticeData                notice; //通知数据
	DataArray<ItemData, unsigned char> items; //物品列表
	bool EnCode(NetEnCode& net_data)
	{
		guild.EnCode(net_data);

		notice.EnCode(net_data);

		net_data.AddArray(items);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!guild.DeCode(net_data))
			return false;

		if (!notice.DeCode(net_data))
			return false;

		if(!net_data.DelArray(items))
			return false;

		return true;
	}
	void Reset(void)
	{
		guild.Reset();

		notice.Reset();

		items.clear();

	}
	DBGuildData& operator= (const DBGuildData& src)
	{
		guild = src.guild;

		notice = src.notice;

		items = src.items;

		return *this;
	}
	bool operator==(const DBGuildData& src) const
	{
		if (guild != src.guild)
		{
			return false;
		}

		if (notice != src.notice)
		{
			return false;
		}

		if (items != src.items)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DBGuildData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DBGuildData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_guild_handler(m_data.guild, this),
            m_notice_handler(m_data.notice, this),
            m_items_handler(m_data.items, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_guild_handler.ResetState();
            m_notice_handler.ResetState();
            m_items_handler.ResetState();
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
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_notice_handler;
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
            switch (m_state)
            {
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_items_handler;
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

            if (!m_guild_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_notice_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_items_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        DBGuildData& m_data;
        GuildData::Handler m_guild_handler;
        NoticeData::Handler m_notice_handler;
        DataArrayHandler<ItemData, unsigned char, ItemData::Handler> m_items_handler;
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

		json_data.Writer().Key(u8"guild", sizeof(u8"guild") - 1);
		guild.ToJson(json_data);

		json_data.Writer().Key(u8"notice", sizeof(u8"notice") - 1);
		notice.ToJson(json_data);

		json_data.Writer().Key(u8"items", sizeof(u8"items") - 1);
		json_data.AddArray(items);

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
        
        DBGuildData::Handler h(*this, nullptr);
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

struct DBGuildMemberData{
	GuildMemberData           member; //帮派成员数据
	unsigned long long        guild_guid; //帮派guid
	signed long long          total_fight_value; //战力
	bool EnCode(NetEnCode& net_data)
	{
		member.EnCode(net_data);

		net_data.AddIntegral(guild_guid);

		net_data.AddIntegral(total_fight_value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!member.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(guild_guid))
			return false;

		if (!net_data.DelIntegral(total_fight_value))
			return false;

		return true;
	}
	void Reset(void)
	{
		member.Reset();

		guild_guid = 0;

		total_fight_value = 0;

	}
	DBGuildMemberData& operator= (const DBGuildMemberData& src)
	{
		member = src.member;

		guild_guid = src.guild_guid;

		total_fight_value = src.total_fight_value;

		return *this;
	}
	bool operator==(const DBGuildMemberData& src) const
	{
		if (member != src.member)
		{
			return false;
		}

		if (guild_guid != src.guild_guid)
		{
			return false;
		}

		if (total_fight_value != src.total_fight_value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DBGuildMemberData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DBGuildMemberData& data, JsonHandler* parent = nullptr)
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
            (void)copy;

            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.guild_guid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.total_fight_value, str, length);
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
        DBGuildMemberData& m_data;
        GuildMemberData::Handler m_member_handler;
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

		json_data.Writer().Key(u8"member", sizeof(u8"member") - 1);
		member.ToJson(json_data);

		json_data.AddIntegral(u8"guild_guid", guild_guid);

		json_data.AddIntegral(u8"total_fight_value", total_fight_value);

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
        
        DBGuildMemberData::Handler h(*this, nullptr);
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

struct DBGuildApplicantData{
	GuildApplicantData        applicant; //帮派申请数据
	unsigned long long        guild_guid; //帮派guid
	signed long long          total_fight_value; //战力
	bool EnCode(NetEnCode& net_data)
	{
		applicant.EnCode(net_data);

		net_data.AddIntegral(guild_guid);

		net_data.AddIntegral(total_fight_value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!applicant.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(guild_guid))
			return false;

		if (!net_data.DelIntegral(total_fight_value))
			return false;

		return true;
	}
	void Reset(void)
	{
		applicant.Reset();

		guild_guid = 0;

		total_fight_value = 0;

	}
	DBGuildApplicantData& operator= (const DBGuildApplicantData& src)
	{
		applicant = src.applicant;

		guild_guid = src.guild_guid;

		total_fight_value = src.total_fight_value;

		return *this;
	}
	bool operator==(const DBGuildApplicantData& src) const
	{
		if (applicant != src.applicant)
		{
			return false;
		}

		if (guild_guid != src.guild_guid)
		{
			return false;
		}

		if (total_fight_value != src.total_fight_value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DBGuildApplicantData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DBGuildApplicantData& data, JsonHandler* parent = nullptr)
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
            (void)copy;

            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.guild_guid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.total_fight_value, str, length);
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
        DBGuildApplicantData& m_data;
        GuildApplicantData::Handler m_applicant_handler;
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

		json_data.Writer().Key(u8"applicant", sizeof(u8"applicant") - 1);
		applicant.ToJson(json_data);

		json_data.AddIntegral(u8"guild_guid", guild_guid);

		json_data.AddIntegral(u8"total_fight_value", total_fight_value);

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
        
        DBGuildApplicantData::Handler h(*this, nullptr);
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

struct ContactData{
	unsigned long long        guid; //联系人GUID
	unsigned char             contact_type; //联系人类型：对应枚举ContactType的最近联系人、好友、黑名单
	char                      name[32]; //好友名字
	unsigned char             role; //role
	unsigned char             job; //职业
	unsigned int              level; //等级
	unsigned int              friendship; //好友度
	unsigned int              last_contact_time; //最近联系时间
	int                       sn; //用户序列号
	unsigned int              status; //是否在线：1在线，0离线
	unsigned int              vip; //vip等级
	unsigned int              reincarnation; //转生等级
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(contact_type);

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(role);

		net_data.AddIntegral(job);

		net_data.AddIntegral(level);

		net_data.AddIntegral(friendship);

		net_data.AddIntegral(last_contact_time);

		net_data.AddIntegral(sn);

		net_data.AddIntegral(status);

		net_data.AddIntegral(vip);

		net_data.AddIntegral(reincarnation);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(contact_type))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(role))
			return false;

		if (!net_data.DelIntegral(job))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(friendship))
			return false;

		if (!net_data.DelIntegral(last_contact_time))
			return false;

		if (!net_data.DelIntegral(sn))
			return false;

		if (!net_data.DelIntegral(status))
			return false;

		if (!net_data.DelIntegral(vip))
			return false;

		if (!net_data.DelIntegral(reincarnation))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		contact_type = 0;

		name[0] = 0;

		role = 0;

		job = 0;

		level = 0;

		friendship = 0;

		last_contact_time = 0;

		sn = 0;

		status = 0;

		vip = 0;

		reincarnation = 0;

		customs.Reset();

	}
	ContactData& operator= (const ContactData& src)
	{
		guid = src.guid;

		contact_type = src.contact_type;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		role = src.role;

		job = src.job;

		level = src.level;

		friendship = src.friendship;

		last_contact_time = src.last_contact_time;

		sn = src.sn;

		status = src.status;

		vip = src.vip;

		reincarnation = src.reincarnation;

		customs = src.customs;

		return *this;
	}
	bool operator==(const ContactData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (contact_type != src.contact_type)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (role != src.role)
		{
			return false;
		}

		if (job != src.job)
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (friendship != src.friendship)
		{
			return false;
		}

		if (last_contact_time != src.last_contact_time)
		{
			return false;
		}

		if (sn != src.sn)
		{
			return false;
		}

		if (status != src.status)
		{
			return false;
		}

		if (vip != src.vip)
		{
			return false;
		}

		if (reincarnation != src.reincarnation)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ContactData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ContactData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
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
                JsonDelIntegral(m_data.contact_type, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.role, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.job, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.friendship, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_contact_time, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.sn, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.status, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.vip, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.reincarnation, str, length);
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
            switch (m_state)
            {
            case 12:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ContactData& m_data;
        CustomData::Handler m_customs_handler;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddIntegral(u8"contact_type", contact_type);

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddIntegral(u8"role", role);

		json_data.AddIntegral(u8"job", job);

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"friendship", friendship);

		json_data.AddIntegral(u8"last_contact_time", last_contact_time);

		json_data.AddIntegral(u8"sn", sn);

		json_data.AddIntegral(u8"status", status);

		json_data.AddIntegral(u8"vip", vip);

		json_data.AddIntegral(u8"reincarnation", reincarnation);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

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
        
        ContactData::Handler h(*this, nullptr);
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

struct DBContactData{
	unsigned long long        player_guid; //玩家GUID
	unsigned long long        contact_guid; //联系人GUID
	unsigned char             contact_type; //联系人类型：对应枚举ContactType的最近联系人、好友、黑名单
	unsigned int              friendship; //好友度
	unsigned int              last_contact_time; //最近联系时间
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(player_guid);

		net_data.AddIntegral(contact_guid);

		net_data.AddIntegral(contact_type);

		net_data.AddIntegral(friendship);

		net_data.AddIntegral(last_contact_time);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelIntegral(contact_guid))
			return false;

		if (!net_data.DelIntegral(contact_type))
			return false;

		if (!net_data.DelIntegral(friendship))
			return false;

		if (!net_data.DelIntegral(last_contact_time))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		player_guid = 0;

		contact_guid = 0;

		contact_type = 0;

		friendship = 0;

		last_contact_time = 0;

		customs.Reset();

	}
	DBContactData& operator= (const DBContactData& src)
	{
		player_guid = src.player_guid;

		contact_guid = src.contact_guid;

		contact_type = src.contact_type;

		friendship = src.friendship;

		last_contact_time = src.last_contact_time;

		customs = src.customs;

		return *this;
	}
	bool operator==(const DBContactData& src) const
	{
		if (player_guid != src.player_guid)
		{
			return false;
		}

		if (contact_guid != src.contact_guid)
		{
			return false;
		}

		if (contact_type != src.contact_type)
		{
			return false;
		}

		if (friendship != src.friendship)
		{
			return false;
		}

		if (last_contact_time != src.last_contact_time)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DBContactData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DBContactData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
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
                JsonDelIntegral(m_data.contact_guid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.contact_type, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.friendship, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_contact_time, str, length);
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
            case 5:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        DBContactData& m_data;
        CustomData::Handler m_customs_handler;
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

		json_data.AddIntegral(u8"player_guid", player_guid);

		json_data.AddIntegral(u8"contact_guid", contact_guid);

		json_data.AddIntegral(u8"contact_type", contact_type);

		json_data.AddIntegral(u8"friendship", friendship);

		json_data.AddIntegral(u8"last_contact_time", last_contact_time);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

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
        
        DBContactData::Handler h(*this, nullptr);
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

struct MessageData{
	DataArray<unsigned char, unsigned short> message; //发送信息内容
	unsigned long long        send_guid; //发送者GUID
	unsigned int              send_time; //发送时间
	unsigned long long        recv_guid; //接受者GUID
	unsigned char             auto_reply; //是否自动回复消息
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(message);

		net_data.AddIntegral(send_guid);

		net_data.AddIntegral(send_time);

		net_data.AddIntegral(recv_guid);

		net_data.AddIntegral(auto_reply);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(message))
			return false;

		if (!net_data.DelIntegral(send_guid))
			return false;

		if (!net_data.DelIntegral(send_time))
			return false;

		if (!net_data.DelIntegral(recv_guid))
			return false;

		if (!net_data.DelIntegral(auto_reply))
			return false;

		return true;
	}
	void Reset(void)
	{
		message.clear();

		send_guid = 0;

		send_time = 0;

		recv_guid = 0;

		auto_reply = 0;

	}
	MessageData& operator= (const MessageData& src)
	{
		message = src.message;

		send_guid = src.send_guid;

		send_time = src.send_time;

		recv_guid = src.recv_guid;

		auto_reply = src.auto_reply;

		return *this;
	}
	bool operator==(const MessageData& src) const
	{
		if (message != src.message)
		{
			return false;
		}

		if (send_guid != src.send_guid)
		{
			return false;
		}

		if (send_time != src.send_time)
		{
			return false;
		}

		if (recv_guid != src.recv_guid)
		{
			return false;
		}

		if (auto_reply != src.auto_reply)
		{
			return false;
		}

		return true;
	}
	bool operator != (const MessageData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(MessageData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.send_guid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.send_time, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.recv_guid, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.auto_reply, str, length);
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.message, this);
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
        MessageData& m_data;
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

		json_data.Writer().Key(u8"message", sizeof(u8"message") - 1);
		json_data.AddArray(message);

		json_data.AddIntegral(u8"send_guid", send_guid);

		json_data.AddIntegral(u8"send_time", send_time);

		json_data.AddIntegral(u8"recv_guid", recv_guid);

		json_data.AddIntegral(u8"auto_reply", auto_reply);

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
        
        MessageData::Handler h(*this, nullptr);
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

struct ContactInfoData{
	unsigned long long        guid; //联系人GUID
	char                      name[32]; //好友名字
	unsigned char             role; //role
	unsigned char             job; //职业
	unsigned int              level; //等级
	int                       sn; //用户序列号
	unsigned int              status; //是否在线：1在线，0离线
	unsigned int              vip; //vip等级
	unsigned int              reincarnation; //转生等级
	unsigned int              last_login_time; //上次登录时间
	unsigned int              last_logout_time; //上次登出时间
	unsigned long long        total_fight_value; //总战力
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddString(name, sizeof(name));

		net_data.AddIntegral(role);

		net_data.AddIntegral(job);

		net_data.AddIntegral(level);

		net_data.AddIntegral(sn);

		net_data.AddIntegral(status);

		net_data.AddIntegral(vip);

		net_data.AddIntegral(reincarnation);

		net_data.AddIntegral(last_login_time);

		net_data.AddIntegral(last_logout_time);

		net_data.AddIntegral(total_fight_value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelIntegral(role))
			return false;

		if (!net_data.DelIntegral(job))
			return false;

		if (!net_data.DelIntegral(level))
			return false;

		if (!net_data.DelIntegral(sn))
			return false;

		if (!net_data.DelIntegral(status))
			return false;

		if (!net_data.DelIntegral(vip))
			return false;

		if (!net_data.DelIntegral(reincarnation))
			return false;

		if (!net_data.DelIntegral(last_login_time))
			return false;

		if (!net_data.DelIntegral(last_logout_time))
			return false;

		if (!net_data.DelIntegral(total_fight_value))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		name[0] = 0;

		role = 0;

		job = 0;

		level = 0;

		sn = 0;

		status = 0;

		vip = 0;

		reincarnation = 0;

		last_login_time = 0;

		last_logout_time = 0;

		total_fight_value = 0;

	}
	ContactInfoData& operator= (const ContactInfoData& src)
	{
		guid = src.guid;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		role = src.role;

		job = src.job;

		level = src.level;

		sn = src.sn;

		status = src.status;

		vip = src.vip;

		reincarnation = src.reincarnation;

		last_login_time = src.last_login_time;

		last_logout_time = src.last_logout_time;

		total_fight_value = src.total_fight_value;

		return *this;
	}
	bool operator==(const ContactInfoData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (role != src.role)
		{
			return false;
		}

		if (job != src.job)
		{
			return false;
		}

		if (level != src.level)
		{
			return false;
		}

		if (sn != src.sn)
		{
			return false;
		}

		if (status != src.status)
		{
			return false;
		}

		if (vip != src.vip)
		{
			return false;
		}

		if (reincarnation != src.reincarnation)
		{
			return false;
		}

		if (last_login_time != src.last_login_time)
		{
			return false;
		}

		if (last_logout_time != src.last_logout_time)
		{
			return false;
		}

		if (total_fight_value != src.total_fight_value)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ContactInfoData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ContactInfoData& data, JsonHandler* parent = nullptr)
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.role, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.job, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.level, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.sn, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.status, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.vip, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.reincarnation, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_login_time, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.last_logout_time, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.total_fight_value, str, length);
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
        ContactInfoData& m_data;
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

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddIntegral(u8"role", role);

		json_data.AddIntegral(u8"job", job);

		json_data.AddIntegral(u8"level", level);

		json_data.AddIntegral(u8"sn", sn);

		json_data.AddIntegral(u8"status", status);

		json_data.AddIntegral(u8"vip", vip);

		json_data.AddIntegral(u8"reincarnation", reincarnation);

		json_data.AddIntegral(u8"last_login_time", last_login_time);

		json_data.AddIntegral(u8"last_logout_time", last_logout_time);

		json_data.AddIntegral(u8"total_fight_value", total_fight_value);

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
        
        ContactInfoData::Handler h(*this, nullptr);
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

struct MailHeadData{
	unsigned long long        mail_guid; //邮件GUID
	unsigned char             mail_type; //邮件类型 0:玩家邮件；1:系统邮件；2:帮派邮件；3:脚本邮件
	DataArray<unsigned char, unsigned char> title; //标题
	unsigned long long        send_guid; //发件人GUID
	char                      send_name[32]; //发件人名字
	unsigned int              send_time; //发件时间
	unsigned int              status; //邮件状态
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(mail_guid);

		net_data.AddIntegral(mail_type);

		net_data.AddArray(title);

		net_data.AddIntegral(send_guid);

		net_data.AddString(send_name, sizeof(send_name));

		net_data.AddIntegral(send_time);

		net_data.AddIntegral(status);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(mail_guid))
			return false;

		if (!net_data.DelIntegral(mail_type))
			return false;

		if(!net_data.DelArray(title))
			return false;

		if (!net_data.DelIntegral(send_guid))
			return false;

		if (!net_data.DelString(send_name, sizeof(send_name)))
			return false;

		if (!net_data.DelIntegral(send_time))
			return false;

		if (!net_data.DelIntegral(status))
			return false;

		return true;
	}
	void Reset(void)
	{
		mail_guid = 0;

		mail_type = 0;

		title.clear();

		send_guid = 0;

		send_name[0] = 0;

		send_time = 0;

		status = 0;

	}
	MailHeadData& operator= (const MailHeadData& src)
	{
		mail_guid = src.mail_guid;

		mail_type = src.mail_type;

		title = src.title;

		send_guid = src.send_guid;

		{
			size_t str_len = strnlen(src.send_name, sizeof(send_name) - 1);
			memcpy(send_name, src.send_name, str_len + 1);
			send_name[sizeof(send_name) - 1] = 0;
		}

		send_time = src.send_time;

		status = src.status;

		return *this;
	}
	bool operator==(const MailHeadData& src) const
	{
		if (mail_guid != src.mail_guid)
		{
			return false;
		}

		if (mail_type != src.mail_type)
		{
			return false;
		}

		if (title != src.title)
		{
			return false;
		}

		if (send_guid != src.send_guid)
		{
			return false;
		}

		if (strncmp(send_name, src.send_name, sizeof(send_name)-1))
		{
			return false;
		}

		if (send_time != src.send_time)
		{
			return false;
		}

		if (status != src.status)
		{
			return false;
		}

		return true;
	}
	bool operator != (const MailHeadData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(MailHeadData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.mail_type, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.send_guid, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.send_time, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.status, str, length);
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
                DataArrayHandler<unsigned char, unsigned char> handler(m_data.title, this);
                handler.String(str, length, copy);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.send_name, str, length);
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
        MailHeadData& m_data;
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

		json_data.AddIntegral(u8"mail_guid", mail_guid);

		json_data.AddIntegral(u8"mail_type", mail_type);

		json_data.Writer().Key(u8"title", sizeof(u8"title") - 1);
		json_data.AddArray(title);

		json_data.AddIntegral(u8"send_guid", send_guid);

		json_data.AddString(u8"send_name", send_name, sizeof(send_name));

		json_data.AddIntegral(u8"send_time", send_time);

		json_data.AddIntegral(u8"status", status);

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
        
        MailHeadData::Handler h(*this, nullptr);
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

struct MailBodyData{
	DataArray<unsigned char, unsigned short> content; //邮件正文
	DataArray<AttrData, unsigned short> attrs; //属性列表
	DataArray<ItemData, unsigned char> items; //物品列表
	DataArray<PetData, unsigned char> pets; //宠物列表
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(content);

		net_data.AddArray(attrs);

		net_data.AddArray(items);

		net_data.AddArray(pets);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(content))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if(!net_data.DelArray(items))
			return false;

		if(!net_data.DelArray(pets))
			return false;

		return true;
	}
	void Reset(void)
	{
		content.clear();

		attrs.clear();

		items.clear();

		pets.clear();

	}
	MailBodyData& operator= (const MailBodyData& src)
	{
		content = src.content;

		attrs = src.attrs;

		items = src.items;

		pets = src.pets;

		return *this;
	}
	bool operator==(const MailBodyData& src) const
	{
		if (content != src.content)
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (items != src.items)
		{
			return false;
		}

		if (pets != src.pets)
		{
			return false;
		}

		return true;
	}
	bool operator != (const MailBodyData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(MailBodyData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_attrs_handler(m_data.attrs, this),
            m_items_handler(m_data.items, this),
            m_pets_handler(m_data.pets, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_attrs_handler.ResetState();
            m_items_handler.ResetState();
            m_pets_handler.ResetState();
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.content, this);
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
            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_attrs_handler;
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_items_handler;
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_pets_handler;
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

            if (!m_items_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_pets_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        MailBodyData& m_data;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
        DataArrayHandler<ItemData, unsigned char, ItemData::Handler> m_items_handler;
        DataArrayHandler<PetData, unsigned char, PetData::Handler> m_pets_handler;
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

		json_data.Writer().Key(u8"content", sizeof(u8"content") - 1);
		json_data.AddArray(content);

		json_data.Writer().Key(u8"attrs", sizeof(u8"attrs") - 1);
		json_data.AddArray(attrs);

		json_data.Writer().Key(u8"items", sizeof(u8"items") - 1);
		json_data.AddArray(items);

		json_data.Writer().Key(u8"pets", sizeof(u8"pets") - 1);
		json_data.AddArray(pets);

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
        
        MailBodyData::Handler h(*this, nullptr);
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

struct MailData{
	MailHeadData              head; //邮件标题
	MailBodyData              body; //邮件内容
	bool EnCode(NetEnCode& net_data)
	{
		head.EnCode(net_data);

		body.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!head.DeCode(net_data))
			return false;

		if (!body.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		head.Reset();

		body.Reset();

	}
	MailData& operator= (const MailData& src)
	{
		head = src.head;

		body = src.body;

		return *this;
	}
	bool operator==(const MailData& src) const
	{
		if (head != src.head)
		{
			return false;
		}

		if (body != src.body)
		{
			return false;
		}

		return true;
	}
	bool operator != (const MailData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(MailData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_head_handler(m_data.head, this),
            m_body_handler(m_data.body, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_head_handler.ResetState();
            m_body_handler.ResetState();
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
                return &m_head_handler;
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_body_handler;
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

            if (!m_head_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_body_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        MailData& m_data;
        MailHeadData::Handler m_head_handler;
        MailBodyData::Handler m_body_handler;
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

		json_data.Writer().Key(u8"head", sizeof(u8"head") - 1);
		head.ToJson(json_data);

		json_data.Writer().Key(u8"body", sizeof(u8"body") - 1);
		body.ToJson(json_data);

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
        
        MailData::Handler h(*this, nullptr);
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

struct DBMailData{
	unsigned long long        recv_guid; //收件人GUID
	DataArray<unsigned long long, unsigned int> guids; //邮件关联GUID列表
	MailHeadData              head; //邮件标题
	MailBodyData              body; //邮件内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(recv_guid);

		net_data.AddArray(guids);

		head.EnCode(net_data);

		body.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(recv_guid))
			return false;

		if(!net_data.DelArray(guids))
			return false;

		if (!head.DeCode(net_data))
			return false;

		if (!body.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		recv_guid = 0;

		guids.clear();

		head.Reset();

		body.Reset();

	}
	DBMailData& operator= (const DBMailData& src)
	{
		recv_guid = src.recv_guid;

		guids = src.guids;

		head = src.head;

		body = src.body;

		return *this;
	}
	bool operator==(const DBMailData& src) const
	{
		if (recv_guid != src.recv_guid)
		{
			return false;
		}

		if (guids != src.guids)
		{
			return false;
		}

		if (head != src.head)
		{
			return false;
		}

		if (body != src.body)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DBMailData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DBMailData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_head_handler(m_data.head, this),
            m_body_handler(m_data.body, this),
            m_guids_handler(m_data.guids, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_head_handler.ResetState();
            m_body_handler.ResetState();
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
            switch (m_state)
            {
            case 2:
            {
                m_assigned_bitset.set(m_state);
                return &m_head_handler;
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                return &m_body_handler;
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
            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                return &m_guids_handler;
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

            if (!m_guids_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_head_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_body_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        DBMailData& m_data;
        MailHeadData::Handler m_head_handler;
        MailBodyData::Handler m_body_handler;
        DataArrayHandler<unsigned long long, unsigned int> m_guids_handler;
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

		json_data.AddIntegral(u8"recv_guid", recv_guid);

		json_data.Writer().Key(u8"guids", sizeof(u8"guids") - 1);
		json_data.AddArray(guids);

		json_data.Writer().Key(u8"head", sizeof(u8"head") - 1);
		head.ToJson(json_data);

		json_data.Writer().Key(u8"body", sizeof(u8"body") - 1);
		body.ToJson(json_data);

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
        
        DBMailData::Handler h(*this, nullptr);
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

struct DBVarData{
	DataArray<unsigned char, unsigned char> key; //key值
	DataArray<unsigned char, unsigned short> value; //value
	unsigned char             value_type; //值类型
	unsigned char             merge_type; //合区类型
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(key);

		net_data.AddArray(value);

		net_data.AddIntegral(value_type);

		net_data.AddIntegral(merge_type);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(key))
			return false;

		if(!net_data.DelArray(value))
			return false;

		if (!net_data.DelIntegral(value_type))
			return false;

		if (!net_data.DelIntegral(merge_type))
			return false;

		return true;
	}
	void Reset(void)
	{
		key.clear();

		value.clear();

		value_type = 0;

		merge_type = 0;

	}
	DBVarData& operator= (const DBVarData& src)
	{
		key = src.key;

		value = src.value;

		value_type = src.value_type;

		merge_type = src.merge_type;

		return *this;
	}
	bool operator==(const DBVarData& src) const
	{
		if (key != src.key)
		{
			return false;
		}

		if (value != src.value)
		{
			return false;
		}

		if (value_type != src.value_type)
		{
			return false;
		}

		if (merge_type != src.merge_type)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DBVarData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DBVarData& data, JsonHandler* parent = nullptr)
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.value_type, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.merge_type, str, length);
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
            case 1:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.value, this);
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
        DBVarData& m_data;
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

		json_data.Writer().Key(u8"key", sizeof(u8"key") - 1);
		json_data.AddArray(key);

		json_data.Writer().Key(u8"value", sizeof(u8"value") - 1);
		json_data.AddArray(value);

		json_data.AddIntegral(u8"value_type", value_type);

		json_data.AddIntegral(u8"merge_type", merge_type);

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
        
        DBVarData::Handler h(*this, nullptr);
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

struct RanklistData{
	char                      name[32]; //上榜名称
	char                      rank_info[256]; //排名信息
	signed long long          rank_data1; //排名数据1
	signed long long          rank_data2; //排名数据2
	signed long long          rank_data3; //排名数据3
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(name, sizeof(name));

		net_data.AddString(rank_info, sizeof(rank_info));

		net_data.AddIntegral(rank_data1);

		net_data.AddIntegral(rank_data2);

		net_data.AddIntegral(rank_data3);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelString(rank_info, sizeof(rank_info)))
			return false;

		if (!net_data.DelIntegral(rank_data1))
			return false;

		if (!net_data.DelIntegral(rank_data2))
			return false;

		if (!net_data.DelIntegral(rank_data3))
			return false;

		return true;
	}
	void Reset(void)
	{
		name[0] = 0;

		rank_info[0] = 0;

		rank_data1 = 0;

		rank_data2 = 0;

		rank_data3 = 0;

	}
	RanklistData& operator= (const RanklistData& src)
	{
		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.rank_info, sizeof(rank_info) - 1);
			memcpy(rank_info, src.rank_info, str_len + 1);
			rank_info[sizeof(rank_info) - 1] = 0;
		}

		rank_data1 = src.rank_data1;

		rank_data2 = src.rank_data2;

		rank_data3 = src.rank_data3;

		return *this;
	}
	bool operator==(const RanklistData& src) const
	{
		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (strncmp(rank_info, src.rank_info, sizeof(rank_info)-1))
		{
			return false;
		}

		if (rank_data1 != src.rank_data1)
		{
			return false;
		}

		if (rank_data2 != src.rank_data2)
		{
			return false;
		}

		if (rank_data3 != src.rank_data3)
		{
			return false;
		}

		return true;
	}
	bool operator != (const RanklistData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(RanklistData& data, JsonHandler* parent = nullptr)
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rank_data1, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rank_data2, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rank_data3, str, length);
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
                JsonDelString(m_data.name, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.rank_info, str, length);
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
        RanklistData& m_data;
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

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddString(u8"rank_info", rank_info, sizeof(rank_info));

		json_data.AddIntegral(u8"rank_data1", rank_data1);

		json_data.AddIntegral(u8"rank_data2", rank_data2);

		json_data.AddIntegral(u8"rank_data3", rank_data3);

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
        
        RanklistData::Handler h(*this, nullptr);
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

struct DBRanklistData{
	char                      name[32]; //上榜名称
	char                      rank_info[256]; //排名信息
	signed long long          rank_data1; //排名数据1
	signed long long          rank_data2; //排名数据2
	signed long long          rank_data3; //排名数据3
	signed long long          rank_guid; //排名guid
	signed long long          object_guid; //排名对象guid
	int                       rank_type; //排行榜类型
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(name, sizeof(name));

		net_data.AddString(rank_info, sizeof(rank_info));

		net_data.AddIntegral(rank_data1);

		net_data.AddIntegral(rank_data2);

		net_data.AddIntegral(rank_data3);

		net_data.AddIntegral(rank_guid);

		net_data.AddIntegral(object_guid);

		net_data.AddIntegral(rank_type);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if (!net_data.DelString(rank_info, sizeof(rank_info)))
			return false;

		if (!net_data.DelIntegral(rank_data1))
			return false;

		if (!net_data.DelIntegral(rank_data2))
			return false;

		if (!net_data.DelIntegral(rank_data3))
			return false;

		if (!net_data.DelIntegral(rank_guid))
			return false;

		if (!net_data.DelIntegral(object_guid))
			return false;

		if (!net_data.DelIntegral(rank_type))
			return false;

		return true;
	}
	void Reset(void)
	{
		name[0] = 0;

		rank_info[0] = 0;

		rank_data1 = 0;

		rank_data2 = 0;

		rank_data3 = 0;

		rank_guid = 0;

		object_guid = 0;

		rank_type = 0;

	}
	DBRanklistData& operator= (const DBRanklistData& src)
	{
		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.rank_info, sizeof(rank_info) - 1);
			memcpy(rank_info, src.rank_info, str_len + 1);
			rank_info[sizeof(rank_info) - 1] = 0;
		}

		rank_data1 = src.rank_data1;

		rank_data2 = src.rank_data2;

		rank_data3 = src.rank_data3;

		rank_guid = src.rank_guid;

		object_guid = src.object_guid;

		rank_type = src.rank_type;

		return *this;
	}
	bool operator==(const DBRanklistData& src) const
	{
		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (strncmp(rank_info, src.rank_info, sizeof(rank_info)-1))
		{
			return false;
		}

		if (rank_data1 != src.rank_data1)
		{
			return false;
		}

		if (rank_data2 != src.rank_data2)
		{
			return false;
		}

		if (rank_data3 != src.rank_data3)
		{
			return false;
		}

		if (rank_guid != src.rank_guid)
		{
			return false;
		}

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
	bool operator != (const DBRanklistData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DBRanklistData& data, JsonHandler* parent = nullptr)
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
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rank_data1, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rank_data2, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rank_data3, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.rank_guid, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.object_guid, str, length);
            }
            break;
            case 7:
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
            (void)copy;

            switch (m_state)
            {
            case 0:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.name, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.rank_info, str, length);
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
        DBRanklistData& m_data;
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

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.AddString(u8"rank_info", rank_info, sizeof(rank_info));

		json_data.AddIntegral(u8"rank_data1", rank_data1);

		json_data.AddIntegral(u8"rank_data2", rank_data2);

		json_data.AddIntegral(u8"rank_data3", rank_data3);

		json_data.AddIntegral(u8"rank_guid", rank_guid);

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
        
        DBRanklistData::Handler h(*this, nullptr);
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

struct TeamBriefData{
	unsigned long long        team_guid; //队伍GUID
	unsigned short            target; //目标ID
	unsigned short            min_require_level; //最低要求等级
	unsigned short            max_require_level; //最高要求等级
	unsigned short            member_count; //成员数量
	MemberData                leader_data; //队长数据
	unsigned char             has_applicant; //是否存在申请
	unsigned short            min_require_reinc; //最低要求转生次数
	unsigned short            max_require_reinc; //最高要求转生次数
	CustomData                customs; //自定义
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(team_guid);

		net_data.AddIntegral(target);

		net_data.AddIntegral(min_require_level);

		net_data.AddIntegral(max_require_level);

		net_data.AddIntegral(member_count);

		leader_data.EnCode(net_data);

		net_data.AddIntegral(has_applicant);

		net_data.AddIntegral(min_require_reinc);

		net_data.AddIntegral(max_require_reinc);

		customs.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(team_guid))
			return false;

		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(min_require_level))
			return false;

		if (!net_data.DelIntegral(max_require_level))
			return false;

		if (!net_data.DelIntegral(member_count))
			return false;

		if (!leader_data.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(has_applicant))
			return false;

		if (!net_data.DelIntegral(min_require_reinc))
			return false;

		if (!net_data.DelIntegral(max_require_reinc))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		team_guid = 0;

		target = 0;

		min_require_level = 0;

		max_require_level = 0;

		member_count = 0;

		leader_data.Reset();

		has_applicant = 0;

		min_require_reinc = 0;

		max_require_reinc = 0;

		customs.Reset();

	}
	TeamBriefData& operator= (const TeamBriefData& src)
	{
		team_guid = src.team_guid;

		target = src.target;

		min_require_level = src.min_require_level;

		max_require_level = src.max_require_level;

		member_count = src.member_count;

		leader_data = src.leader_data;

		has_applicant = src.has_applicant;

		min_require_reinc = src.min_require_reinc;

		max_require_reinc = src.max_require_reinc;

		customs = src.customs;

		return *this;
	}
	bool operator==(const TeamBriefData& src) const
	{
		if (team_guid != src.team_guid)
		{
			return false;
		}

		if (target != src.target)
		{
			return false;
		}

		if (min_require_level != src.min_require_level)
		{
			return false;
		}

		if (max_require_level != src.max_require_level)
		{
			return false;
		}

		if (member_count != src.member_count)
		{
			return false;
		}

		if (leader_data != src.leader_data)
		{
			return false;
		}

		if (has_applicant != src.has_applicant)
		{
			return false;
		}

		if (min_require_reinc != src.min_require_reinc)
		{
			return false;
		}

		if (max_require_reinc != src.max_require_reinc)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TeamBriefData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TeamBriefData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_leader_data_handler(m_data.leader_data, this),
            m_customs_handler(m_data.customs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_leader_data_handler.ResetState();
            m_customs_handler.ResetState();
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
                JsonDelIntegral(m_data.team_guid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.min_require_level, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.max_require_level, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.member_count, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.has_applicant, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.min_require_reinc, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.max_require_reinc, str, length);
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
            case 5:
            {
                m_assigned_bitset.set(m_state);
                return &m_leader_data_handler;
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
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

            if (!m_leader_data_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        TeamBriefData& m_data;
        MemberData::Handler m_leader_data_handler;
        CustomData::Handler m_customs_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<10> m_assigned_bitset;

        static const std::bitset<10> template_assigned_bitset;
        static const char* m_key_list[10];
        static const char* m_alias_list[10];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"team_guid", team_guid);

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"min_require_level", min_require_level);

		json_data.AddIntegral(u8"max_require_level", max_require_level);

		json_data.AddIntegral(u8"member_count", member_count);

		json_data.Writer().Key(u8"leader_data", sizeof(u8"leader_data") - 1);
		leader_data.ToJson(json_data);

		json_data.AddIntegral(u8"has_applicant", has_applicant);

		json_data.AddIntegral(u8"min_require_reinc", min_require_reinc);

		json_data.AddIntegral(u8"max_require_reinc", max_require_reinc);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

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
        
        TeamBriefData::Handler h(*this, nullptr);
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

struct TeamApplicantData{
	unsigned long long        guid; //玩家GUID
	char                      name[32]; //玩家名字
	DataArray<AttrData, unsigned short> attrs; //属性数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddString(name, sizeof(name));

		net_data.AddArray(attrs);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		name[0] = 0;

		attrs.clear();

	}
	TeamApplicantData& operator= (const TeamApplicantData& src)
	{
		guid = src.guid;

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		attrs = src.attrs;

		return *this;
	}
	bool operator==(const TeamApplicantData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TeamApplicantData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TeamApplicantData& data, JsonHandler* parent = nullptr)
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
            switch (m_state)
            {
            case 2:
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
        TeamApplicantData& m_data;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
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

		json_data.AddString(u8"name", name, sizeof(name));

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
        
        TeamApplicantData::Handler h(*this, nullptr);
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

struct ExchangeData{
	unsigned long long        day; //交易日期
	DataArray<KeyValueU32, unsigned short> records; //交易记录
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(day);

		net_data.AddArray(records);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(day))
			return false;

		if(!net_data.DelArray(records))
			return false;

		return true;
	}
	void Reset(void)
	{
		day = 0;

		records.clear();

	}
	ExchangeData& operator= (const ExchangeData& src)
	{
		day = src.day;

		records = src.records;

		return *this;
	}
	bool operator==(const ExchangeData& src) const
	{
		if (day != src.day)
		{
			return false;
		}

		if (records != src.records)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ExchangeData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ExchangeData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_records_handler(m_data.records, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_records_handler.ResetState();
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
                JsonDelIntegral(m_data.day, str, length);
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
                return &m_records_handler;
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

            if (!m_records_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ExchangeData& m_data;
        DataArrayHandler<KeyValueU32, unsigned short, KeyValueU32::Handler> m_records_handler;
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

		json_data.AddIntegral(u8"day", day);

		json_data.Writer().Key(u8"records", sizeof(u8"records") - 1);
		json_data.AddArray(records);

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
        
        ExchangeData::Handler h(*this, nullptr);
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

struct ForbidData{
	unsigned char             sid; //sid
	DataArray<unsigned char, unsigned char> keyword; //禁止关键字
	unsigned char             keytype; //1:角色名 2:帐号名 3:IP 4:硬件码
	unsigned char             fbdtype; //1:禁止登陆
	signed long long          begin; //开始时间
	signed long long          end; //结束时间
	DataArray<unsigned char, unsigned char> desc; //描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(sid);

		net_data.AddArray(keyword);

		net_data.AddIntegral(keytype);

		net_data.AddIntegral(fbdtype);

		net_data.AddIntegral(begin);

		net_data.AddIntegral(end);

		net_data.AddArray(desc);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(sid))
			return false;

		if(!net_data.DelArray(keyword))
			return false;

		if (!net_data.DelIntegral(keytype))
			return false;

		if (!net_data.DelIntegral(fbdtype))
			return false;

		if (!net_data.DelIntegral(begin))
			return false;

		if (!net_data.DelIntegral(end))
			return false;

		if(!net_data.DelArray(desc))
			return false;

		return true;
	}
	void Reset(void)
	{
		sid = 0;

		keyword.clear();

		keytype = 0;

		fbdtype = 0;

		begin = 0;

		end = 0;

		desc.clear();

	}
	ForbidData& operator= (const ForbidData& src)
	{
		sid = src.sid;

		keyword = src.keyword;

		keytype = src.keytype;

		fbdtype = src.fbdtype;

		begin = src.begin;

		end = src.end;

		desc = src.desc;

		return *this;
	}
	bool operator==(const ForbidData& src) const
	{
		if (sid != src.sid)
		{
			return false;
		}

		if (keyword != src.keyword)
		{
			return false;
		}

		if (keytype != src.keytype)
		{
			return false;
		}

		if (fbdtype != src.fbdtype)
		{
			return false;
		}

		if (begin != src.begin)
		{
			return false;
		}

		if (end != src.end)
		{
			return false;
		}

		if (desc != src.desc)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ForbidData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ForbidData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.keytype, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.fbdtype, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.begin, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.end, str, length);
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
                DataArrayHandler<unsigned char, unsigned char> handler(m_data.keyword, this);
                handler.String(str, length, copy);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned char> handler(m_data.desc, this);
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
        ForbidData& m_data;
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

		json_data.AddIntegral(u8"sid", sid);

		json_data.Writer().Key(u8"keyword", sizeof(u8"keyword") - 1);
		json_data.AddArray(keyword);

		json_data.AddIntegral(u8"keytype", keytype);

		json_data.AddIntegral(u8"fbdtype", fbdtype);

		json_data.AddIntegral(u8"begin", begin);

		json_data.AddIntegral(u8"end", end);

		json_data.Writer().Key(u8"desc", sizeof(u8"desc") - 1);
		json_data.AddArray(desc);

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
        
        ForbidData::Handler h(*this, nullptr);
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

struct ForbidTalkData{
	unsigned long long        guid; //角色guid
	signed long long          begin; //开始时间
	signed long long          end; //结束时间
	DataArray<unsigned char, unsigned char> desc; //描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(guid);

		net_data.AddIntegral(begin);

		net_data.AddIntegral(end);

		net_data.AddArray(desc);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(guid))
			return false;

		if (!net_data.DelIntegral(begin))
			return false;

		if (!net_data.DelIntegral(end))
			return false;

		if(!net_data.DelArray(desc))
			return false;

		return true;
	}
	void Reset(void)
	{
		guid = 0;

		begin = 0;

		end = 0;

		desc.clear();

	}
	ForbidTalkData& operator= (const ForbidTalkData& src)
	{
		guid = src.guid;

		begin = src.begin;

		end = src.end;

		desc = src.desc;

		return *this;
	}
	bool operator==(const ForbidTalkData& src) const
	{
		if (guid != src.guid)
		{
			return false;
		}

		if (begin != src.begin)
		{
			return false;
		}

		if (end != src.end)
		{
			return false;
		}

		if (desc != src.desc)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ForbidTalkData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ForbidTalkData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.begin, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.end, str, length);
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
                DataArrayHandler<unsigned char, unsigned char> handler(m_data.desc, this);
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
        ForbidTalkData& m_data;
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

		json_data.AddIntegral(u8"begin", begin);

		json_data.AddIntegral(u8"end", end);

		json_data.Writer().Key(u8"desc", sizeof(u8"desc") - 1);
		json_data.AddArray(desc);

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
        
        ForbidTalkData::Handler h(*this, nullptr);
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

struct PasturePetData{
	unsigned long long        owner_guid; //拥有者GUID
	PetData                   pet; //子区ID
	unsigned short            map; //地图
	unsigned short            x; //x坐标
	unsigned short            y; //y坐标
	unsigned int              placed_time; //放置时间
	unsigned int              placed_status; //放置状态
	unsigned int              replaced_time; //替换时间
	unsigned long long        replaced_player_guid; //替换的玩家GUID
	char                      replaced_player_name[32]; //替换的玩家名字
	unsigned long long        replaced_pet_guid; //替换的宠物GUID
	char                      replaced_pet_name[32]; //替换的宠物名字
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(owner_guid);

		pet.EnCode(net_data);

		net_data.AddIntegral(map);

		net_data.AddIntegral(x);

		net_data.AddIntegral(y);

		net_data.AddIntegral(placed_time);

		net_data.AddIntegral(placed_status);

		net_data.AddIntegral(replaced_time);

		net_data.AddIntegral(replaced_player_guid);

		net_data.AddString(replaced_player_name, sizeof(replaced_player_name));

		net_data.AddIntegral(replaced_pet_guid);

		net_data.AddString(replaced_pet_name, sizeof(replaced_pet_name));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(owner_guid))
			return false;

		if (!pet.DeCode(net_data))
			return false;

		if (!net_data.DelIntegral(map))
			return false;

		if (!net_data.DelIntegral(x))
			return false;

		if (!net_data.DelIntegral(y))
			return false;

		if (!net_data.DelIntegral(placed_time))
			return false;

		if (!net_data.DelIntegral(placed_status))
			return false;

		if (!net_data.DelIntegral(replaced_time))
			return false;

		if (!net_data.DelIntegral(replaced_player_guid))
			return false;

		if (!net_data.DelString(replaced_player_name, sizeof(replaced_player_name)))
			return false;

		if (!net_data.DelIntegral(replaced_pet_guid))
			return false;

		if (!net_data.DelString(replaced_pet_name, sizeof(replaced_pet_name)))
			return false;

		return true;
	}
	void Reset(void)
	{
		owner_guid = 0;

		pet.Reset();

		map = 0;

		x = 0;

		y = 0;

		placed_time = 0;

		placed_status = 0;

		replaced_time = 0;

		replaced_player_guid = 0;

		replaced_player_name[0] = 0;

		replaced_pet_guid = 0;

		replaced_pet_name[0] = 0;

	}
	PasturePetData& operator= (const PasturePetData& src)
	{
		owner_guid = src.owner_guid;

		pet = src.pet;

		map = src.map;

		x = src.x;

		y = src.y;

		placed_time = src.placed_time;

		placed_status = src.placed_status;

		replaced_time = src.replaced_time;

		replaced_player_guid = src.replaced_player_guid;

		{
			size_t str_len = strnlen(src.replaced_player_name, sizeof(replaced_player_name) - 1);
			memcpy(replaced_player_name, src.replaced_player_name, str_len + 1);
			replaced_player_name[sizeof(replaced_player_name) - 1] = 0;
		}

		replaced_pet_guid = src.replaced_pet_guid;

		{
			size_t str_len = strnlen(src.replaced_pet_name, sizeof(replaced_pet_name) - 1);
			memcpy(replaced_pet_name, src.replaced_pet_name, str_len + 1);
			replaced_pet_name[sizeof(replaced_pet_name) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const PasturePetData& src) const
	{
		if (owner_guid != src.owner_guid)
		{
			return false;
		}

		if (pet != src.pet)
		{
			return false;
		}

		if (map != src.map)
		{
			return false;
		}

		if (x != src.x)
		{
			return false;
		}

		if (y != src.y)
		{
			return false;
		}

		if (placed_time != src.placed_time)
		{
			return false;
		}

		if (placed_status != src.placed_status)
		{
			return false;
		}

		if (replaced_time != src.replaced_time)
		{
			return false;
		}

		if (replaced_player_guid != src.replaced_player_guid)
		{
			return false;
		}

		if (strncmp(replaced_player_name, src.replaced_player_name, sizeof(replaced_player_name)-1))
		{
			return false;
		}

		if (replaced_pet_guid != src.replaced_pet_guid)
		{
			return false;
		}

		if (strncmp(replaced_pet_name, src.replaced_pet_name, sizeof(replaced_pet_name)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const PasturePetData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PasturePetData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_pet_handler(m_data.pet, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_pet_handler.ResetState();
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
                JsonDelIntegral(m_data.owner_guid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.map, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.x, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.y, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.placed_time, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.placed_status, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.replaced_time, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.replaced_player_guid, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.replaced_pet_guid, str, length);
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
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.replaced_player_name, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.replaced_pet_name, str, length);
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
                return &m_pet_handler;
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

            if (!m_pet_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        PasturePetData& m_data;
        PetData::Handler m_pet_handler;
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

		json_data.AddIntegral(u8"owner_guid", owner_guid);

		json_data.Writer().Key(u8"pet", sizeof(u8"pet") - 1);
		pet.ToJson(json_data);

		json_data.AddIntegral(u8"map", map);

		json_data.AddIntegral(u8"x", x);

		json_data.AddIntegral(u8"y", y);

		json_data.AddIntegral(u8"placed_time", placed_time);

		json_data.AddIntegral(u8"placed_status", placed_status);

		json_data.AddIntegral(u8"replaced_time", replaced_time);

		json_data.AddIntegral(u8"replaced_player_guid", replaced_player_guid);

		json_data.AddString(u8"replaced_player_name", replaced_player_name, sizeof(replaced_player_name));

		json_data.AddIntegral(u8"replaced_pet_guid", replaced_pet_guid);

		json_data.AddString(u8"replaced_pet_name", replaced_pet_name, sizeof(replaced_pet_name));

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
        
        PasturePetData::Handler h(*this, nullptr);
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

struct LadderData{
	unsigned long long        player_guid; //玩家GUID
	unsigned int              rank; //排行
	PlayerData                player; //玩家数据
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(player_guid);

		net_data.AddIntegral(rank);

		player.EnCode(net_data);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelIntegral(rank))
			return false;

		if (!player.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		player_guid = 0;

		rank = 0;

		player.Reset();

	}
	LadderData& operator= (const LadderData& src)
	{
		player_guid = src.player_guid;

		rank = src.rank;

		player = src.player;

		return *this;
	}
	bool operator==(const LadderData& src) const
	{
		if (player_guid != src.player_guid)
		{
			return false;
		}

		if (rank != src.rank)
		{
			return false;
		}

		if (player != src.player)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LadderData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LadderData& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_player_handler(m_data.player, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_player_handler.ResetState();
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
                JsonDelIntegral(m_data.rank, str, length);
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
                return &m_player_handler;
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

            if (!m_player_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        LadderData& m_data;
        PlayerData::Handler m_player_handler;
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

		json_data.AddIntegral(u8"player_guid", player_guid);

		json_data.AddIntegral(u8"rank", rank);

		json_data.Writer().Key(u8"player", sizeof(u8"player") - 1);
		player.ToJson(json_data);

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
        
        LadderData::Handler h(*this, nullptr);
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

struct LadderRecordData{
	unsigned char             action; //动作
	unsigned int              time; //时间
	unsigned long long        player_guid; //玩家GUID
	char                      player_name[32]; //玩家名字
	unsigned int              player_rank; //排行
	unsigned long long        target_guid; //玩家GUID
	char                      target_name[32]; //玩家名字
	unsigned int              target_rank; //排行
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(action);

		net_data.AddIntegral(time);

		net_data.AddIntegral(player_guid);

		net_data.AddString(player_name, sizeof(player_name));

		net_data.AddIntegral(player_rank);

		net_data.AddIntegral(target_guid);

		net_data.AddString(target_name, sizeof(target_name));

		net_data.AddIntegral(target_rank);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(action))
			return false;

		if (!net_data.DelIntegral(time))
			return false;

		if (!net_data.DelIntegral(player_guid))
			return false;

		if (!net_data.DelString(player_name, sizeof(player_name)))
			return false;

		if (!net_data.DelIntegral(player_rank))
			return false;

		if (!net_data.DelIntegral(target_guid))
			return false;

		if (!net_data.DelString(target_name, sizeof(target_name)))
			return false;

		if (!net_data.DelIntegral(target_rank))
			return false;

		return true;
	}
	void Reset(void)
	{
		action = 0;

		time = 0;

		player_guid = 0;

		player_name[0] = 0;

		player_rank = 0;

		target_guid = 0;

		target_name[0] = 0;

		target_rank = 0;

	}
	LadderRecordData& operator= (const LadderRecordData& src)
	{
		action = src.action;

		time = src.time;

		player_guid = src.player_guid;

		{
			size_t str_len = strnlen(src.player_name, sizeof(player_name) - 1);
			memcpy(player_name, src.player_name, str_len + 1);
			player_name[sizeof(player_name) - 1] = 0;
		}

		player_rank = src.player_rank;

		target_guid = src.target_guid;

		{
			size_t str_len = strnlen(src.target_name, sizeof(target_name) - 1);
			memcpy(target_name, src.target_name, str_len + 1);
			target_name[sizeof(target_name) - 1] = 0;
		}

		target_rank = src.target_rank;

		return *this;
	}
	bool operator==(const LadderRecordData& src) const
	{
		if (action != src.action)
		{
			return false;
		}

		if (time != src.time)
		{
			return false;
		}

		if (player_guid != src.player_guid)
		{
			return false;
		}

		if (strncmp(player_name, src.player_name, sizeof(player_name)-1))
		{
			return false;
		}

		if (player_rank != src.player_rank)
		{
			return false;
		}

		if (target_guid != src.target_guid)
		{
			return false;
		}

		if (strncmp(target_name, src.target_name, sizeof(target_name)-1))
		{
			return false;
		}

		if (target_rank != src.target_rank)
		{
			return false;
		}

		return true;
	}
	bool operator != (const LadderRecordData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(LadderRecordData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.action, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.time, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.player_guid, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.player_rank, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target_guid, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target_rank, str, length);
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
                JsonDelString(m_data.player_name, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.target_name, str, length);
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
        LadderRecordData& m_data;
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

		json_data.AddIntegral(u8"action", action);

		json_data.AddIntegral(u8"time", time);

		json_data.AddIntegral(u8"player_guid", player_guid);

		json_data.AddString(u8"player_name", player_name, sizeof(player_name));

		json_data.AddIntegral(u8"player_rank", player_rank);

		json_data.AddIntegral(u8"target_guid", target_guid);

		json_data.AddString(u8"target_name", target_name, sizeof(target_name));

		json_data.AddIntegral(u8"target_rank", target_rank);

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
        
        LadderRecordData::Handler h(*this, nullptr);
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

struct PlayerDetail{
	unsigned int              uid; //账号ID
	unsigned char             sid; //子区ID
	unsigned long long        guid; //玩家GUID
	char                      unid[32]; //玩家UNID
	char                      name[32]; //玩家名字
	DataArray<AttrData, unsigned short> attrs; //属性数据
	unsigned short            map; //地图ID
	unsigned short            x; //x坐标
	unsigned short            y; //y坐标
	unsigned char             state; //玩家状态 0:彻底销毁 1:使用中 2:已删除
	unsigned int              createtime; //创建时间
	unsigned int              lastlogin; //最后一次登陆时间
	unsigned int              destroytime; //销毁时间 state为0=销毁时间 state为2=删除时间
	CustomData                customs; //自定义
	ItemContainerData         equips; //装备
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(uid);

		net_data.AddIntegral(sid);

		net_data.AddIntegral(guid);

		net_data.AddString(unid, sizeof(unid));

		net_data.AddString(name, sizeof(name));

		net_data.AddArray(attrs);

		net_data.AddIntegral(map);

		net_data.AddIntegral(x);

		net_data.AddIntegral(y);

		net_data.AddIntegral(state);

		net_data.AddIntegral(createtime);

		net_data.AddIntegral(lastlogin);

		net_data.AddIntegral(destroytime);

		customs.EnCode(net_data);

		equips.EnCode(net_data);

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

		if (!net_data.DelString(unid, sizeof(unid)))
			return false;

		if (!net_data.DelString(name, sizeof(name)))
			return false;

		if(!net_data.DelArray(attrs))
			return false;

		if (!net_data.DelIntegral(map))
			return false;

		if (!net_data.DelIntegral(x))
			return false;

		if (!net_data.DelIntegral(y))
			return false;

		if (!net_data.DelIntegral(state))
			return false;

		if (!net_data.DelIntegral(createtime))
			return false;

		if (!net_data.DelIntegral(lastlogin))
			return false;

		if (!net_data.DelIntegral(destroytime))
			return false;

		if (!customs.DeCode(net_data))
			return false;

		if (!equips.DeCode(net_data))
			return false;

		return true;
	}
	void Reset(void)
	{
		uid = 0;

		sid = 0;

		guid = 0;

		unid[0] = 0;

		name[0] = 0;

		attrs.clear();

		map = 0;

		x = 0;

		y = 0;

		state = 0;

		createtime = 0;

		lastlogin = 0;

		destroytime = 0;

		customs.Reset();

		equips.Reset();

	}
	PlayerDetail& operator= (const PlayerDetail& src)
	{
		uid = src.uid;

		sid = src.sid;

		guid = src.guid;

		{
			size_t str_len = strnlen(src.unid, sizeof(unid) - 1);
			memcpy(unid, src.unid, str_len + 1);
			unid[sizeof(unid) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.name, sizeof(name) - 1);
			memcpy(name, src.name, str_len + 1);
			name[sizeof(name) - 1] = 0;
		}

		attrs = src.attrs;

		map = src.map;

		x = src.x;

		y = src.y;

		state = src.state;

		createtime = src.createtime;

		lastlogin = src.lastlogin;

		destroytime = src.destroytime;

		customs = src.customs;

		equips = src.equips;

		return *this;
	}
	bool operator==(const PlayerDetail& src) const
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

		if (strncmp(unid, src.unid, sizeof(unid)-1))
		{
			return false;
		}

		if (strncmp(name, src.name, sizeof(name)-1))
		{
			return false;
		}

		if (attrs != src.attrs)
		{
			return false;
		}

		if (map != src.map)
		{
			return false;
		}

		if (x != src.x)
		{
			return false;
		}

		if (y != src.y)
		{
			return false;
		}

		if (state != src.state)
		{
			return false;
		}

		if (createtime != src.createtime)
		{
			return false;
		}

		if (lastlogin != src.lastlogin)
		{
			return false;
		}

		if (destroytime != src.destroytime)
		{
			return false;
		}

		if (customs != src.customs)
		{
			return false;
		}

		if (equips != src.equips)
		{
			return false;
		}

		return true;
	}
	bool operator != (const PlayerDetail& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PlayerDetail& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_customs_handler(m_data.customs, this),
            m_equips_handler(m_data.equips, this),
            m_attrs_handler(m_data.attrs, this),
            m_default_handler(this),
            m_state(0) { (void)m_data; }

        ~Handler(){}

        void ResetState(void)
        {
            m_state = 0;
            m_assigned_bitset.reset();
            m_customs_handler.ResetState();
            m_equips_handler.ResetState();
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
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.map, str, length);
            }
            break;
            case 7:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.x, str, length);
            }
            break;
            case 8:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.y, str, length);
            }
            break;
            case 9:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.state, str, length);
            }
            break;
            case 10:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.createtime, str, length);
            }
            break;
            case 11:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.lastlogin, str, length);
            }
            break;
            case 12:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.destroytime, str, length);
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
                JsonDelString(m_data.unid, str, length);
            }
            break;
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
            switch (m_state)
            {
            case 13:
            {
                m_assigned_bitset.set(m_state);
                return &m_customs_handler;
            }
            break;
            case 14:
            {
                m_assigned_bitset.set(m_state);
                return &m_equips_handler;
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
            switch (m_state)
            {
            case 5:
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

            if (!m_customs_handler.IsAllMemberSet())
            {
                return false;
            }

            if (!m_equips_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        PlayerDetail& m_data;
        CustomData::Handler m_customs_handler;
        ItemContainerData::Handler m_equips_handler;
        DataArrayHandler<AttrData, unsigned short, AttrData::Handler> m_attrs_handler;
        HoleJsonHandler m_default_handler;
        unsigned int m_state;
        std::bitset<15> m_assigned_bitset;

        static const std::bitset<15> template_assigned_bitset;
        static const char* m_key_list[15];
        static const char* m_alias_list[15];
    };

	bool ToJson(JsonEnCode& json_data) const
	{
		json_data.Writer().StartObject();

		json_data.AddIntegral(u8"uid", uid);

		json_data.AddIntegral(u8"sid", sid);

		json_data.AddIntegral(u8"guid", guid);

		json_data.AddString(u8"unid", unid, sizeof(unid));

		json_data.AddString(u8"name", name, sizeof(name));

		json_data.Writer().Key(u8"attrs", sizeof(u8"attrs") - 1);
		json_data.AddArray(attrs);

		json_data.AddIntegral(u8"map", map);

		json_data.AddIntegral(u8"x", x);

		json_data.AddIntegral(u8"y", y);

		json_data.AddIntegral(u8"state", state);

		json_data.AddIntegral(u8"createtime", createtime);

		json_data.AddIntegral(u8"lastlogin", lastlogin);

		json_data.AddIntegral(u8"destroytime", destroytime);

		json_data.Writer().Key(u8"customs", sizeof(u8"customs") - 1);
		customs.ToJson(json_data);

		json_data.Writer().Key(u8"equips", sizeof(u8"equips") - 1);
		equips.ToJson(json_data);

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
        
        PlayerDetail::Handler h(*this, nullptr);
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

struct Pt{
	unsigned short            x; //x坐标
	unsigned short            y; //y坐标
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(x);

		net_data.AddIntegral(y);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(x))
			return false;

		if (!net_data.DelIntegral(y))
			return false;

		return true;
	}
	void Reset(void)
	{
		x = 0;

		y = 0;

	}
	Pt& operator= (const Pt& src)
	{
		x = src.x;

		y = src.y;

		return *this;
	}
	bool operator==(const Pt& src) const
	{
		if (x != src.x)
		{
			return false;
		}

		if (y != src.y)
		{
			return false;
		}

		return true;
	}
	bool operator != (const Pt& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(Pt& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.x, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.y, str, length);
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
        Pt& m_data;
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

		json_data.AddIntegral(u8"x", x);

		json_data.AddIntegral(u8"y", y);

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
        
        Pt::Handler h(*this, nullptr);
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

struct ViolateData{
	unsigned char             type; //违禁字类型
	DataArray<unsigned char, unsigned char> violate; //违禁字
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddArray(violate);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if(!net_data.DelArray(violate))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		violate.clear();

	}
	ViolateData& operator= (const ViolateData& src)
	{
		type = src.type;

		violate = src.violate;

		return *this;
	}
	bool operator==(const ViolateData& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (violate != src.violate)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ViolateData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ViolateData& data, JsonHandler* parent = nullptr)
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
            (void)copy;

            switch (m_state)
            {
            case 1:
            {
                m_assigned_bitset.set(m_state);
                DataArrayHandler<unsigned char, unsigned char> handler(m_data.violate, this);
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
        ViolateData& m_data;
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

		json_data.AddIntegral(u8"type", type);

		json_data.Writer().Key(u8"violate", sizeof(u8"violate") - 1);
		json_data.AddArray(violate);

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
        
        ViolateData::Handler h(*this, nullptr);
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

struct ChannelSwitchData{
	unsigned char             type; //频道类型
	unsigned char             sw; //开关
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(sw);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(sw))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		sw = 0;

	}
	ChannelSwitchData& operator= (const ChannelSwitchData& src)
	{
		type = src.type;

		sw = src.sw;

		return *this;
	}
	bool operator==(const ChannelSwitchData& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (sw != src.sw)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ChannelSwitchData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ChannelSwitchData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.sw, str, length);
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
        ChannelSwitchData& m_data;
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

		json_data.AddIntegral(u8"type", type);

		json_data.AddIntegral(u8"sw", sw);

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
        
        ChannelSwitchData::Handler h(*this, nullptr);
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

struct VerifyData{
	unsigned long long        fbd_time; //封禁时间
	int                       accountstatus; //账号状态 0=正常 1=禁用 2=临时密码使用中 3=异常 4=待销户 5=已销户
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(fbd_time);

		net_data.AddIntegral(accountstatus);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(fbd_time))
			return false;

		if (!net_data.DelIntegral(accountstatus))
			return false;

		return true;
	}
	void Reset(void)
	{
		fbd_time = 0;

		accountstatus = 0;

	}
	VerifyData& operator= (const VerifyData& src)
	{
		fbd_time = src.fbd_time;

		accountstatus = src.accountstatus;

		return *this;
	}
	bool operator==(const VerifyData& src) const
	{
		if (fbd_time != src.fbd_time)
		{
			return false;
		}

		if (accountstatus != src.accountstatus)
		{
			return false;
		}

		return true;
	}
	bool operator != (const VerifyData& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(VerifyData& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.fbd_time, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.accountstatus, str, length);
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
            if (((template_assigned_bitset & m_assigned_bitset) ^ template_assigned_bitset).any())
            {
                return false;
            }

            return true;
        }
    private:
        VerifyData& m_data;
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

		json_data.AddIntegral(u8"fbd_time", fbd_time);

		json_data.AddIntegral(u8"accountstatus", accountstatus);

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
        
        VerifyData::Handler h(*this, nullptr);
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

struct IPPort{
	char                      ip[32]; //ip
	unsigned short            port; //port
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddString(ip, sizeof(ip));

		net_data.AddIntegral(port);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelString(ip, sizeof(ip)))
			return false;

		if (!net_data.DelIntegral(port))
			return false;

		return true;
	}
	void Reset(void)
	{
		ip[0] = 0;

		port = 0;

	}
	IPPort& operator= (const IPPort& src)
	{
		{
			size_t str_len = strnlen(src.ip, sizeof(ip) - 1);
			memcpy(ip, src.ip, str_len + 1);
			ip[sizeof(ip) - 1] = 0;
		}

		port = src.port;

		return *this;
	}
	bool operator==(const IPPort& src) const
	{
		if (strncmp(ip, src.ip, sizeof(ip)-1))
		{
			return false;
		}

		if (port != src.port)
		{
			return false;
		}

		return true;
	}
	bool operator != (const IPPort& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(IPPort& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.port, str, length);
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
                JsonDelString(m_data.ip, str, length);
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
        IPPort& m_data;
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

		json_data.AddString(u8"ip", ip, sizeof(ip));

		json_data.AddIntegral(u8"port", port);

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
        
        IPPort::Handler h(*this, nullptr);
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

struct CrossIPPort{
	unsigned int              gid; //区id
	unsigned short            gcp;
	unsigned short            gsp;
	char                      innerip[32]; //内网ip
	char                      extip[32]; //外网ip
	char                      innerflag[64]; //内网标记
	char                      clientname[64]; //区显示名称
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(gid);

		net_data.AddIntegral(gcp);

		net_data.AddIntegral(gsp);

		net_data.AddString(innerip, sizeof(innerip));

		net_data.AddString(extip, sizeof(extip));

		net_data.AddString(innerflag, sizeof(innerflag));

		net_data.AddString(clientname, sizeof(clientname));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(gid))
			return false;

		if (!net_data.DelIntegral(gcp))
			return false;

		if (!net_data.DelIntegral(gsp))
			return false;

		if (!net_data.DelString(innerip, sizeof(innerip)))
			return false;

		if (!net_data.DelString(extip, sizeof(extip)))
			return false;

		if (!net_data.DelString(innerflag, sizeof(innerflag)))
			return false;

		if (!net_data.DelString(clientname, sizeof(clientname)))
			return false;

		return true;
	}
	void Reset(void)
	{
		gid = 0;

		gcp = 0;

		gsp = 0;

		innerip[0] = 0;

		extip[0] = 0;

		innerflag[0] = 0;

		clientname[0] = 0;

	}
	CrossIPPort& operator= (const CrossIPPort& src)
	{
		gid = src.gid;

		gcp = src.gcp;

		gsp = src.gsp;

		{
			size_t str_len = strnlen(src.innerip, sizeof(innerip) - 1);
			memcpy(innerip, src.innerip, str_len + 1);
			innerip[sizeof(innerip) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.extip, sizeof(extip) - 1);
			memcpy(extip, src.extip, str_len + 1);
			extip[sizeof(extip) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.innerflag, sizeof(innerflag) - 1);
			memcpy(innerflag, src.innerflag, str_len + 1);
			innerflag[sizeof(innerflag) - 1] = 0;
		}

		{
			size_t str_len = strnlen(src.clientname, sizeof(clientname) - 1);
			memcpy(clientname, src.clientname, str_len + 1);
			clientname[sizeof(clientname) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const CrossIPPort& src) const
	{
		if (gid != src.gid)
		{
			return false;
		}

		if (gcp != src.gcp)
		{
			return false;
		}

		if (gsp != src.gsp)
		{
			return false;
		}

		if (strncmp(innerip, src.innerip, sizeof(innerip)-1))
		{
			return false;
		}

		if (strncmp(extip, src.extip, sizeof(extip)-1))
		{
			return false;
		}

		if (strncmp(innerflag, src.innerflag, sizeof(innerflag)-1))
		{
			return false;
		}

		if (strncmp(clientname, src.clientname, sizeof(clientname)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const CrossIPPort& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CrossIPPort& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.gid, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gcp, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.gsp, str, length);
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
                JsonDelString(m_data.innerip, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.extip, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.innerflag, str, length);
            }
            break;
            case 6:
            {
                m_assigned_bitset.set(m_state);
                JsonDelString(m_data.clientname, str, length);
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
        CrossIPPort& m_data;
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

		json_data.AddIntegral(u8"gid", gid);

		json_data.AddIntegral(u8"gcp", gcp);

		json_data.AddIntegral(u8"gsp", gsp);

		json_data.AddString(u8"innerip", innerip, sizeof(innerip));

		json_data.AddString(u8"extip", extip, sizeof(extip));

		json_data.AddString(u8"innerflag", innerflag, sizeof(innerflag));

		json_data.AddString(u8"clientname", clientname, sizeof(clientname));

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
        
        CrossIPPort::Handler h(*this, nullptr);
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

struct TestJsonName{
	DataArray<unsigned char, unsigned short> name; //名称
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddArray(name);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if(!net_data.DelArray(name))
			return false;

		return true;
	}
	void Reset(void)
	{
		name.clear();

	}
	TestJsonName& operator= (const TestJsonName& src)
	{
		name = src.name;

		return *this;
	}
	bool operator==(const TestJsonName& src) const
	{
		if (name != src.name)
		{
			return false;
		}

		return true;
	}
	bool operator != (const TestJsonName& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(TestJsonName& data, JsonHandler* parent = nullptr)
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
                DataArrayHandler<unsigned char, unsigned short> handler(m_data.name, this);
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
        TestJsonName& m_data;
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

		json_data.Writer().Key(u8"name", sizeof(u8"name") - 1);
		json_data.AddArray(name);

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
        
        TestJsonName::Handler h(*this, nullptr);
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
