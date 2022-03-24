#include "utility.hpp"
#include "log_system.hpp"
#include "app_util.hpp"
#include "timer_system.hpp"

LogSystem::LogSystem()
    :
    m_log_dir(u8"./"),
    m_default_logger(nullptr),
    m_delay_caller(nullptr)
{

}

LogSystem::~LogSystem()
{

}

bool LogSystem::IsOk(void)
{
    return sLogSystem.m_default_logger != nullptr;
}

bool LogSystem::Initialize(size_t log_thread_num, size_t max_log_event_num, size_t print_cache_size)
{
    FUNC_PERFORMANCE_CHECK();
    return init_logger_manager(log_thread_num, max_log_event_num, print_cache_size);
}

bool LogSystem::CreateDefaultLogger(const std::string& log_dir, const std::string& default_log_name)
{
    FUNC_PERFORMANCE_CHECK();

    std::string utf8_log_path;

    if (!AppUtil::DefaultToUtf8(log_dir, utf8_log_path))
    {
        LogPrinf(u8"'{}' log path to utf8  fail.\n", log_dir);
        return false;
    }
    else
    {
        m_log_dir = utf8_log_path;
    }

    if (m_default_logger)
    {
        file_logger_flush(m_default_logger);
        destroy_file_logger(m_default_logger);
        m_default_logger = nullptr;
    }

    HFILELOGGER new_default_logger = create_file_logger(
        m_log_dir.c_str(),
        default_log_name.c_str());

    if (!new_default_logger)
    {
        if (m_default_logger)
        {
            LogErr(u8"create log '{}' fail.\n",
                default_log_name);
        }
        else
        {
            LogPrinf(u8"create log '{}' fail.\n",
                default_log_name);
        }
        return false;
    }
    else
    {
        if (m_default_logger)
        {
            file_logger_flush(m_default_logger);
            destroy_file_logger(m_default_logger);
        }

        m_default_logger = new_default_logger;
    }

    m_switchSys = true;
    m_switchErr = true;
    m_switchWrn = true;
    m_switchInf = true;
    m_switchDbg = false;

    return true;
}

void LogSystem::Uninitialized(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_default_logger)
    {
        file_logger_flush(m_default_logger);
        destroy_file_logger(m_default_logger);
        m_default_logger = nullptr;
    }

    for (const auto &pair : m_logMapping)
    {
        file_logger_flush(pair.second.Component);
        destroy_file_logger(pair.second.Component);
    }

    m_logMapping.clear();
    uninit_logger_manager();
}

void LogSystem::OnUpdate(void)
{
    FUNC_PERFORMANCE_CHECK();

    std::vector<std::string> destroy_list;
    uint64_t cur_time = get_time();

    for (const auto &pair : m_logMapping)
    {
        if (cur_time - pair.second.LastTime >= 60)
        {
            destroy_list.push_back(pair.first);
        }
    };

    for (const auto &log : destroy_list)
    {
        std::map<std::string, HFILELOGGEREX>::iterator 
            ite = m_logMapping.find(log);
        if (ite == m_logMapping.end())
        {
            continue;
        }

        destroy_file_logger((*ite).second.Component);
        m_logMapping.erase(ite);
    }
}

void LogSystem::StartFlushTimer(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_delay_caller)
    {
        return;
    }

    m_delay_caller = sTimerSystem.AddDelayCaller(60 * 1000, -1, this, &LogSystem::OnUpdate);
}

void LogSystem::StopFlushTimer(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_delay_caller)
    {
        sTimerSystem.DelDelayCaller(m_delay_caller);
        m_delay_caller = nullptr;
    }
}

HFILELOGGER LogSystem::GetDefaultLogger(void)
{
    FUNC_PERFORMANCE_CHECK();

    return m_default_logger;
}

HFILELOGGEREX& LogSystem::GetSpecifiedLogger(const std::string& specified_log_name)
{
    FUNC_PERFORMANCE_CHECK();

    std::map<std::string, HFILELOGGEREX>::iterator ite = m_logMapping.find(specified_log_name);
    if (ite != m_logMapping.end())
    {
        return (*ite).second;
    }

    std::string full_path = m_log_dir + u8"/";
    std::string new_file_name = u8"";

    size_t p1 = specified_log_name.rfind(u8"\\");
    size_t p2 = specified_log_name.rfind(u8"/");
    size_t last_split = 0;

    if (p1 != std::string::npos)
    {
        if (p1 > last_split)
        {
            last_split = p1;
        }
    }

    if (p2 != std::string::npos)
    {
        if (p2 > last_split)
        {
            last_split = p2;
        }
    }

    if (last_split)
    {
        full_path += specified_log_name.substr(0, last_split + 1);
        new_file_name = specified_log_name.substr(last_split + 1);
    }
    else
    {
        new_file_name = specified_log_name;
    }

    std::string utf8_full_path;

    if (!AppUtil::DefaultToUtf8(full_path, utf8_full_path))
    {
        LogPrinf(u8"'{}' log path to utf8  fail.\n", full_path);
    }

    HFILELOGGEREX& component_ex = m_logMapping[specified_log_name];
    component_ex.Component = create_file_logger(utf8_full_path.c_str(), new_file_name.c_str());
    component_ex.LastTime = get_time();

    return component_ex;
}

void LogSystem::DestroySpecifiedLogger(const std::string& specified_log_name)
{
    FUNC_PERFORMANCE_CHECK();

    auto it_log = m_logMapping.find(specified_log_name);

    if (it_log != m_logMapping.end())
    {
        destroy_file_logger(it_log->second.Component);
        m_logMapping.erase(it_log);
    }
}

bool LogSystem::IsSysEnabled(void)
{
    return m_switchSys;
}

bool LogSystem::IsErrEnabled(void)
{
    return m_switchErr;
}

bool LogSystem::IsWrnEnabled(void)
{
    return m_switchWrn;
}

bool LogSystem::IsInfEnabled(void)
{
    return m_switchInf;
}

bool LogSystem::IsDbgEnabled(void)
{
    return m_switchDbg;
}

void LogSystem::EnableSys(bool enable)
{
    m_switchSys = enable;
}

void LogSystem::EnableErr(bool enable)
{
    m_switchErr = enable;
}

void LogSystem::EnableWrn(bool enable)
{
    m_switchWrn = enable;
}

void LogSystem::EnableInf(bool enable)
{
    m_switchInf = enable;
}

void LogSystem::EnableDbg(bool enable)
{
    m_switchDbg = enable;
}

