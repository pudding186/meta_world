#pragma once

#include <thread>
#include <atomic>
#include <string>
#include <stdint.h>

#include "os_util.h"
#include "timer_system.hpp"
#include "log_system.hpp"
#include "net_system.hpp"
#include "dump_system.hpp"
#include "common/util/app_util.hpp"

enum class SvrAppState :int32_t {
    NONE = 0,
    START,
    INIT,
    RUN,
    UNINIT,
    STOP
};

enum class SvrAppStopType :int32_t {
    NONE = 0,
    NORMAL,
    INIT_FAIL,
    CRASH,
    INTERRUPT,
};

enum class AsyncRet :int32_t {
    ASYNC_CONTINUE,
    ASYNC_SUCCESS,
    ASYNC_FAIL,
};

template<typename T>
class SvrApp
{
public:
    // override by Implement
    static inline std::string GetName() { return "Unknow Server"; }
    static inline std::string GetVersion() { return "Unknow version"; }
    static inline uint32_t GetZoneId() { return 0; }

    inline size_t MaxLogThreadNum() { return 1; }
    inline size_t MaxLogEventNum() { return 4096; }
    inline size_t MaxPrintCacheSize() { return 65536; }

    inline uint32_t MaxSocketNum() { return 8; }
    inline uint32_t MaxIOThreadNum() { return 2; }
    inline uint32_t MaxAcceptExNum() { return 4; }
    inline uint32_t MaxEventNum() { return 64 * 1024; }

    inline size_t MaxDBEventNum() { return 256 * 1024; }


    std::string OnInitialize(void) { return u8""; }

    bool OnUpdate(CFuncPerformanceInfo** info) 
    {
        FUNC_PERFORMANCE_CHECK();
        *info = &s_func_perf_info;

        return false; 
    }
    void OnUninitialized(void) {}

    AsyncRet OnAsyncInit(void) { return AsyncRet::ASYNC_SUCCESS; }
    AsyncRet OnAsyncUninit(void) { return AsyncRet::ASYNC_SUCCESS; }

public:
    SvrApp()
    {
        m_svr_state = SvrAppState::NONE;
        m_stop_type = SvrAppStopType::NONE;
        m_async_caller = nullptr;
    }

    virtual ~SvrApp()
    {

    }

    bool Start(void)
    {
        SvrAppState svr_app_state = SvrAppState::NONE;
        if (m_svr_state.compare_exchange_strong(
            svr_app_state,
            SvrAppState::START
        ))
        {
            try
            {
                m_svr_thread = std::thread(&SvrApp<T>::_ThreadProc, this);
            }
            catch (...)
            {
                m_svr_state = SvrAppState::NONE;
                //OnError(e.what());
                return false;
            }
            return true;
        }

        return true;
    }

    void WaitFor(void)
    {
        if (m_svr_thread.get_id() != std::this_thread::get_id())
        {
            if (m_svr_thread.joinable())
            {
                m_svr_thread.join();
            }
        }
    }

    void Stop(SvrAppStopType type)
    {
        SvrAppStopType svr_stop_type = SvrAppStopType::NONE;

        if (m_stop_type.compare_exchange_strong(
            svr_stop_type,
            type
        ))
        {
            std::string stop_msg;
            switch (type)
            {
            case SvrAppStopType::NORMAL:
            {
                stop_msg = fmt::format(u8"{} Stop ...!", T::GetName());
            }
            break;
            case SvrAppStopType::INIT_FAIL:
            {
                stop_msg = fmt::format(u8"{} Stop Init Fail!", T::GetName());
            }
            break;
            case SvrAppStopType::CRASH:
            {
                stop_msg = fmt::format(u8"{} Stop Crash!", T::GetName());
            }
            break;
            case SvrAppStopType::INTERRUPT:
            {
                stop_msg = fmt::format(u8"{} Stop Interrupt!", T::GetName());
            }
            break;
            default:
            {
                stop_msg = fmt::format(u8"{} Stop Unknow Type:{}!", T::GetName(), static_cast<int32_t>(type));
            }
            }
            if (sLogSystem.GetDefaultLogger())
            {
                LogWRN(u8"{}", stop_msg);
            }
            else
            {
                LogPrinf(u8"{}", stop_msg);
            }
        }
    }

