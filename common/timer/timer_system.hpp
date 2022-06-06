#pragma once
#include "utility.hpp"
#include "smemory.hpp"
#include "timer.h"
#include "meyers_singleton.hpp"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996)
#pragma warning( disable : 4505)
#include "croncpp/croncpp.h"
#pragma warning( pop )
#elif __GNUC__
#include "croncpp/croncpp.h"
#else
#error "unknown compiler"
#endif


#include <functional>

class TimerSystem;

class ITimer
{
    friend TimerSystem;
public:
    ITimer():m_timer(nullptr){}
    virtual ~ITimer(){}
public:
    virtual void OnTimer(void) = 0;

    inline HTIMERINFO GetTimerInfo() { return m_timer; }
    inline int32_t GetRemainCount() { return timer_remain_count(m_timer); }
protected:
private:
    inline void SetTimerInfo(HTIMERINFO info) { m_timer = info; }
    HTIMERINFO m_timer;
};

class TimeCaller :
    public ITimer
{
public:

    TimeCaller(const std::function<void()>& call_back):m_fnFunctor(call_back){}

    ~TimeCaller() = default;

    void OnTimer(void) override
    {
        FUNC_PERFORMANCE_CHECK();

        if (m_fnFunctor)
        {
            m_fnFunctor();
        }
    }
protected:
    std::function<void()> m_fnFunctor;
};

class DelayCaller :
    public TimeCaller
{
public:
    DelayCaller(const std::function<void()>& call_back) :TimeCaller(call_back) {}
    ~DelayCaller() = default;

    void OnTimer(void) override
    {
        FUNC_PERFORMANCE_CHECK();

        if (this->m_fnFunctor)
        {
            this->m_fnFunctor();
        }

        if (!this->GetRemainCount())
        {
            S_DELETE(this);
            return;
        }
    }
protected:
private:
};

class CronCaller :
    public TimeCaller
{
public:
    CronCaller(const std::function<void()>& call_back) :TimeCaller(call_back) {}

    ~CronCaller() = default;

    void OnTimer(void) override
    {
        FUNC_PERFORMANCE_CHECK();

        if (this->m_fnFunctor)
        {
            this->m_fnFunctor();
        }

        Refresh();
    }

    bool Init(const std::string& cron_expr)
    {
        try
        {
            m_cronexpr = cron::make_cron(cron_expr);
        }
        catch (cron::bad_cronexpr const& ex)
        {
            (void)ex;
            return false;
        }

        return true;
    }

    bool Refresh(void);
protected:
private:
    cron::cronexpr m_cronexpr;
};


class TimerSystem
{
public:
    TimerSystem();

    ~TimerSystem();

public:  
    bool Initialize(void);

    void Uninitialized(void);

    void OnUpdate(CFuncPerformanceInfo** info);

public:
    bool AddTimer(ITimer *timer, uint32_t elapse, int32_t count);

    bool ModifyTimer(ITimer *timer, uint32_t elapse, int32_t count);

    void DelTimer(ITimer *timer);

    int32_t GetRemainCount(ITimer *timer);

    std::string IsValidCronExpr(const std::string& cron_expr);
public:

    TimeCaller* CreateTimeCaller(
        uint32_t elapse,
        int32_t count,
        const std::function<void()>& call_back
    )
    {
        FUNC_PERFORMANCE_CHECK();

        TimeCaller* time_caller = S_NEW(TimeCaller, 1, call_back);

        if (!AddTimer(time_caller, elapse, count))
        {
            S_DELETE(time_caller);
            return nullptr;
        }

        return time_caller;
    }

    CronCaller* CreateCronCaller(
        const std::string& cron_expr, 
        const std::function<void()>& call_back)
    {
        FUNC_PERFORMANCE_CHECK();

        CronCaller* cron_caller = S_NEW(CronCaller, 1, call_back);

        if (!cron_caller->Init(cron_expr))
        {
            S_DELETE(cron_caller);
            return nullptr;
        }

        if (!cron_caller->Refresh())
        {
            S_DELETE(cron_caller);
            return nullptr;
        }

        return cron_caller;
    }

    void DestroyCaller(ITimer* caller);

    bool DelayCall(
        uint32_t elapse,
        int32_t count,
        const std::function<void()>& call_back)
    {
        FUNC_PERFORMANCE_CHECK();

        DelayCaller* delay_caller = S_NEW(DelayCaller, 1, call_back);

        if (!AddTimer(delay_caller, elapse, count))
        {
            S_DELETE(delay_caller);
            return false;
        }

        return true;
    }

public:
    static void OnTimer(HTIMERINFO timer);

private:
    HTIMERMANAGER m_timer_manager;
};

#define sTimerSystem MeyersSingleton<TimerSystem>::Instance()


#define LOCAL_TO_UTC(time) ((time)?local_to_utc(time):0)
#define UTC_TO_LOCAL(time) ((time)?utc_to_local(time):0)