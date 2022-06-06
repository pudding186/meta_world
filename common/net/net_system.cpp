#include "utility.hpp"
#include "net_system.hpp"
#include "session.hpp"

NetSystem::NetSystem()
    :
    m_netComponent(nullptr)
{

}

NetSystem::~NetSystem()
{

}

bool NetSystem::Initialize(uint32_t max_socket_num, uint32_t max_io_thread_num, uint32_t max_accept_ex_num, uint32_t max_event_num)
{
    FUNC_PERFORMANCE_CHECK();

    m_netComponent = create_net_tcp
    (
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        max_socket_num,
        max_io_thread_num,
        max_accept_ex_num,
        max_event_num
    );
    if (!m_netComponent)
    {
        return false;
    }

    return true;
}

void NetSystem::Uninitialized(void)
{
    FUNC_PERFORMANCE_CHECK();

    if (m_netComponent)
    {
        destroy_net_tcp(m_netComponent);
        m_netComponent = nullptr;
    }
}

bool NetSystem::OnUpdate(CFuncPerformanceInfo** info)
{
    FUNC_PERFORMANCE_CHECK();
    *info = &s_func_perf_info;
    return net_tcp_run(m_netComponent, 0);
}