    std::string Initialize(void)
    {
        if (!sTimerSystem.Initialize())
        {
            return u8"timer system init fail !";
        }

        SetState(SvrAppState::INIT);

        if (!sLogSystem.Initialize(
            static_cast<T*>(this)->MaxLogThreadNum(),
            static_cast<T*>(this)->MaxLogEventNum(),
            static_cast<T*>(this)->MaxPrintCacheSize()
        ))
        {
            return u8"logger system init fail !";
        }
        else
        {
            if (!sLogSystem.CreateDefaultLogger(u8"./", T::GetName()))
            {
                return u8"Create Default Logger fail !";
            }

            LogSYS(u8"{:*^30}", fmt::format(u8"{}[ver:{}] Starting...", T::GetName(), T::GetVersion()));
            LogSYS(u8"Time System Init Success!");
            LogSYS(u8"Logger System Init Success!");
        }

        if (!sNetSystem.Initialize(
            static_cast<T*>(this)->MaxSocketNum(),
            static_cast<T*>(this)->MaxIOThreadNum(),
            static_cast<T*>(this)->MaxAcceptExNum(),
            static_cast<T*>(this)->MaxEventNum()
        ))
        {
            return u8"Net System Init Fail!";
        }

        LogSYS(u8"Net System Init Success!");

        return static_cast<T*>(this)->OnInitialize();
    }

    void Uninitialized(void)
    {
        static_cast<T*>(this)->OnUninitialized();

        sNetSystem.Uninitialized();

        LogSYS(u8"{}[ver:{}] Close Success!", T::GetName(), T::GetVersion());

        sLogSystem.Uninitialized();

        sTimerSystem.Uninitialized();
    }

    void Run(void)
    {
        std::string init_ret = Initialize();
        if (init_ret == "")
        {
            uint64_t proc_cycles = 0;
            m_last_check_cycle = get_cycle();
            m_last_check_tick = get_tick();

            while (m_svr_state != SvrAppState::STOP)
            {
                bool busy = false;

                CFuncPerformanceInfo* fpfi = nullptr;

                sTimerSystem.OnUpdate(&fpfi);
                proc_cycles += fpfi->once_cycles;

                if (sNetSystem.OnUpdate(&fpfi))
                {
                    busy = true;
                }
                proc_cycles += fpfi->once_cycles;

                if (static_cast<T*>(this)->OnUpdate(&fpfi))
                {
                    busy = true;
                }
                proc_cycles += fpfi->once_cycles;

                if (get_tick() - m_last_check_tick >= 1000)
                {
                    m_last_elapse_cycles = get_cycle() - m_last_check_cycle;
                    m_last_proc_cycles = proc_cycles;
                    m_last_1_ms_cycles = m_last_elapse_cycles / 1000;

                    m_last_check_cycle = get_cycle();
                    proc_cycles = 0;
                    m_last_check_tick = get_tick();
                }

                if (m_stop_type != SvrAppStopType::NONE)
                {
                    SetState(SvrAppState::UNINIT);
                }

                if (!busy)
                {
                    Sleep(1);
                }
            }
        }
        else
        {
            if (sLogSystem.GetDefaultLogger())
            {
                LogERR(u8"{} Init Fail:{}", T::GetName(), init_ret);
            }
            else
            {
                LogPrinf(u8"{} Init Fail:{}", T::GetName(), init_ret);
            }

            Stop(SvrAppStopType::INIT_FAIL);
        }

        Uninitialized();
    }

    bool IsStopping(void)
    {
        return m_stop_type != SvrAppStopType::NONE;
    }

    void SetState(SvrAppState app_state)
    {
        FUNC_PERFORMANCE_CHECK();

        switch (app_state)
        {
        case SvrAppState::INIT:
        {
            if (m_svr_state != SvrAppState::START)
            {
                return;
            }

            if (m_async_caller)
            {
                sTimerSystem.ModifyTimer(m_async_caller, 100, -1);
            }
            else
            {
                m_async_caller = sTimerSystem.CreateTimeCaller(100, -1, std::bind(&SvrApp<T>::OnState, this));
            }

            m_svr_state = SvrAppState::INIT;
        }
        break;
        case SvrAppState::UNINIT:
        {
            if (m_svr_state != SvrAppState::START &&
                m_svr_state != SvrAppState::INIT &&
                m_svr_state != SvrAppState::RUN)
            {
                return;
            }

            if (m_async_caller)
            {
                sTimerSystem.ModifyTimer(m_async_caller, 1000, -1);
            }
            else
            {
                m_async_caller = sTimerSystem.CreateTimeCaller(1000, -1, std::bind(&SvrApp<T>::OnState, this));
            }

            m_svr_state = SvrAppState::UNINIT;

        }
        break;
        case SvrAppState::RUN:
        {
            if (m_svr_state != SvrAppState::INIT)
            {
                return;
            }

            if (m_async_caller)
            {
                sTimerSystem.DestroyCaller(m_async_caller);
                m_async_caller = nullptr;
            }

            m_svr_state = SvrAppState::RUN;
        }
        break;
        case SvrAppState::STOP:
        {
            if (m_svr_state != SvrAppState::UNINIT)
            {
                return;
            }

            m_svr_state = SvrAppState::STOP;
        }
        break;
        default:
            return;
        }
    }

