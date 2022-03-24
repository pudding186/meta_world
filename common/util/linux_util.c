#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../../libsvr/include/utility.hpp"
#include "os_util.h"

int os_util_proc_mem_get(PID pid, os_util_proc_mem* proc_mem)
{
    char proc_mem_path[64];
    char proc_statm[512];

    if (pid)
    {
        snprintf(proc_mem_path, sizeof(proc_mem_path), "/proc/%d/statm", pid);
    }
    else
    {
        snprintf(proc_mem_path, sizeof(proc_mem_path), "/proc/self/statm");
    }

    int fd = open(proc_mem_path, O_RDONLY);

    if (fd < 0)
    {
        return ENOENT;
    }

    int len = 0;
    int ret = 0;

    if ((len = read(fd, proc_statm, sizeof(proc_statm))) < 0)
    {
        ret = errno;
    }
    else
    {
        mem_seg info[16];
        size_t info_size = split_mem_to_segments(proc_statm, len, " ", 1, info, 16);

        if (info_size >= 2)
        {
            long sz = sysconf(_SC_PAGESIZE);

            proc_statm[info[0].mem_size] = '\0';
            proc_statm[info[0].mem_size + info[1].mem_size + 1] = '\0';

            proc_mem->size = strtol(info[0].mem, 0, 10) * sz;
            proc_mem->resident = strtol(info[1].mem, 0, 10) * sz;
        }
        else
        {
            ret = -1;
        }
    }
    close(fd);

    return ret;
}