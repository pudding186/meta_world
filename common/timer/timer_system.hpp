#pragma once
#include "utility.hpp"
#include "special_decay_type.hpp"
#include "smemory.hpp"
#include "timer.h"
#include "meyers_singleton.hpp"
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

class DelayCaller :
    public ITimer
{
public:
    DelayCaller() = default;
    ~DelayCaller() = default;

    void OnTimer(void) override
    {
        FUNC_PERFORMANCE_CHECK();

        if (!GetRemainCount())
        {
            S_DELETE(this);
            return;
        }

        if (m_fnFunctor)
        {
            m_fnFunctor();
        }
    }

    template<typename T, typename...Args>
    void SetCallBack(T* cls, void (T::* fn)(Args...), Args&&...args)
    {
        FUNC_PERFORMANCE_CHECK();
        m_fnFunctor = std::bind(fn, cls, std::forward<Args>(args)...);
    }
protected:
private:
    std::function<void()> m_fnFunctor;
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

public:
    template <typename T, typename...Args>
    DelayCaller* AddDelayCaller(
        uint32_t elapse,
        int32_t count,
        T* cls,
        void(T::* fn)(Args...),
        const Args&...args)
    {
        FUNC_PERFORMANCE_CHECK();

        DelayCaller* caller = S_NEW(DelayCaller, 1);
        caller->SetCallBack(cls, fn, (special_decay_type<Args>&&)(args)...);

        if (!AddTimer(caller, elapse, count))
        {
            S_DELETE(caller);
            return nullptr;
        }

        return caller;
    }

    bool ModDelayCaller(DelayCaller* caller, uint32_t elapse, int32_t count);

    void DelDelayCaller(DelayCaller* caller);

public:
    static void OnTimer(HTIMERINFO timer);

private:
    HTIMERMANAGER m_timer_manager;
};

#define sTimerSystem MeyersSingleton<TimerSystem>::Instance()