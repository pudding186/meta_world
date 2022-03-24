#pragma once
#include <string>
#include <map>
#include <vector>

#define MAX_KEY_LENGTH 128

class CustomVar
{
public:

    class Var
    {
    public:
        Var(const std::string& key, int64_t* int_value, std::string* str_value, CustomVar& custom_var):
            m_key(key), m_int_value(int_value), m_str_value(str_value), m_custom_var(custom_var){}

        void operator=(uint64_t value);

        void operator=(const std::string& value);

        operator int64_t(void);

        operator std::string(void);

        int64_t GetInt(void);
        const std::string& GetStr(void);

        void SetInt(int64_t value);
        void SetStr(const std::string& value);
    protected:
    private:
        const std::string&     m_key;
        int64_t*          m_int_value;
        std::string*    m_str_value;
        CustomVar&      m_custom_var;
    };

public:

    CustomVar() = default;
    ~CustomVar() = default;

    void SetInt(const std::string& key, int64_t value);
    int64_t GetInt(const std::string& key) const;

    void SetStr(const std::string& key, const std::string& value);
    const std::string& GetStr(const std::string& key);

    CustomVar& operator=(const CustomVar& custom_var);

    Var operator[](const std::string& key);

    void GetAllIntKeys(std::vector<std::string>& keys);

    void GetAllStringKeys(std::vector<std::string>& keys);

    void Clear();

    //bool ToJson(std::string& jsonstr);

    //bool FromJson(const std::string& jsonstr);

    //void FromCustomData(const CustomData& data);
    //void ToCustomData(CustomData& data);
    //void ToCustomData(CustomData& data, const std::vector<std::string>& var_list);

    

protected:
private:
    std::map<std::string, int64_t> m_int_map;
    std::map<std::string, std::string> m_str_map;
};

