#include "utility.hpp"
#include "timer_system.hpp"


//class UpdateTimeTimer
//    :
//    public ITimer
//{
//public:
//    UpdateTimeTimer()
//    {
//        SetInterval(1000);
//        SetCount(-1);
//    }
//
//    ~UpdateTimeTimer()
//    {
//
//    }
//
//public:
//    void OnTimer(void) override
//    {
//        sTimeSystem.OnUpdate();
//    }
//};
//
//UpdateTimeTimer* g_update_timer = 0;

//INSTANCE_SINGLETON(TimerSystem);
TimerSystem::TimerSystem()
    :
    m_timer_manager(nullptr)
{

}

TimerSystem::~TimerSystem()
{

}

bool TimerSystem::Initialize(void)
{
    FUNC_PERFORMANCE_CHECK();

    m_timer_manager = create_timer_manager(OnTimer, get_tick);
    if (!m_timer_manager)
    {
        return false;
    }

    return true;
}

void TimerSystem::Uninitialized(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_timer_manager)
    {
        destroy_timer_manager(m_timer_manager);
        m_timer_manager = nullptr;
    }
}

void TimerSystem::OnUpdate(CFuncPerformanceInfo** info)
{
    FUNC_PERFORMANCE_CHECK();
    *info = &s_func_perf_info;

    timer_update(m_timer_manager, 5);
}

bool TimerSystem::AddTimer(ITimer* timer, uint32_t elapse, int32_t count)
{
    FUNC_PERFORMANCE_CHECK();

    if (!timer)
    {
        return false;
    }

    if (timer->GetTimerInfo())
    {
        if (timer == timer_get_data(timer->GetTimerInfo()))
        {
            DelTimer(timer);
        }
    }

    if (count > 0)
    {
        count++;
    }

    HTIMERINFO h = timer_add(
        m_timer_manager,
        elapse,
        count,
        timer
    );

    if (!h)
    {
        return false;
    }

    timer->SetTimerInfo(h);

    return true;
}

bool TimerSystem::ModifyTimer(ITimer* timer, uint32_t elapse, int32_t count)
{
    FUNC_PERFORMANCE_CHECK();

    if (!timer)
    {
        return false;
    }

    if (count > 0)
    {
        count++;
    }

    if (timer->GetTimerInfo() &&
        timer == timer_get_data(timer->GetTimerInfo()))
    {
        timer_mod(timer->GetTimerInfo(), elapse, count, timer);

        return true;
    }
    else
    {
        return AddTimer(timer, elapse, count);
    }
}

void TimerSystem::DelTimer(ITimer *timer)
{
    FUNC_PERFORMANCE_CHECK();

    if (!timer ||
        !timer->GetTimerInfo())
    {
        return;
    }

    if (timer == timer_get_data(timer->GetTimerInfo()))
    {
        timer_mod(timer->GetTimerInfo(), 1, 1, timer);
    }
}

int32_t TimerSystem::GetRemainCount(ITimer *timer)
{
    FUNC_PERFORMANCE_CHECK();

    if (!timer ||
        !timer->GetTimerInfo())
    {
        return 0;
    }

    if (timer == timer_get_data(timer->GetTimerInfo()))
    {
        return timer_remain_count(timer->GetTimerInfo());
    }

    return 0;
}

bool TimerSystem::ModDelayCaller(DelayCaller* caller, uint32_t elapse, int32_t count)
{
    FUNC_PERFORMANCE_CHECK();

    return ModifyTimer(caller, elapse, count);
}

void TimerSystem::DelDelayCaller(DelayCaller* caller)
{
    FUNC_PERFORMANCE_CHECK();

    if (!caller)
    {
        return;
    }

    DelTimer(caller);
}

void TimerSystem::OnTimer(HTIMERINFO timer)
{
    FUNC_PERFORMANCE_CHECK();

    ITimer *t = static_cast<ITimer*>(timer_get_data(timer));

    if (t == timer_get_data(t->GetTimerInfo()))
    {
        t->OnTimer();
    }
}
