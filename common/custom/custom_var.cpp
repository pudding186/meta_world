#include "custom_var.hpp"
#include "utility.hpp"
/// <summary>
/// CustomVar
/// </summary>
///

void CustomVar::SetInt(const std::string& key, int64_t value)
{
    FUNC_PERFORMANCE_CHECK();
    if (0 == value)
    {
        m_int_map.erase(key);
    }
    else
    {
        m_int_map[key] = value;
    }
}

int64_t CustomVar::GetInt(const std::string& key) const
{
    FUNC_PERFORMANCE_CHECK();

    auto it = m_int_map.find(key);

    if (it != m_int_map.end())
    {
        return it->second;
    }

    return 0;
}

void CustomVar::SetStr(const std::string& key, const std::string& value)
{
    FUNC_PERFORMANCE_CHECK();
    if (value.empty())
    {
        m_str_map.erase(key);
    }
    else
    {
        m_str_map[key] = value;
    }
}

const std::string& CustomVar::GetStr(const std::string& key) const
{
    FUNC_PERFORMANCE_CHECK();

    static std::string empty_str;

    auto it = m_str_map.find(key);

    if (it != m_str_map.end())
    {
        return it->second;
    }

    return empty_str;
}

CustomVar& CustomVar::operator=(const CustomVar& custom_var)
{
    FUNC_PERFORMANCE_CHECK();

    m_int_map = custom_var.m_int_map;
    m_str_map = custom_var.m_str_map;

    return *this;
}

CustomVar::Var CustomVar::operator[](const std::string& key)
{
    FUNC_PERFORMANCE_CHECK();

    int64_t* int_value = nullptr;
    std::string* str_value = nullptr;

    auto it_int = m_int_map.find(key);

    if (it_int != m_int_map.end())
    {
        int_value = &(it_int->second);
    }

    auto it_str = m_str_map.find(key);

    if (it_str != m_str_map.end())
    {
        str_value = &(it_str->second);
    }

    return CustomVar::Var(key, int_value, str_value, *this);
}

void CustomVar::GetAllIntKeys(std::vector<std::string>& keys)
{
    FUNC_PERFORMANCE_CHECK();

    keys.clear();

    for (auto& pair : m_int_map)
    {
        keys.push_back(pair.first);
    }
}

void CustomVar::GetAllStringKeys(std::vector<std::string>& keys)
{
    FUNC_PERFORMANCE_CHECK();

    keys.clear();

    for (auto& pair : m_str_map)
    {
        keys.push_back(pair.first);
    }
}

void CustomVar::Clear()
{
    FUNC_PERFORMANCE_CHECK();

    m_int_map.clear();
    m_str_map.clear();
}

//bool CustomVar::ToJson(std::string& jsonstr)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    CustomData data;
//
//    ToCustomData(data);
//
//    //jsonstr = data.MarshalJson();
//
//    //return true;
//    return data.MarshalJson(jsonstr);
//}
//
//bool CustomVar::FromJson(const std::string& jsonstr)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    CustomData data;
//
//    if (!data.UnmarshalJson(jsonstr))
//    {
//        return false;
//    }
//    else
//    {
//        FromCustomData(data);
//    }
//
//    return true;
//}

