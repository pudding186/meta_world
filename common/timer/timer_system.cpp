#include "utility.hpp"
#include "timer_system.hpp"

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
        timer_del(timer->GetTimerInfo());
    }

    timer->SetTimerInfo(nullptr);
}

void TimerSystem::OnTimer(HTIMERINFO timer)
{
    FUNC_PERFORMANCE_CHECK();

    ITimer* t = static_cast<ITimer*>(timer_get_data(timer));

    if (t == timer_get_data(t->GetTimerInfo()))
    {
        t->OnTimer();
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

std::string TimerSystem::IsValidCronExpr(const std::string& cron_expr)
{
    FUNC_PERFORMANCE_CHECK();

    try
    {
        auto cron = cron::make_cron(cron_expr);
    }
    catch (cron::bad_cronexpr const& ex)
    {
        return ex.what();
    }

    return "";
}

void TimerSystem::DestroyCaller(ITimer* caller)
{
    FUNC_PERFORMANCE_CHECK();

    DelTimer(caller);

    S_DELETE(caller);
}

bool CronCaller::Refresh(void)
{
    std::time_t now = std::time(0);

    return sTimerSystem.ModifyTimer(
        this,
        static_cast<uint32_t>((cron::cron_next(m_cronexpr, now) - now) * 1000),
        1);
}
