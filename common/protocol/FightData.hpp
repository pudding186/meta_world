#pragma once

#include "net_data.hpp"

//===============包含的其他协议文件===============
#include "StructDef.hpp"
 //===============宏定义开始===============
//===============宏定义结束===============

//===============数据定义开始===============
struct ActionBegin:Protocol<ActionBegin>
{
	unsigned short            target; //目标id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

	}
	ActionBegin& operator= (const ActionBegin& src)
	{
		target = src.target;

		return *this;
	}
	bool operator==(const ActionBegin& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ActionBegin& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ActionBegin& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
        ActionBegin& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ActionBegin::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ActionEnd:Protocol<ActionEnd>
{
	unsigned short            target; //目标id
	unsigned char             state; //0:活着的 1:死亡的 2:离场的
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(state);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(state);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(state))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(state))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		state = 0;

	}
	ActionEnd& operator= (const ActionEnd& src)
	{
		target = src.target;

		state = src.state;

		return *this;
	}
	bool operator==(const ActionEnd& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (state != src.state)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ActionEnd& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ActionEnd& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
        ActionEnd& m_data;
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

		json_data.AddIntegral(u8"target", target);

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
        
        ActionEnd::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct AddBuff:Protocol<AddBuff>
{
	unsigned short            target; //目标id
	unsigned short            skill; //技能id
	unsigned short            buff; //buffid
	unsigned char             stack; //堆叠层数
	unsigned short            remain; //剩余回合数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skill);

		net_data.AddIntegral(buff);

		net_data.AddIntegral(stack);

		net_data.AddIntegral(remain);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skill);

		net_data.AddIntegral(buff);

		net_data.AddIntegral(stack);

		net_data.AddIntegral(remain);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skill))
			return false;

		if (!net_data.DelIntegral(buff))
			return false;

		if (!net_data.DelIntegral(stack))
			return false;

		if (!net_data.DelIntegral(remain))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skill))
			return false;

		if (!net_data.DelIntegral(buff))
			return false;

		if (!net_data.DelIntegral(stack))
			return false;

		if (!net_data.DelIntegral(remain))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		skill = 0;

		buff = 0;

		stack = 0;

		remain = 0;

	}
	AddBuff& operator= (const AddBuff& src)
	{
		target = src.target;

		skill = src.skill;

		buff = src.buff;

		stack = src.stack;

		remain = src.remain;

		return *this;
	}
	bool operator==(const AddBuff& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (skill != src.skill)
		{
			return false;
		}

		if (buff != src.buff)
		{
			return false;
		}

		if (stack != src.stack)
		{
			return false;
		}

		if (remain != src.remain)
		{
			return false;
		}

		return true;
	}
	bool operator != (const AddBuff& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(AddBuff& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.skill, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.buff, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.stack, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.remain, str, length);
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
        AddBuff& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"skill", skill);

		json_data.AddIntegral(u8"buff", buff);

		json_data.AddIntegral(u8"stack", stack);

		json_data.AddIntegral(u8"remain", remain);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        AddBuff::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct DelBuff:Protocol<DelBuff>
{
	unsigned short            target; //目标id
	unsigned short            buff; //buffid
	unsigned char             stack; //堆叠层数
	unsigned short            remain; //剩余回合数
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(buff);

		net_data.AddIntegral(stack);

		net_data.AddIntegral(remain);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(buff);

		net_data.AddIntegral(stack);

		net_data.AddIntegral(remain);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(buff))
			return false;

		if (!net_data.DelIntegral(stack))
			return false;

		if (!net_data.DelIntegral(remain))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(buff))
			return false;

		if (!net_data.DelIntegral(stack))
			return false;

		if (!net_data.DelIntegral(remain))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		buff = 0;

		stack = 0;

		remain = 0;

	}
	DelBuff& operator= (const DelBuff& src)
	{
		target = src.target;

		buff = src.buff;

		stack = src.stack;

		remain = src.remain;

		return *this;
	}
	bool operator==(const DelBuff& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (buff != src.buff)
		{
			return false;
		}

		if (stack != src.stack)
		{
			return false;
		}

		if (remain != src.remain)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DelBuff& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DelBuff& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.buff, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.stack, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.remain, str, length);
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
        DelBuff& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"buff", buff);

		json_data.AddIntegral(u8"stack", stack);

		json_data.AddIntegral(u8"remain", remain);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        DelBuff::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct DiedNtf:Protocol<DiedNtf>
{
	unsigned short            target; //目标通知
	unsigned char             leave; //死亡后是否出场
	signed long long          effect; //死亡特效ID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(leave);

		net_data.AddIntegral(effect);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(leave);

		net_data.AddIntegral(effect);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(leave))
			return false;

		if (!net_data.DelIntegral(effect))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(leave))
			return false;

		if (!net_data.DelIntegral(effect))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		leave = 0;

		effect = 0;

	}
	DiedNtf& operator= (const DiedNtf& src)
	{
		target = src.target;

		leave = src.leave;

		effect = src.effect;

		return *this;
	}
	bool operator==(const DiedNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (leave != src.leave)
		{
			return false;
		}

		if (effect != src.effect)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DiedNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DiedNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.leave, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.effect, str, length);
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
        DiedNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"leave", leave);

		json_data.AddIntegral(u8"effect", effect);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        DiedNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ReviveNtf:Protocol<ReviveNtf>
{
	unsigned short            target; //目标通知
	int                       hp; //如果为友方则显示血量,敌方为0
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(hp);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(hp);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(hp))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(hp))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		hp = 0;

	}
	ReviveNtf& operator= (const ReviveNtf& src)
	{
		target = src.target;

		hp = src.hp;

		return *this;
	}
	bool operator==(const ReviveNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (hp != src.hp)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ReviveNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ReviveNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.hp, str, length);
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
        ReviveNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"hp", hp);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ReviveNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct PaoPaoNtf:Protocol<PaoPaoNtf>
{
	unsigned short            target; //消息目标
	char                      text[512]; //消息内容
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddString(text, sizeof(text));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddString(text, sizeof(text));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelString(text, sizeof(text)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelString(text, sizeof(text)))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		text[0] = 0;

	}
	PaoPaoNtf& operator= (const PaoPaoNtf& src)
	{
		target = src.target;

		{
			size_t str_len = strnlen(src.text, sizeof(text) - 1);
			memcpy(text, src.text, str_len + 1);
			text[sizeof(text) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const PaoPaoNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (strncmp(text, src.text, sizeof(text)-1))
		{
			return false;
		}

		return true;
	}
	bool operator != (const PaoPaoNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(PaoPaoNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
        PaoPaoNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

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
        
        PaoPaoNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct FightAttrNtf:Protocol<FightAttrNtf>
{
	unsigned short            target; //目标id
	unsigned short            attr; //属性名
	signed long long          value; //属性值 大于0为增加 小于0为减少
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(attr);

		net_data.AddIntegral(value);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(attr);

		net_data.AddIntegral(value);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(attr))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(attr))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		attr = 0;

		value = 0;

	}
	FightAttrNtf& operator= (const FightAttrNtf& src)
	{
		target = src.target;

		attr = src.attr;

		value = src.value;

		return *this;
	}
	bool operator==(const FightAttrNtf& src) const
	{
		if (target != src.target)
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
	bool operator != (const FightAttrNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(FightAttrNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
        FightAttrNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

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
        
        FightAttrNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ShotSKillNtf:Protocol<ShotSKillNtf>
{
	unsigned short            actor; //施放者id
	unsigned int              skillid; //施放技能id
	DataArray<unsigned short, unsigned char> targets; //攻击目标
	unsigned char             mastery; //是否触发精通
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(skillid);

		net_data.AddArray(targets);

		net_data.AddIntegral(mastery);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(skillid);

		net_data.AddArray(targets);

		net_data.AddIntegral(mastery);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(skillid))
			return false;

		if(!net_data.DelArray(targets))
			return false;

		if (!net_data.DelIntegral(mastery))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(skillid))
			return false;

		if(!net_data.DelArray(targets))
			return false;

		if (!net_data.DelIntegral(mastery))
			return false;

		return true;
	}
	void Reset(void)
	{
		actor = 0;

		skillid = 0;

		targets.clear();

		mastery = 0;

	}
	ShotSKillNtf& operator= (const ShotSKillNtf& src)
	{
		actor = src.actor;

		skillid = src.skillid;

		targets = src.targets;

		mastery = src.mastery;

		return *this;
	}
	bool operator==(const ShotSKillNtf& src) const
	{
		if (actor != src.actor)
		{
			return false;
		}

		if (skillid != src.skillid)
		{
			return false;
		}

		if (targets != src.targets)
		{
			return false;
		}

		if (mastery != src.mastery)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ShotSKillNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ShotSKillNtf& data, JsonHandler* parent = nullptr)
            :
            JsonHandler(parent),
            m_data(data),
            m_targets_handler(m_data.targets, this),
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
                JsonDelIntegral(m_data.skillid, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.mastery, str, length);
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
                return &m_targets_handler;
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

            if (!m_targets_handler.IsAllMemberSet())
            {
                return false;
            }

            return true;
        }
    private:
        ShotSKillNtf& m_data;
        DataArrayHandler<unsigned short, unsigned char> m_targets_handler;
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

		json_data.AddIntegral(u8"skillid", skillid);

		json_data.Writer().Key(u8"targets", sizeof(u8"targets") - 1);
		json_data.AddArray(targets);

		json_data.AddIntegral(u8"mastery", mastery);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ShotSKillNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct SkillCooldownNtf:Protocol<SkillCooldownNtf>
{
	unsigned short            target; //目标id
	unsigned short            skillid; //技能id
	unsigned short            cd; //cd
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skillid);

		net_data.AddIntegral(cd);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skillid);

		net_data.AddIntegral(cd);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skillid))
			return false;

		if (!net_data.DelIntegral(cd))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skillid))
			return false;

		if (!net_data.DelIntegral(cd))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		skillid = 0;

		cd = 0;

	}
	SkillCooldownNtf& operator= (const SkillCooldownNtf& src)
	{
		target = src.target;

		skillid = src.skillid;

		cd = src.cd;

		return *this;
	}
	bool operator==(const SkillCooldownNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (skillid != src.skillid)
		{
			return false;
		}

		if (cd != src.cd)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SkillCooldownNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SkillCooldownNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.skillid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.cd, str, length);
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
        SkillCooldownNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"skillid", skillid);

		json_data.AddIntegral(u8"cd", cd);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        SkillCooldownNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ShowAttrChangeNtf:Protocol<ShowAttrChangeNtf>
{
	unsigned short            target; //目标id
	unsigned short            attr; //属性名
	signed long long          value; //属性值 大于0为增加 小于0为减少
	unsigned char             type; //伤害/增益类型 0:普通 5:毒伤 6:震摄 7:风系 8:雷系 9:水系 10:火系 11:三尸 12:鬼火 13:物理
	unsigned char             burst; //0:普通 1:爆击 2:致死
	unsigned char             ignore_phydef; //0:普通 1:破物理攻击
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(attr);

		net_data.AddIntegral(value);

		net_data.AddIntegral(type);

		net_data.AddIntegral(burst);

		net_data.AddIntegral(ignore_phydef);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(attr);

		net_data.AddIntegral(value);

		net_data.AddIntegral(type);

		net_data.AddIntegral(burst);

		net_data.AddIntegral(ignore_phydef);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(attr))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(burst))
			return false;

		if (!net_data.DelIntegral(ignore_phydef))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(attr))
			return false;

		if (!net_data.DelIntegral(value))
			return false;

		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(burst))
			return false;

		if (!net_data.DelIntegral(ignore_phydef))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		attr = 0;

		value = 0;

		type = 0;

		burst = 0;

		ignore_phydef = 0;

	}
	ShowAttrChangeNtf& operator= (const ShowAttrChangeNtf& src)
	{
		target = src.target;

		attr = src.attr;

		value = src.value;

		type = src.type;

		burst = src.burst;

		ignore_phydef = src.ignore_phydef;

		return *this;
	}
	bool operator==(const ShowAttrChangeNtf& src) const
	{
		if (target != src.target)
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

		if (type != src.type)
		{
			return false;
		}

		if (burst != src.burst)
		{
			return false;
		}

		if (ignore_phydef != src.ignore_phydef)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ShowAttrChangeNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ShowAttrChangeNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.type, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.burst, str, length);
            }
            break;
            case 5:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.ignore_phydef, str, length);
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
        ShowAttrChangeNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"attr", attr);

		json_data.AddIntegral(u8"value", value);

		json_data.AddIntegral(u8"type", type);

		json_data.AddIntegral(u8"burst", burst);

		json_data.AddIntegral(u8"ignore_phydef", ignore_phydef);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ShowAttrChangeNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct MissNtf:Protocol<MissNtf>
{
	unsigned short            target; //目标id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

	}
	MissNtf& operator= (const MissNtf& src)
	{
		target = src.target;

		return *this;
	}
	bool operator==(const MissNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		return true;
	}
	bool operator != (const MissNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(MissNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
        MissNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        MissNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct DefenseNtf:Protocol<DefenseNtf>
{
	unsigned short            target; //目标id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

	}
	DefenseNtf& operator= (const DefenseNtf& src)
	{
		target = src.target;

		return *this;
	}
	bool operator==(const DefenseNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		return true;
	}
	bool operator != (const DefenseNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(DefenseNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
        DefenseNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        DefenseNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct EscapeNtf:Protocol<EscapeNtf>
{
	unsigned short            target; //目标id
	unsigned char             result; //0:失败 1:成功
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(result);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(result);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(result))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(result))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		result = 0;

	}
	EscapeNtf& operator= (const EscapeNtf& src)
	{
		target = src.target;

		result = src.result;

		return *this;
	}
	bool operator==(const EscapeNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (result != src.result)
		{
			return false;
		}

		return true;
	}
	bool operator != (const EscapeNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(EscapeNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.result, str, length);
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
        EscapeNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"result", result);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        EscapeNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ProtectNtf:Protocol<ProtectNtf>
{
	unsigned short            actor; //发起者id
	unsigned short            target; //目标id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(target);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(target);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(target))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(target))
			return false;

		return true;
	}
	void Reset(void)
	{
		actor = 0;

		target = 0;

	}
	ProtectNtf& operator= (const ProtectNtf& src)
	{
		actor = src.actor;

		target = src.target;

		return *this;
	}
	bool operator==(const ProtectNtf& src) const
	{
		if (actor != src.actor)
		{
			return false;
		}

		if (target != src.target)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ProtectNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ProtectNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
        ProtectNtf& m_data;
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

		json_data.AddIntegral(u8"actor", actor);

		json_data.AddIntegral(u8"target", target);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ProtectNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct HuaWuNtf:Protocol<HuaWuNtf>
{
	unsigned short            target; //目标id
	unsigned short            skillid; //技能id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skillid);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skillid);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skillid))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skillid))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		skillid = 0;

	}
	HuaWuNtf& operator= (const HuaWuNtf& src)
	{
		target = src.target;

		skillid = src.skillid;

		return *this;
	}
	bool operator==(const HuaWuNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (skillid != src.skillid)
		{
			return false;
		}

		return true;
	}
	bool operator != (const HuaWuNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(HuaWuNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.skillid, str, length);
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
        HuaWuNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"skillid", skillid);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        HuaWuNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct FailNtf:Protocol<FailNtf>
{
	unsigned short            target; //目标id
	int                       errcode; //错误码
	char                      errmsg[256]; //错误描述
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(errcode);

		net_data.AddString(errmsg, sizeof(errmsg));

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(errcode))
			return false;

		if (!net_data.DelString(errmsg, sizeof(errmsg)))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		errcode = 0;

		errmsg[0] = 0;

	}
	FailNtf& operator= (const FailNtf& src)
	{
		target = src.target;

		errcode = src.errcode;

		{
			size_t str_len = strnlen(src.errmsg, sizeof(errmsg) - 1);
			memcpy(errmsg, src.errmsg, str_len + 1);
			errmsg[sizeof(errmsg) - 1] = 0;
		}

		return *this;
	}
	bool operator==(const FailNtf& src) const
	{
		if (target != src.target)
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
	bool operator != (const FailNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(FailNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
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
        FailNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

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
        
        FailNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct SummonNtf:Protocol<SummonNtf>
{
	unsigned short            actor; //召唤者id
	unsigned char             action; //是否有召唤动作
	unsigned short            target1; //召唤目标1id
	unsigned short            target2; //召唤目标2id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(action);

		net_data.AddIntegral(target1);

		net_data.AddIntegral(target2);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(action);

		net_data.AddIntegral(target1);

		net_data.AddIntegral(target2);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(action))
			return false;

		if (!net_data.DelIntegral(target1))
			return false;

		if (!net_data.DelIntegral(target2))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(action))
			return false;

		if (!net_data.DelIntegral(target1))
			return false;

		if (!net_data.DelIntegral(target2))
			return false;

		return true;
	}
	void Reset(void)
	{
		actor = 0;

		action = 0;

		target1 = 0;

		target2 = 0;

	}
	SummonNtf& operator= (const SummonNtf& src)
	{
		actor = src.actor;

		action = src.action;

		target1 = src.target1;

		target2 = src.target2;

		return *this;
	}
	bool operator==(const SummonNtf& src) const
	{
		if (actor != src.actor)
		{
			return false;
		}

		if (action != src.action)
		{
			return false;
		}

		if (target1 != src.target1)
		{
			return false;
		}

		if (target2 != src.target2)
		{
			return false;
		}

		return true;
	}
	bool operator != (const SummonNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(SummonNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.action, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target1, str, length);
            }
            break;
            case 3:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.target2, str, length);
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
        SummonNtf& m_data;
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

		json_data.AddIntegral(u8"action", action);

		json_data.AddIntegral(u8"target1", target1);

		json_data.AddIntegral(u8"target2", target2);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        SummonNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ShowFighterNtf:Protocol<ShowFighterNtf>
{
	unsigned short            target; //显示者id
	unsigned short            replace; //替换者id
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(replace);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(replace);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(replace))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(replace))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		replace = 0;

	}
	ShowFighterNtf& operator= (const ShowFighterNtf& src)
	{
		target = src.target;

		replace = src.replace;

		return *this;
	}
	bool operator==(const ShowFighterNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (replace != src.replace)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ShowFighterNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ShowFighterNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.replace, str, length);
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
        ShowFighterNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"replace", replace);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ShowFighterNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct HideFighterNtf:Protocol<HideFighterNtf>
{
	unsigned short            target; //战斗者id
	signed long long          effect; //隐藏特效ID
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(effect);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(effect);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(effect))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(effect))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		effect = 0;

	}
	HideFighterNtf& operator= (const HideFighterNtf& src)
	{
		target = src.target;

		effect = src.effect;

		return *this;
	}
	bool operator==(const HideFighterNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (effect != src.effect)
		{
			return false;
		}

		return true;
	}
	bool operator != (const HideFighterNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(HideFighterNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.effect, str, length);
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
        HideFighterNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"effect", effect);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        HideFighterNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct CatchNtf:Protocol<CatchNtf>
{
	unsigned short            actor; //捕捉者id
	unsigned short            target; //捕捉目标1id
	unsigned char             result; //0:失败 1:成功
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(target);

		net_data.AddIntegral(result);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(actor);

		net_data.AddIntegral(target);

		net_data.AddIntegral(result);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(result))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(result))
			return false;

		return true;
	}
	void Reset(void)
	{
		actor = 0;

		target = 0;

		result = 0;

	}
	CatchNtf& operator= (const CatchNtf& src)
	{
		actor = src.actor;

		target = src.target;

		result = src.result;

		return *this;
	}
	bool operator==(const CatchNtf& src) const
	{
		if (actor != src.actor)
		{
			return false;
		}

		if (target != src.target)
		{
			return false;
		}

		if (result != src.result)
		{
			return false;
		}

		return true;
	}
	bool operator != (const CatchNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(CatchNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.result, str, length);
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
        CatchNtf& m_data;
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

		json_data.AddIntegral(u8"actor", actor);

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"result", result);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        CatchNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ShowEffectNtf:Protocol<ShowEffectNtf>
{
	unsigned char             type; //特效类型
	unsigned short            actor; //发起者id
	unsigned short            target; //目标1id
	signed long long          param1; //自定义参数1
	signed long long          param2; //自定义参数2
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(actor);

		net_data.AddIntegral(target);

		net_data.AddIntegral(param1);

		net_data.AddIntegral(param2);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(type);

		net_data.AddIntegral(actor);

		net_data.AddIntegral(target);

		net_data.AddIntegral(param1);

		net_data.AddIntegral(param2);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(param1))
			return false;

		if (!net_data.DelIntegral(param2))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(type))
			return false;

		if (!net_data.DelIntegral(actor))
			return false;

		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(param1))
			return false;

		if (!net_data.DelIntegral(param2))
			return false;

		return true;
	}
	void Reset(void)
	{
		type = 0;

		actor = 0;

		target = 0;

		param1 = 0;

		param2 = 0;

	}
	ShowEffectNtf& operator= (const ShowEffectNtf& src)
	{
		type = src.type;

		actor = src.actor;

		target = src.target;

		param1 = src.param1;

		param2 = src.param2;

		return *this;
	}
	bool operator==(const ShowEffectNtf& src) const
	{
		if (type != src.type)
		{
			return false;
		}

		if (actor != src.actor)
		{
			return false;
		}

		if (target != src.target)
		{
			return false;
		}

		if (param1 != src.param1)
		{
			return false;
		}

		if (param2 != src.param2)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ShowEffectNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ShowEffectNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.actor, str, length);
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
                JsonDelIntegral(m_data.param1, str, length);
            }
            break;
            case 4:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.param2, str, length);
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
        ShowEffectNtf& m_data;
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

		json_data.AddIntegral(u8"actor", actor);

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"param1", param1);

		json_data.AddIntegral(u8"param2", param2);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ShowEffectNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct StealSkillNtf:Protocol<StealSkillNtf>
{
	unsigned short            target; //目标id
	unsigned short            skill; //技能id
	unsigned int              performance; //技能熟练度
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skill);

		net_data.AddIntegral(performance);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skill);

		net_data.AddIntegral(performance);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skill))
			return false;

		if (!net_data.DelIntegral(performance))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skill))
			return false;

		if (!net_data.DelIntegral(performance))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		skill = 0;

		performance = 0;

	}
	StealSkillNtf& operator= (const StealSkillNtf& src)
	{
		target = src.target;

		skill = src.skill;

		performance = src.performance;

		return *this;
	}
	bool operator==(const StealSkillNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (skill != src.skill)
		{
			return false;
		}

		if (performance != src.performance)
		{
			return false;
		}

		return true;
	}
	bool operator != (const StealSkillNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(StealSkillNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.skill, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.performance, str, length);
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
        StealSkillNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"skill", skill);

		json_data.AddIntegral(u8"performance", performance);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        StealSkillNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
        rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>, JsonAllocator> rd(&json_allocator, 1024);
        rapidjson::StringStream ss(json.c_str());

        if (rd.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, jd))
        {
            return h.IsAllMemberSet();
        }

        return false;
    }
};