    void OnState(void)
    {
        FUNC_PERFORMANCE_CHECK();

        switch (m_svr_state.load())
        {
        case SvrAppState::INIT:
        {
            switch (static_cast<T*>(this)->OnAsyncInit())
            {
            case AsyncRet::ASYNC_SUCCESS:
            {
                SetState(SvrAppState::RUN);
                LogSYS(u8"{:*^30}", fmt::format(u8"{}[ver:{}] Working...", T::GetName(), T::GetVersion()));
            }
            break;
            case AsyncRet::ASYNC_FAIL:
            {
                Stop(SvrAppStopType::INIT_FAIL);
                SetState(SvrAppState::UNINIT);
            }
            break;
            default:
                return;
            }
        }
        break;
        case SvrAppState::UNINIT:
        {
            switch (static_cast<T*>(this)->OnAsyncUninit())
            {
            case AsyncRet::ASYNC_SUCCESS:
            case AsyncRet::ASYNC_FAIL:
                SetState(SvrAppState::STOP);
                break;
            default:
                return;
            }
        }
        break;
        default:
            return;
        }
    }
private:

#ifdef _MSC_VER

    static void _OnInvalidParameter(const wchar_t* expression,
        const wchar_t* function,
        const wchar_t* file,
        unsigned int line,
        uintptr_t pReserved)
    {
        (void)expression;
        (void)function;
        (void)file;
        (void)line;
        (void)pReserved;

        throw u8"OnInvalidParameter";
    }

    static void _OnPurecall(void)
    {
        throw u8"OnPurecall";
    }

    static int _OnException(unsigned int code, struct _EXCEPTION_POINTERS* ep)
    {
        (void)code;

        char stack_file_path[MAX_PATH] = { 0 };

        snprintf(stack_file_path, sizeof(stack_file_path), "stack_%s_%s.log", T::GetName().c_str(), T::GetVersion().c_str());
        char buf[8192] = { 0 };
        FuncStackToCache(DefFuncPerf(), buf, sizeof(buf));

        FILE* stack_file = nullptr;

        INT32 err = fopen_s(&stack_file, stack_file_path, "wb");

        if (err != 0 || 0 == stack_file)
        {
            return EXCEPTION_CONTINUE_SEARCH;
        }

        fprintf(stack_file, u8"Version: %s\r\n", T::GetVersion().c_str());
        fprintf(stack_file, u8"[%s] crush:\r\n%s", T::GetName().c_str(), buf);
        fclose(stack_file);

        std::string msg = "Version: " + T::GetVersion() + "\r\n";
        msg += fmt::format("Zone: {}\r\n", T::GetZoneId());
        msg += T::GetName() + " crush:\r\n ";
        msg += buf;

        AppUtil::ReportDingDing(msg);

        if (DumpSystem::GenMiniDump(ep))
        {
            return EXCEPTION_EXECUTE_HANDLER;
        }

        return EXCEPTION_CONTINUE_SEARCH;
    }

    void _ThreadProc(void)
    {
        _set_invalid_parameter_handler(_OnInvalidParameter);
        _set_purecall_handler(_OnPurecall);

        __try
        {
            Run();
        }
        __except (_OnException(GetExceptionCode(), GetExceptionInformation()))
        {
            Stop(SvrAppStopType::CRASH);
        }
    }

#elif __GNUC__

    void _ThreadProc(void)
    {
        Run();
    }
#else
#error u8"unknown compiler"
#endif

private:
    std::thread m_svr_thread;
    std::atomic<SvrAppState> m_svr_state;
    std::atomic<SvrAppStopType> m_stop_type;
    ITimer* m_async_caller;

    uint64_t m_last_proc_cycles;
    uint64_t m_last_elapse_cycles;
    uint64_t m_last_check_cycle;
    uint64_t m_last_1_ms_cycles;
    uint32_t m_last_check_tick;
};
