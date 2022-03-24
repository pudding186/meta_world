#pragma once
#include "logger.hpp"
#include "meyers_singleton.hpp"
#include <map>
#include <string>

class DelayCaller;

enum class LogLevel : unsigned int
{
    LogLevelSys = 0x00,
    LogLevelErr,
    LogLevelWrn,
    LogLevelInf,
    LogLevelDbg,
    LogLevelMax
};

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

    bool IsSysEnabled(void);

    bool IsErrEnabled(void);

    bool IsWrnEnabled(void);

    bool IsInfEnabled(void);

    bool IsDbgEnabled(void);

public:
    void EnableSys(bool enable);
    void EnableErr(bool enable);
    void EnableWrn(bool enable);
    void EnableInf(bool enable);
    void EnableDbg(bool enable);

private:
    std::string m_log_dir;
    HFILELOGGER m_default_logger;
    DelayCaller* m_delay_caller;
    std::map<std::string, HFILELOGGEREX> m_logMapping;

    bool m_switchSys;
    bool m_switchErr;
    bool m_switchWrn;
    bool m_switchInf;
    bool m_switchDbg;
};

#define sLogSystem MeyersSingleton<LogSystem>::Instance()

#ifdef _DEBUG
#define LogSysImpl(c, f, ...) do {if (sLogSystem.IsSysEnabled()) {file_logger_log(c, log_sys, u8"{}", fmt::format(FMT_STRING(f), ##__VA_ARGS__));}} while (0)
#define LogErrImpl(c, f, ...) do {if (sLogSystem.IsErrEnabled()) {file_logger_log(c, log_cri, u8"{}", fmt::format(FMT_STRING(f), ##__VA_ARGS__));}} while (0)
#define LogWrnImpl(c, f, ...) do {if (sLogSystem.IsWrnEnabled()) {file_logger_log(c, log_wrn, u8"{}", fmt::format(FMT_STRING(f), ##__VA_ARGS__));}} while (0)
#define LogInfImpl(c, f, ...) do {if (sLogSystem.IsInfEnabled()) {file_logger_log(c, log_inf, u8"{}", fmt::format(FMT_STRING(f), ##__VA_ARGS__));}} while (0)
#define LogDbgImpl(c, f, ...) do {if (sLogSystem.IsDbgEnabled()) {file_logger_log(c, log_dbg, u8"{}", fmt::format(FMT_STRING(f), ##__VA_ARGS__));}} while (0)
#define LogNulImpl(c, f, ...) do {if (sLogSystem.IsDbgEnabled()) {file_logger_log(c, log_nul, u8"{}", fmt::format(FMT_STRING(f), ##__VA_ARGS__));}} while (0)
//#define LogSysImpl(c, f, ...) do {if (sLogSystem.IsSysEnabled()) {file_logger_log(c, log_sys, f, ##__VA_ARGS__);}} while (0)
//#define LogErrImpl(c, f, ...) do {if (sLogSystem.IsErrEnabled()) {file_logger_log(c, log_cri, f, ##__VA_ARGS__);}} while (0)
//#define LogWrnImpl(c, f, ...) do {if (sLogSystem.IsWrnEnabled()) {file_logger_log(c, log_wrn, f, ##__VA_ARGS__);}} while (0)
//#define LogInfImpl(c, f, ...) do {if (sLogSystem.IsInfEnabled()) {file_logger_log(c, log_inf, f, ##__VA_ARGS__);}} while (0)
//#define LogDbgImpl(c, f, ...) do {if (sLogSystem.IsDbgEnabled()) {file_logger_log(c, log_dbg, f, ##__VA_ARGS__);}} while (0)
//#define LogNulImpl(c, f, ...) do {if (sLogSystem.IsDbgEnabled()) {file_logger_log(c, log_nul, f, ##__VA_ARGS__);}} while (0)
#else
#define LogSysImpl(c, f, ...) do {if (sLogSystem.IsSysEnabled()) {file_logger_log(c, log_sys, f, ##__VA_ARGS__);}} while (0)
#define LogErrImpl(c, f, ...) do {if (sLogSystem.IsErrEnabled()) {file_logger_log(c, log_cri, f, ##__VA_ARGS__);}} while (0)
#define LogWrnImpl(c, f, ...) do {if (sLogSystem.IsWrnEnabled()) {file_logger_log(c, log_wrn, f, ##__VA_ARGS__);}} while (0)
#define LogInfImpl(c, f, ...) do {if (sLogSystem.IsInfEnabled()) {file_logger_log(c, log_inf, f, ##__VA_ARGS__);}} while (0)
#define LogDbgImpl(c, f, ...) do {if (sLogSystem.IsDbgEnabled()) {file_logger_log(c, log_dbg, f, ##__VA_ARGS__);}} while (0)
#define LogNulImpl(c, f, ...) do {if (sLogSystem.IsDbgEnabled()) {file_logger_log(c, log_nul, f, ##__VA_ARGS__);}} while (0)
#endif

