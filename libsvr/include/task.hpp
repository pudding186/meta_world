#pragma once

#include "smemory.hpp"
#include "lock_free_queue.hpp"

#include <windows.h>
#include <thread>
#include <queue>

//class TaskProc;
//class ITask;
//
//class TaskData
//{
//public:
//    TaskData(ITask* task) :m_task(task)
//    {
//        ZeroMemory(&m_data.over_lapped, sizeof(m_data.over_lapped));
//    }
//protected:
//private:
//    ITask*      m_task;
//    OVERLAPPED  m_over_lapped;
//    
//};

class ITask
{
public:

    virtual void OnProc(std::thread::id t_id) = 0;
    virtual void OnResult() = 0;
};

class SyncTaskProc
{
public:
    SyncTaskProc(size_t task_queue_size)

    bool AddTask(ITask* task)
    {
        return m_req_loop_queue.push(task);
    }

    ITask* PopTask()
    {
        ITask* task;
        if (m_ack_loop_queue.pop(task))
        {
            return task;
        }

        return nullptr;
    }

protected:

    void _DoTask()
    {
        for (;;)
        {
            ITask* task;

            if (!m_req_loop_queue.pop(task))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }

            if (!task)
            {
                return;
            }

            task->OnProc(std::this_thread::get_id());

            if (m_ack_cache_queue.empty())
            {
                if (!m_ack_loop_queue.push(task))
                {
                    m_ack_cache_queue.push(task);
                }
            }
            else
            {
                m_ack_cache_queue.push(task);

                while (!m_ack_cache_queue.empty())
                {
                    task = m_ack_cache_queue.front();

                    if (m_ack_loop_queue.push(task))
                    {
                        m_ack_cache_queue.pop();
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

private:
    std::thread m_thread;
    LockFreeLoopQueue<ITask*, false, false> m_req_loop_queue;
    LockFreeLoopQueue<ITask*, false, false> m_ack_loop_queue;
    std::queue<ITask*> m_ack_cache_queue;
};