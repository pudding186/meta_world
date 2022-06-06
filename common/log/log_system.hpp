#pragma once
#include "logger.hpp"
#include "meyers_singleton.hpp"
#include <map>
#include <string>

class ITimer;

struct HFILELOGGEREX
{
    HFILELOGGER Component;
    unsigned long long LastTime;
};

class LogSystem
{
public:
    LogSystem();

    ~LogSystem();

    static bool IsOk(void);

public:
    bool Initialize(size_t log_thread_num, size_t max_log_event_num, size_t print_cache_size);

    bool CreateDefaultLogger(const std::string& log_dir, const std::string& default_log_name);

    void Uninitialized(void);

    void OnUpdate(void);

public:
    void StartFlushTimer(void);

    void StopFlushTimer(void);

public:
    HFILELOGGER GetDefaultLogger(void);

    HFILELOGGEREX &GetSpecifiedLogger(const std::string &specified_log_name);

    void DestroySpecifiedLogger(const std::string& specified_log_name);

private:
    std::string m_log_dir;
    HFILELOGGER m_default_logger;
    ITimer* m_delay_caller;
    std::map<std::string, HFILELOGGEREX> m_logMapping;
};

#define sLogSystem MeyersSingleton<LogSystem>::Instance()

#define LogPrinf(f, ...) fmt::print(f, ##__VA_ARGS__)

#ifdef _DEBUG
#define LogImpl(p, l, f, ...) do{if (LoggerPrefix##p::Enable()) {file_logger_log<LoggerPrefix##p>(l, u8"{}", fmt::format(FMT_STRING(f), ##__VA_ARGS__));}} while (0)
#else
#define LogImpl(p, l, f, ...) do{if (LoggerPrefix##p::Enable()) {file_logger_log<LoggerPrefix##p>(l, f, ##__VA_ARGS__);}} while (0)
#endif

DECLEAR_LOG_PREFIX(SYS, print_color::green)
#define LogSYS(f, ...) LogImpl(SYS, sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)

DECLEAR_LOG_PREFIX(ERR, print_color::red)
#define LogERR(f, ...) LogImpl(ERR, sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)

DECLEAR_LOG_PREFIX(CRI, print_color::pink)
#define LogCRI(f, ...) LogImpl(CRI, sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)

DECLEAR_LOG_PREFIX(WRN, print_color::yellow)
#define LogWRN(f, ...) LogImpl(WRN, sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)

DECLEAR_LOG_PREFIX(INF, print_color::blue)
#define LogINF(f, ...) LogImpl(INF, sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)

DECLEAR_LOG_PREFIX(DBG, print_color::white)
#define LogDBG(f, ...) LogImpl(DBG, sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)

#define LogFlush() file_logger_flush(sLogSystem.GetDefaultLogger())
#define LogReset() reset_file_logger(sLogSystem.GetDefaultLogger())

#define LogSYSEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogImpl(SYS, info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogERREx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogImpl(ERR, info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogCRIEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogImpl(CRI, info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogWRNEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogImpl(WRN, info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogINFEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogImpl(INF, info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogDBGEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogImpl(DBG, info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)

#define LogFlushEx(f, ...) file_logger_flush(sLogSystem.GetSpecifiedLogger(fmt::format(f, ##__VA_ARGS__)).Component)
#define LogResetEx(f, ...) reset_file_logger(sLogSystem.GetSpecifiedLogger(fmt::format(f, ##__VA_ARGS__)).Component)

#ifdef _DEBUG
#define StrFormat(f, ...) fmt::format(FMT_STRING(f), ##__VA_ARGS__)
#else
#define StrFormat(f, ...) fmt::format(f, ##__VA_ARGS__)
#endif