#define LogSys(f, ...) LogSysImpl(sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)
#define LogErr(f, ...) LogErrImpl(sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)
#define LogWrn(f, ...) LogWrnImpl(sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)
#define LogInf(f, ...) LogInfImpl(sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)
#define LogDbg(f, ...) LogDbgImpl(sLogSystem.GetDefaultLogger(), f, ##__VA_ARGS__)
#define LogPrinf(f, ...) fmt::print(f, ##__VA_ARGS__)
#define LogFlush() file_logger_flush(sLogSystem.GetDefaultLogger())
#define LogReset() reset_file_logger(sLogSystem.GetDefaultLogger())

#define LogSysEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogSysImpl(info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogErrEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogErrImpl(info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogWrnEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogWrnImpl(info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogInfEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogInfImpl(info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogDbgEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogDbgImpl(info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogFileEx(n, f, ...) do {HFILELOGGEREX &info = sLogSystem.GetSpecifiedLogger(n);LogNulImpl(info.Component, f, ##__VA_ARGS__);info.LastTime = get_time();} while (0)
#define LogFlushEx(f, ...) file_logger_flush(sLogSystem.GetSpecifiedLogger(fmt::format(f, ##__VA_ARGS__)).Component)
#define LogResetEx(f, ...) reset_file_logger(sLogSystem.GetSpecifiedLogger(fmt::format(f, ##__VA_ARGS__)).Component)

#ifdef _DEBUG
#define StrFormat(f, ...) fmt::format(FMT_STRING(f), ##__VA_ARGS__)
#else
#define StrFormat(f, ...) fmt::format(f, ##__VA_ARGS__)
#endif


#ifdef _MSC_VER
#define LogSysGB(f, ...) do {if (sLogSystem.IsSysEnabled()) {std::string utf8_f; AppUtil::MultiByteToOther(CP_ACP, fmt::format(FMT_STRING(f), ##__VA_ARGS__), CP_UTF8, utf8_f);file_logger_log(sLogSystem.GetLogComponent(), log_sys, u8"{}", utf8_f);}} while (0)
#define LogErrGB(f, ...) do {if (sLogSystem.IsErrEnabled()) {std::string utf8_f; AppUtil::MultiByteToOther(CP_ACP, fmt::format(FMT_STRING(f), ##__VA_ARGS__), CP_UTF8, utf8_f);file_logger_log(sLogSystem.GetLogComponent(), log_cri, u8"{}", utf8_f);}} while (0)
#define LogWrnGB(f, ...) do {if (sLogSystem.IsWrnEnabled()) {std::string utf8_f; AppUtil::MultiByteToOther(CP_ACP, fmt::format(FMT_STRING(f), ##__VA_ARGS__), CP_UTF8, utf8_f);file_logger_log(sLogSystem.GetLogComponent(), log_wrn, u8"{}", utf8_f);}} while (0)
#define LogInfGB(f, ...) do {if (sLogSystem.IsInfEnabled()) {std::string utf8_f; AppUtil::MultiByteToOther(CP_ACP, fmt::format(FMT_STRING(f), ##__VA_ARGS__), CP_UTF8, utf8_f);file_logger_log(sLogSystem.GetLogComponent(), log_inf, u8"{}", utf8_f);}} while (0)
#define LogDbgGB(f, ...) do {if (sLogSystem.IsDbgEnabled()) {std::string utf8_f; AppUtil::MultiByteToOther(CP_ACP, fmt::format(FMT_STRING(f), ##__VA_ARGS__), CP_UTF8, utf8_f);file_logger_log(sLogSystem.GetLogComponent(), log_dbg, u8"{}", utf8_f);}} while (0)
#define LogPrinfGB(f, ...) do {std::string utf8_f; AppUtil::MultiByteToOther(CP_ACP, fmt::format(FMT_STRING(f), ##__VA_ARGS__), CP_UTF8, utf8_f); fmt::print("{}", utf8_f);} while (0)//LogPrinf(f, ##__VA_ARGS__)
#elif __GNUC__
#define LogSysGB(f, ...) LogSys(f, ##__VA_ARGS__)
#define LogErrGB(f, ...) LogErr(f, ##__VA_ARGS__)
#define LogWrnGB(f, ...) LogWrn(f, ##__VA_ARGS__)
#define LogInfGB(f, ...) LogInf(f, ##__VA_ARGS__)
#define LogDbgGB(f, ...) LogDbg(f, ##__VA_ARGS__)
#define LogPrinfGB(f, ...) LogPrinf(f, ##__VA_ARGS__)
#else
#error "unknown compiler"
#endif