//void CustomVar::FromCustomData(const CustomData& data)
//{
//    FUNC_PERFORMANCE_CHECK();
//    for (UINT32 i = 0; i < data.intdata.size(); i++)
//    {
//        CustomIntData& item = data.intdata[i];
//        SetInt(std::string(reinterpret_cast<char*>(item.key.data()), static_cast<size_t>(item.key.size())), item.value);
//    }
//
//    for (UINT32 i = 0; i < data.strdata.size(); i++)
//    {
//        CustomStrData& item = data.strdata[i];
//        SetStr(std::string(reinterpret_cast<char*>(item.key.data()), static_cast<size_t>(item.key.size())), 
//            std::string(reinterpret_cast<char*>(item.value.data()), static_cast<size_t>(item.value.size())));
//    }
//}
//
//void CustomVar::ToCustomData(CustomData& data)
//{
//    FUNC_PERFORMANCE_CHECK();
//    unsigned int int_idx = data.intdata.size();
//    unsigned int str_idx = data.strdata.size();
//    data.strdata.resize(data.strdata.size() + static_cast<unsigned int>(m_str_map.size()));
//    data.intdata.resize(data.intdata.size() + static_cast<unsigned int>(m_int_map.size()));
//
//    for (auto& int_pair : m_int_map)
//    {
//        CustomIntData& item = data.intdata[int_idx];
//        item.key.clear();
//        item.key.append_data(int_pair.first);
//        item.value = int_pair.second;
//
//        ++int_idx;
//    }
//
//    for (auto& str_pair : m_str_map)
//    {
//        CustomStrData& item = data.strdata[str_idx];
//        item.key.clear();
//        item.key.append_data(str_pair.first);
//        item.value.clear();
//        item.value.append_data(str_pair.second);
//
//        ++str_idx;
//    }
//}
//
//void CustomVar::ToCustomData(CustomData& data, const std::vector<std::string>& var_list)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    unsigned int int_data_size = data.intdata.size();
//    unsigned int str_data_size = data.strdata.size();
//    data.strdata.resize(data.strdata.size() + static_cast<unsigned int>(var_list.size()));
//    data.intdata.resize(data.intdata.size() + static_cast<unsigned int>(var_list.size()));
//
//    for (auto& var : var_list)
//    {
//        auto it_str = m_str_map.find(var);
//        if (it_str != m_str_map.end())
//        {
//            data.strdata[str_data_size].key.clear();
//            data.strdata[str_data_size].key.append_data(it_str->first);
//            data.strdata[str_data_size].value.clear();
//            data.strdata[str_data_size].value.append_data(it_str->second);
//            ++str_data_size;
//        }
//    }
//
//    data.strdata.resize(str_data_size);
//
//    for (auto& var : var_list)
//    {
//        auto it_int = m_int_map.find(var);
//        if (it_int != m_int_map.end())
//        {
//            data.intdata[int_data_size].key.clear();
//            data.intdata[int_data_size].key.append_data(it_int->first);
//            data.intdata[int_data_size].value = it_int->second;
//            ++int_data_size;
//        }
//    }
//
//    data.intdata.resize(int_data_size);
//}

/// <summary>
/// CustomVar::Var
/// </summary>


void CustomVar::Var::operator=(uint64_t value)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_int_value)
    {
        *m_int_value = value;
    }
    else
    {
        m_custom_var.SetInt(m_key, value);
    }
}

void CustomVar::Var::operator=(const std::string& value)
{
    FUNC_PERFORMANCE_CHECK();
    if (m_str_value)
    {
        *m_str_value = value;
    }
    else
    {
        m_custom_var.SetStr(m_key, value);
    }
}

CustomVar::Var::operator int64_t(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_int_value)
    {
        return *m_int_value;
    }

    return 0;
}

CustomVar::Var::operator std::string(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_str_value)
    {
        return *m_str_value;
    }

    return "";
}

int64_t CustomVar::Var::GetInt(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_int_value)
    {
        return *m_int_value;
    }

    return 0;
}

const std::string& CustomVar::Var::GetStr(void)
{
    FUNC_PERFORMANCE_CHECK();
    static std::string empty_str;

    if (m_str_value)
    {
        return *m_str_value;
    }
    else
    {
        return empty_str;
    }
}

void CustomVar::Var::SetInt(int64_t value)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_int_value)
    {
        *m_int_value = value;
    }
    else
    {
        m_custom_var.SetInt(m_key, value);
    }
}

void CustomVar::Var::SetStr(const std::string& value)
{
    FUNC_PERFORMANCE_CHECK();
    if (m_str_value)
    {
        *m_str_value = value;
    }
    else
    {
        m_custom_var.SetStr(m_key, value);
    }
}

