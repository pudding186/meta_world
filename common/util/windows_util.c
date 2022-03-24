#include <Windows.h>
#include <Psapi.h>
#include "os_util.h"

int os_util_proc_mem_get(PID pid, os_util_proc_mem* proc_mem)
{
    PROCESS_MEMORY_COUNTERS cnt;

    HANDLE proc = NULL;
    HANDLE cur_proc = NULL;

    int last_error = 0;

    if (pid == 0)
    {
        cur_proc = GetCurrentProcess();
        proc = cur_proc;
    }
    else
    {
        proc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    }

    if (!proc)
    {
        return (int)GetLastError();
    }

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    size_t size = 1024 * 1024;
    char* buf = (char*)malloc(size);
    memset(buf, 0, size);

    proc_mem->resident = 0;

    for (;;)
    {
        BOOL ret = QueryWorkingSet(proc, buf, (DWORD)size);
        PSAPI_WORKING_SET_INFORMATION* wt = (PSAPI_WORKING_SET_INFORMATION*)(buf);

        if (ret)
        {    
            PSAPI_WORKING_SET_BLOCK* pFirst = (PSAPI_WORKING_SET_BLOCK*)(buf + sizeof(PSAPI_WORKING_SET_INFORMATION));

            for (ULONG_PTR mem_entry_cnt = 0; mem_entry_cnt < wt->NumberOfEntries; mem_entry_cnt++, pFirst++)
            {
                if (!pFirst->Shared)
                {
                    proc_mem->resident += si.dwPageSize;
                }
            }

            last_error = 0;
            break;
        }
        else
        {
            last_error = GetLastError();
            if (last_error == ERROR_BAD_LENGTH)
            {
                size += wt->NumberOfEntries * sizeof(PSAPI_WORKING_SET_BLOCK);
                free(buf);
                buf = (char*)malloc(size);
                memset(buf, 0, size);
            }
            else
            {
                break;
            }
        }
    }

    if (GetProcessMemoryInfo(proc, &cnt, sizeof(cnt)))
    {
        proc_mem->size = cnt.PagefileUsage;
    }
    else
    {
        last_error = (int)GetLastError();
    }

    free(buf);

    if (!cur_proc)
    {
        CloseHandle(proc);
    }

    return last_error;
}