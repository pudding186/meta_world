#ifndef OS_UTIL_H
#define OS_UTIL_H

#ifdef _MSC_VER
#include <conio.h>

typedef unsigned int PID;

#define _chmod(x)

#elif __GNUC__

#include <sys/types.h>
#include <kbhit.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SetConsoleOutputCP(x)
#define _stricmp strcasecmp
#define gets_s(cmd) scanf(u8"%s", cmd)
#define Sleep(x) usleep((x) * 1000)
#define fopen_s(f, n, m)\
[&]()\
{\
    *f = fopen(n, m); \
    if(*f) return 0;\
    return errno; \
}();
#define localtime_s(s, t) localtime_r(t, s)

typedef pid_t PID;
#else
#error "unknown compiler"
#endif

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned long long
        size,
        resident;
}os_util_proc_mem;

extern int os_util_proc_mem_get(PID pid, os_util_proc_mem* proc_mem);

#ifdef  __cplusplus
}
#endif

#endif