struct ForgetNtf:Protocol<ForgetNtf>
{
	unsigned short            target; //目标id
	unsigned short            skillid; //技能id
	unsigned char             forget; //0:未遗忘 1:遗忘
	bool EnCode(NetEnCode& net_data)
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skillid);

		net_data.AddIntegral(forget);

		return true;
	}
	bool EnCodeEx(NetEnCode& net_data) override
	{
		net_data.AddIntegral(target);

		net_data.AddIntegral(skillid);

		net_data.AddIntegral(forget);

		return true;
	}
	bool DeCode(NetDeCode& net_data)
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skillid))
			return false;

		if (!net_data.DelIntegral(forget))
			return false;

		return true;
	}
	bool DeCodeEx(NetDeCode& net_data) override
	{
		if (!net_data.DelIntegral(target))
			return false;

		if (!net_data.DelIntegral(skillid))
			return false;

		if (!net_data.DelIntegral(forget))
			return false;

		return true;
	}
	void Reset(void)
	{
		target = 0;

		skillid = 0;

		forget = 0;

	}
	ForgetNtf& operator= (const ForgetNtf& src)
	{
		target = src.target;

		skillid = src.skillid;

		forget = src.forget;

		return *this;
	}
	bool operator==(const ForgetNtf& src) const
	{
		if (target != src.target)
		{
			return false;
		}

		if (skillid != src.skillid)
		{
			return false;
		}

		if (forget != src.forget)
		{
			return false;
		}

		return true;
	}
	bool operator != (const ForgetNtf& src) const
	{
		return !(*this == src);
	}

    //===============Json===============

    class Handler :
        public JsonHandler
    {
    public:
        Handler(ForgetNtf& data, JsonHandler* parent = nullptr)
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
                JsonDelIntegral(m_data.target, str, length);
            }
            break;
            case 1:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.skillid, str, length);
            }
            break;
            case 2:
            {
                m_assigned_bitset.set(m_state);
                JsonDelIntegral(m_data.forget, str, length);
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
        ForgetNtf& m_data;
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

		json_data.AddIntegral(u8"target", target);

		json_data.AddIntegral(u8"skillid", skillid);

		json_data.AddIntegral(u8"forget", forget);

		json_data.Writer().EndObject();

		return json_data.Writer().IsComplete();
	}

    bool MarshalJson(std::string& json) const
    {
        JsonEnCode json_encode(4096);

        bool json_ret = this->ToJson(json_encode);

        json = json_encode.ToString();

        return json_ret;
    }

    bool UnmarshalJson(const std::string& json)
    {
        if (json.empty())
        {
            return true;
        }
        
        ForgetNtf::Handler h(*this, nullptr);
        JsonDeCode jd(&h);

        JsonAllocator json_allocator;
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
class CFightData
{
public:
	CFightData()
	{
		size_t max_protocol_size = 0;
		if (sizeof(ActionBegin) > max_protocol_size)
			max_protocol_size = sizeof(ActionBegin);

		if (sizeof(ActionEnd) > max_protocol_size)
			max_protocol_size = sizeof(ActionEnd);

		if (sizeof(AddBuff) > max_protocol_size)
			max_protocol_size = sizeof(AddBuff);

		if (sizeof(DelBuff) > max_protocol_size)
			max_protocol_size = sizeof(DelBuff);

		if (sizeof(DiedNtf) > max_protocol_size)
			max_protocol_size = sizeof(DiedNtf);

		if (sizeof(ReviveNtf) > max_protocol_size)
			max_protocol_size = sizeof(ReviveNtf);

		if (sizeof(PaoPaoNtf) > max_protocol_size)
			max_protocol_size = sizeof(PaoPaoNtf);

		if (sizeof(FightAttrNtf) > max_protocol_size)
			max_protocol_size = sizeof(FightAttrNtf);

		if (sizeof(ShotSKillNtf) > max_protocol_size)
			max_protocol_size = sizeof(ShotSKillNtf);

		if (sizeof(SkillCooldownNtf) > max_protocol_size)
			max_protocol_size = sizeof(SkillCooldownNtf);

		if (sizeof(ShowAttrChangeNtf) > max_protocol_size)
			max_protocol_size = sizeof(ShowAttrChangeNtf);

		if (sizeof(MissNtf) > max_protocol_size)
			max_protocol_size = sizeof(MissNtf);

		if (sizeof(DefenseNtf) > max_protocol_size)
			max_protocol_size = sizeof(DefenseNtf);

		if (sizeof(EscapeNtf) > max_protocol_size)
			max_protocol_size = sizeof(EscapeNtf);

		if (sizeof(ProtectNtf) > max_protocol_size)
			max_protocol_size = sizeof(ProtectNtf);

		if (sizeof(HuaWuNtf) > max_protocol_size)
			max_protocol_size = sizeof(HuaWuNtf);

		if (sizeof(FailNtf) > max_protocol_size)
			max_protocol_size = sizeof(FailNtf);

		if (sizeof(SummonNtf) > max_protocol_size)
			max_protocol_size = sizeof(SummonNtf);

		if (sizeof(ShowFighterNtf) > max_protocol_size)
			max_protocol_size = sizeof(ShowFighterNtf);

		if (sizeof(HideFighterNtf) > max_protocol_size)
			max_protocol_size = sizeof(HideFighterNtf);

		if (sizeof(CatchNtf) > max_protocol_size)
			max_protocol_size = sizeof(CatchNtf);

		if (sizeof(ShowEffectNtf) > max_protocol_size)
			max_protocol_size = sizeof(ShowEffectNtf);

		if (sizeof(StealSkillNtf) > max_protocol_size)
			max_protocol_size = sizeof(StealSkillNtf);

		if (sizeof(ForgetNtf) > max_protocol_size)
			max_protocol_size = sizeof(ForgetNtf);

		m_protocol_buffer = S_MALLOC(max_protocol_size);
	}

	virtual ~CFightData()
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
		if (proto.module_id != 108)
			return false;

		net_data.AddIntegral(proto.module_id);
		net_data.AddIntegral(proto.protocol_id);

		return static_cast<T&>(proto).EnCode(net_data);
	}

	bool BuildProtocol(protocol_base* proto, NetEnCode& net_data)
	{
		if (proto->ModuleId() != 108)
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
			return ActionBegin::SName();
		}
		break;
		case 2:
		{
			return ActionEnd::SName();
		}
		break;
		case 3:
		{
			return AddBuff::SName();
		}
		break;
		case 4:
		{
			return DelBuff::SName();
		}
		break;
		case 5:
		{
			return DiedNtf::SName();
		}
		break;
		case 6:
		{
			return ReviveNtf::SName();
		}
		break;
		case 7:
		{
			return PaoPaoNtf::SName();
		}
		break;
		case 8:
		{
			return FightAttrNtf::SName();
		}
		break;
		case 9:
		{
			return ShotSKillNtf::SName();
		}
		break;
		case 10:
		{
			return SkillCooldownNtf::SName();
		}
		break;
		case 11:
		{
			return ShowAttrChangeNtf::SName();
		}
		break;
		case 12:
		{
			return MissNtf::SName();
		}
		break;
		case 13:
		{
			return DefenseNtf::SName();
		}
		break;
		case 14:
		{
			return EscapeNtf::SName();
		}
		break;
		case 15:
		{
			return ProtectNtf::SName();
		}
		break;
		case 16:
		{
			return HuaWuNtf::SName();
		}
		break;
		case 17:
		{
			return FailNtf::SName();
		}
		break;
		case 18:
		{
			return SummonNtf::SName();
		}
		break;
		case 19:
		{
			return ShowFighterNtf::SName();
		}
		break;
		case 20:
		{
			return HideFighterNtf::SName();
		}
		break;
		case 21:
		{
			return CatchNtf::SName();
		}
		break;
		case 22:
		{
			return ShowEffectNtf::SName();
		}
		break;
		case 23:
		{
			return StealSkillNtf::SName();
		}
		break;
		case 24:
		{
			return ForgetNtf::SName();
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
		case 1:
		{
			ActionBegin* proto = new(m_protocol_buffer) ActionBegin();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ActionBegin(*proto);
				proto->~ActionBegin();
				return true;
			}
			else
			{
				proto->~ActionBegin();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 2:
		{
			ActionEnd* proto = new(m_protocol_buffer) ActionEnd();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ActionEnd(*proto);
				proto->~ActionEnd();
				return true;
			}
			else
			{
				proto->~ActionEnd();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 3:
		{
			AddBuff* proto = new(m_protocol_buffer) AddBuff();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_AddBuff(*proto);
				proto->~AddBuff();
				return true;
			}
			else
			{
				proto->~AddBuff();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 4:
		{
			DelBuff* proto = new(m_protocol_buffer) DelBuff();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DelBuff(*proto);
				proto->~DelBuff();
				return true;
			}
			else
			{
				proto->~DelBuff();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 5:
		{
			DiedNtf* proto = new(m_protocol_buffer) DiedNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DiedNtf(*proto);
				proto->~DiedNtf();
				return true;
			}
			else
			{
				proto->~DiedNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 6:
		{
			ReviveNtf* proto = new(m_protocol_buffer) ReviveNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ReviveNtf(*proto);
				proto->~ReviveNtf();
				return true;
			}
			else
			{
				proto->~ReviveNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 7:
		{
			PaoPaoNtf* proto = new(m_protocol_buffer) PaoPaoNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_PaoPaoNtf(*proto);
				proto->~PaoPaoNtf();
				return true;
			}
			else
			{
				proto->~PaoPaoNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 8:
		{
			FightAttrNtf* proto = new(m_protocol_buffer) FightAttrNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_FightAttrNtf(*proto);
				proto->~FightAttrNtf();
				return true;
			}
			else
			{
				proto->~FightAttrNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 9:
		{
			ShotSKillNtf* proto = new(m_protocol_buffer) ShotSKillNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ShotSKillNtf(*proto);
				proto->~ShotSKillNtf();
				return true;
			}
			else
			{
				proto->~ShotSKillNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 10:
		{
			SkillCooldownNtf* proto = new(m_protocol_buffer) SkillCooldownNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SkillCooldownNtf(*proto);
				proto->~SkillCooldownNtf();
				return true;
			}
			else
			{
				proto->~SkillCooldownNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 11:
		{
			ShowAttrChangeNtf* proto = new(m_protocol_buffer) ShowAttrChangeNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ShowAttrChangeNtf(*proto);
				proto->~ShowAttrChangeNtf();
				return true;
			}
			else
			{
				proto->~ShowAttrChangeNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 12:
		{
			MissNtf* proto = new(m_protocol_buffer) MissNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_MissNtf(*proto);
				proto->~MissNtf();
				return true;
			}
			else
			{
				proto->~MissNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 13:
		{
			DefenseNtf* proto = new(m_protocol_buffer) DefenseNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_DefenseNtf(*proto);
				proto->~DefenseNtf();
				return true;
			}
			else
			{
				proto->~DefenseNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 14:
		{
			EscapeNtf* proto = new(m_protocol_buffer) EscapeNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_EscapeNtf(*proto);
				proto->~EscapeNtf();
				return true;
			}
			else
			{
				proto->~EscapeNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 15:
		{
			ProtectNtf* proto = new(m_protocol_buffer) ProtectNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ProtectNtf(*proto);
				proto->~ProtectNtf();
				return true;
			}
			else
			{
				proto->~ProtectNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 16:
		{
			HuaWuNtf* proto = new(m_protocol_buffer) HuaWuNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_HuaWuNtf(*proto);
				proto->~HuaWuNtf();
				return true;
			}
			else
			{
				proto->~HuaWuNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 17:
		{
			FailNtf* proto = new(m_protocol_buffer) FailNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_FailNtf(*proto);
				proto->~FailNtf();
				return true;
			}
			else
			{
				proto->~FailNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 18:
		{
			SummonNtf* proto = new(m_protocol_buffer) SummonNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_SummonNtf(*proto);
				proto->~SummonNtf();
				return true;
			}
			else
			{
				proto->~SummonNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 19:
		{
			ShowFighterNtf* proto = new(m_protocol_buffer) ShowFighterNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ShowFighterNtf(*proto);
				proto->~ShowFighterNtf();
				return true;
			}
			else
			{
				proto->~ShowFighterNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 20:
		{
			HideFighterNtf* proto = new(m_protocol_buffer) HideFighterNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_HideFighterNtf(*proto);
				proto->~HideFighterNtf();
				return true;
			}
			else
			{
				proto->~HideFighterNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 21:
		{
			CatchNtf* proto = new(m_protocol_buffer) CatchNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_CatchNtf(*proto);
				proto->~CatchNtf();
				return true;
			}
			else
			{
				proto->~CatchNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 22:
		{
			ShowEffectNtf* proto = new(m_protocol_buffer) ShowEffectNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ShowEffectNtf(*proto);
				proto->~ShowEffectNtf();
				return true;
			}
			else
			{
				proto->~ShowEffectNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 23:
		{
			StealSkillNtf* proto = new(m_protocol_buffer) StealSkillNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_StealSkillNtf(*proto);
				proto->~StealSkillNtf();
				return true;
			}
			else
			{
				proto->~StealSkillNtf();
				net_data.Reset(net_data_pos);
				return false;
			}
		}
		break;
		case 24:
		{
			ForgetNtf* proto = new(m_protocol_buffer) ForgetNtf();
			if (proto->DeCode(net_data))
			{
				static_cast<D*>(this)->OnRecv_ForgetNtf(*proto);
				proto->~ForgetNtf();
				return true;
			}
			else
			{
				proto->~ForgetNtf();
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

	static inline unsigned short GetProtocolNum(void){ return 24; }

	static const unsigned short module_id = 108;

	static const unsigned short protocol_num = 24;

//===============以下协议回调函数需要使用者来实现===============
	void OnRecv_ActionBegin(ActionBegin& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ActionEnd(ActionEnd& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_AddBuff(AddBuff& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DelBuff(DelBuff& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DiedNtf(DiedNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ReviveNtf(ReviveNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_PaoPaoNtf(PaoPaoNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_FightAttrNtf(FightAttrNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ShotSKillNtf(ShotSKillNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SkillCooldownNtf(SkillCooldownNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ShowAttrChangeNtf(ShowAttrChangeNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_MissNtf(MissNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_DefenseNtf(DefenseNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_EscapeNtf(EscapeNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ProtectNtf(ProtectNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_HuaWuNtf(HuaWuNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_FailNtf(FailNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_SummonNtf(SummonNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ShowFighterNtf(ShowFighterNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_HideFighterNtf(HideFighterNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_CatchNtf(CatchNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ShowEffectNtf(ShowEffectNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_StealSkillNtf(StealSkillNtf& rstProtocol){ (void)(rstProtocol); };
	void OnRecv_ForgetNtf(ForgetNtf& rstProtocol){ (void)(rstProtocol); };
private:
	 void* m_protocol_buffer;

};
