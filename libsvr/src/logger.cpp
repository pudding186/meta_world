#include "./lib_svr_common_def.h"

#ifdef _MSC_VER
#include <windows.h>
#undef max
#undef min
#elif __GNUC__
#include <limits.h>
#include <endian.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <signal.h>
#else
#error "unknown compiler"
#endif

#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>

#include "../include/utility.hpp"
#include "../include/memory_pool.h"
#include "../include/loop_cache.h"
#include "../include/logger.hpp"
#include "../include/timer.h"

#define MAX_LOG_FILE_PATH       1024
#define MAX_LOG_FILE_SIZE       1024*1024*1024

typedef enum e_log_option
{
    opt_write   = 0x00000001,
    opt_write_c = (0x00000001 << 1),
    opt_flush   = (0x00000001 << 2),
    opt_close   = (0x00000001 << 3),
    opt_open    = (0x00000001 << 4),
    opt_reset   = (0x00000001 << 5),
}log_option;

typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> TPMS;

typedef struct st_print_cmd
{
    //file_logger_level   lv;
    print_color         color;
    size_t              data_len;
}print_cmd;


typedef struct st_log_cmd log_cmd;

typedef struct st_log_queue
{
    HLOOPCACHE  cmd_queue;
    HLOOPCACHE  rcy_queue;
    std::queue<log_cmd*>* rcy_cache_queue;
}log_queue;

//typedef struct st_log_obj_pool
//{
//    struct st_log_obj_pool* next_pool;
//    SMemory::IClassMemory*  obj_pool;
//}log_obj_pool;
//typedef struct st_log_mem_pool
//{
//    struct st_log_mem_pool* next_pool;
//    HMEMORYMANAGER          mem_pool;
//}log_mem_pool;

typedef struct st_log_proc
{
    struct st_log_proc* next_proc;
    log_queue*          queue;
    //log_obj_pool*       log_obj_pool_head;
    //log_mem_pool*       log_mem_pool_head;
    time_t              last_log_time;
    struct tm           last_log_tm;
    char                time_str[32];
    unsigned int        t_id;
    bool                is_run;
}log_proc;

typedef struct st_file_logger
{
    unsigned long long log_req_mt;
    unsigned long long log_req;
    unsigned long long log_ack;
#ifdef _MSC_VER
    wchar_t* path;
    wchar_t* name;
#elif __GNUC__
    char* path;
    char* name;
#else
#error "unknown compiler"
#endif
    FILE* file;
    size_t      log_thread_idx;
    struct tm   file_time;
    size_t      file_size;
    size_t      file_idx;
    log_proc* create_proc;

}file_logger;

typedef struct st_log_cmd
{
    log_option          option;
    file_logger*        logger;
    //file_logger_level   lv;
    print_color         color;
    const char*         prefix;
    SFormatArgs<>*      fmt_args;
    TPMS                tpms;
}log_cmd;

class log_thread
{
public:
    log_thread();

    ~log_thread();

    void start();

    unsigned int _proc_log();

    void _proc_log_end();

    void _log_func();

    bool _check_log(log_cmd* cmd, log_proc* proc, std::string& err_msg);

    bool _reset_log(log_cmd* cmd, log_proc* proc, std::string& err_msg);

    void _do_cmd(log_cmd* cmd, log_proc* proc);

    inline void set_idx(size_t idx) { m_idx = idx; }
    inline unsigned int get_proc_count(void) { return m_do_proc_count; }
    inline std::thread& log_thread_ref(void) { return m_log_thread; }
    inline HLOOPCACHE print_data_cache(void) { return m_print_data_cache; }
    inline bool is_run(void) { return m_is_run; }
protected:
private:
    std::thread     m_log_thread;
    size_t          m_idx;
    unsigned int    m_do_proc_count;
    HLOOPCACHE      m_print_data_cache;
    bool            m_is_run;
};

class print_thread
{
public:
    print_thread();
    ~print_thread();

    void _check_print(print_cmd* cmd);

    void _print_func();

    unsigned int _proc_print();

    void _proc_print_end();

    bool _do_pt_cmd(HLOOPCACHE print_cache);

    inline HLOOPCACHE print_cache(void) { return m_print_cache; }
    
protected:
private:
    std::thread         m_print_thread;
    print_color         m_last_color;
    HLOOPCACHE          m_print_cache;
};

typedef struct st_logger_manager
{
    log_thread*         log_thread_array;
    size_t              log_thread_num;
    size_t              log_queue_size;
    log_proc*           log_proc_head;
    log_proc**          main_log_proc;
    print_thread*       print_thread_pt;
    size_t              print_cache_size;
    bool                is_run;
}logger_manager;

static logger_manager* g_logger_manager = 0;
//static TLS_VAR log_proc* s_log_proc = 0;

class CThreadLogProc
{
public:
    CThreadLogProc(void)
    {
        m_log_proc = (log_proc*)malloc(sizeof(log_proc));
        m_log_proc->queue = (log_queue*)malloc(sizeof(log_queue) * g_logger_manager->log_thread_num);
        //m_log_proc->log_mem_pool_head = 0;
        //m_log_proc->log_obj_pool_head = 0;

        for (size_t i = 0; i < g_logger_manager->log_thread_num; i++)
        {
            m_log_proc->queue[i].cmd_queue = create_loop_cache(0, sizeof(log_cmd*) * g_logger_manager->log_queue_size);
            m_log_proc->queue[i].rcy_queue = create_loop_cache(0, sizeof(log_cmd*) * g_logger_manager->log_queue_size);
            m_log_proc->queue[i].rcy_cache_queue = new std::queue<log_cmd*>;
        }

        m_log_proc->last_log_time = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()));
#ifdef _MSC_VER
        localtime_s(&m_log_proc->last_log_tm, &m_log_proc->last_log_time);
        m_log_proc->t_id = ::GetCurrentThreadId();
#elif __GNUC__
        localtime_r(&m_log_proc->last_log_time, &m_log_proc->last_log_tm);
        m_log_proc->t_id = syscall(__NR_gettid);
#else
#error "unknown compiler"
#endif
        strftime(m_log_proc->time_str, sizeof(m_log_proc->time_str), "%Y-%m-%d %H:%M:%S", &m_log_proc->last_log_tm);

        m_log_proc->is_run = true;

#ifdef _MSC_VER
        do
        {
            m_log_proc->next_proc = g_logger_manager->log_proc_head;
        } while (InterlockedCompareExchangePointer(
            reinterpret_cast<PVOID*>(&(g_logger_manager->log_proc_head)),
            m_log_proc,
            m_log_proc->next_proc) != m_log_proc->next_proc);
#elif __GNUC__
        m_log_proc->next_proc = g_logger_manager->log_proc_head;
        while (!__atomic_compare_exchange(
            &g_logger_manager->log_proc_head,
            &m_log_proc->next_proc,
            &m_log_proc, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
        {
        }
#else
#error "unknown compiler"
#endif
    }

    ~CThreadLogProc(void)
    {
        if (g_logger_manager)
        {
            if (m_log_proc)
            {
                m_log_proc->is_run = false;
            }
        }
    }

    inline log_proc* GetLogProc(void) { return m_log_proc; }

    inline log_proc** GetLogProcAddr(void) { return &m_log_proc; }
protected:
    log_proc* m_log_proc;
private:
};

//void check_log_proc(void)
//{
//    if (g_logger_manager)
//    {
//        if (s_log_proc)
//        {
//            s_log_proc->is_run = false;
//        }
//    }
//}

//////////////////////////////////////////////////////////////////////////

log_thread::log_thread()
{
    m_print_data_cache = create_loop_cache(0, g_logger_manager->print_cache_size);
    m_do_proc_count = 0;
}

log_thread::~log_thread()
{
    m_log_thread.join();

    destroy_loop_cache(m_print_data_cache);
}

void log_thread::start()
{
    m_log_thread = std::thread(&log_thread::_log_func, this);
}

#ifdef _MSC_VER

int mb_to_wc(unsigned int code_page, const char* src, int c_len, wchar_t* dst, int w_size)
{
    int translate_len = MultiByteToWideChar(code_page, 0, src, c_len, dst, w_size);

    if (!translate_len)
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            return MultiByteToWideChar(code_page, 0, src, c_len, 0, 0);
        }

        return 0;
    }

    return translate_len;
}

int wc_to_mb(unsigned int code_page, const wchar_t* src, int w_len, char* dst, int c_size)
{
    int translate_len = WideCharToMultiByte(code_page, 0, src, w_len, dst, c_size, 0, 0);

    if (!translate_len)
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            return WideCharToMultiByte(code_page, 0, src, w_len, 0, 0, 0, 0);
        }

        return 0;
    }

    return translate_len;
}


bool _mk_dir(const wchar_t* dir)
{
    size_t i;
    wchar_t* p1;
    wchar_t* p;
    wchar_t path[MAX_LOG_FILE_PATH] = { 0 };
    wcsncpy_s(path, MAX_LOG_FILE_PATH, dir, _TRUNCATE);
    p1 = path + 1;
    p = path;

    for (i = 0; i < wcslen(path); i++)
    {
        if (*(p + i) == L'\\')
        {
            *(p + i) = L'/';
        }
    }

    do
    {
        p1 = wcschr(p1, L'/');
        if (p1 != 0)
        {
            *p1 = L'\0';
        }

        if (-1 == _wmkdir(path))
        {
            if ((0 == p1) && (errno != EEXIST))
            {
                return false;
            }
        }

        if (p1 != 0)
        {
            *p1++ = L'/';
        }
    } while (p1);

    return true;
}

bool log_thread::_check_log(log_cmd* cmd, log_proc* proc, std::string& err_msg)
{
    wchar_t file_full_path[MAX_LOG_FILE_PATH];

    time_t cur_time = std::chrono::system_clock::to_time_t(cmd->tpms);

    if (!cmd->logger->file)
    {
        localtime_s(&cmd->logger->file_time, &cur_time);

        int file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
            L"%ls/%ls_%04d-%02d-%02d.txt",
            cmd->logger->path, cmd->logger->name,
            cmd->logger->file_time.tm_year + 1900,
            cmd->logger->file_time.tm_mon + 1,
            cmd->logger->file_time.tm_mday);

        if (wcslen(cmd->logger->name) <= 0)
        {
            file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
                L"%ls/%04d-%02d-%02d.txt",
                cmd->logger->path,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday);
        }

        cmd->logger->file = _wfsopen(file_full_path, L"a", _SH_DENYWR);
        if (!cmd->logger->file)
        {
            char sz_file_full_path[MAX_LOG_FILE_PATH];
            int len = wc_to_mb(CP_UTF8, file_full_path, file_full_path_len, sz_file_full_path, MAX_LOG_FILE_PATH);
            sz_file_full_path[len] = 0;
            //fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno);
            //err_msg = sys_error.what();
            err_msg = fmt::system_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno).what();
            err_msg.append("\n");

            return false;
        }

        cmd->logger->file_idx = 0;

        fseek(cmd->logger->file, 0, SEEK_END);
        cmd->logger->file_size = ftell(cmd->logger->file);
    }

    long long time_del = cur_time - proc->last_log_time;

    if (time_del)
    {
        if (time_del < 60 && time_del > -60)
        {
            int new_sec = proc->last_log_tm.tm_sec + (int)time_del;

            if (new_sec >= 0 && new_sec <= 59)
            {
                proc->last_log_tm.tm_sec = new_sec;
            }
            else
            {
                localtime_s(&proc->last_log_tm, &cur_time);
            }
        }
        else
        {
            localtime_s(&proc->last_log_tm, &cur_time);
        }

        proc->last_log_time = cur_time;
        strftime(proc->time_str, sizeof(proc->time_str), "%Y-%m-%d %H:%M:%S", &proc->last_log_tm);

        if (proc->last_log_tm.tm_yday != cmd->logger->file_time.tm_yday)
        {
            cmd->logger->file_time = proc->last_log_tm;

            if (cmd->logger->file)
            {
                fclose(cmd->logger->file);
                cmd->logger->file = 0;
            }

            int file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
                L"%ls/%ls_%04d-%02d-%02d.txt",
                cmd->logger->path, cmd->logger->name,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday);

            if (wcslen(cmd->logger->name) <= 0)
            {
                file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
                    L"%ls/%04d-%02d-%02d.txt",
                    cmd->logger->path,
                    cmd->logger->file_time.tm_year + 1900,
                    cmd->logger->file_time.tm_mon + 1,
                    cmd->logger->file_time.tm_mday);
            }

            cmd->logger->file = _wfsopen(file_full_path, L"a", _SH_DENYWR);
            if (!cmd->logger->file)
            {
                char sz_file_full_path[MAX_LOG_FILE_PATH];
                int len = wc_to_mb(CP_UTF8, file_full_path, file_full_path_len, sz_file_full_path, MAX_LOG_FILE_PATH);
                sz_file_full_path[len] = 0;
                //fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno);
                err_msg = fmt::system_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno).what();
                err_msg.append("\n");
                return false;
            }

            fseek(cmd->logger->file, 0, SEEK_END);
            cmd->logger->file_size = ftell(cmd->logger->file);
        }
    }

    while (cmd->logger->file_size >= MAX_LOG_FILE_SIZE)
    {
        cmd->logger->file_idx++;

        fclose(cmd->logger->file);
        cmd->logger->file = 0;

        int file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
            L"%ls/%ls_%04d-%02d-%02d_%zu.txt",
            cmd->logger->path, cmd->logger->name,
            cmd->logger->file_time.tm_year + 1900,
            cmd->logger->file_time.tm_mon + 1,
            cmd->logger->file_time.tm_mday,
            cmd->logger->file_idx);

        if (wcslen(cmd->logger->name) <= 0)
        {
            file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
                L"%ls/%04d-%02d-%02d_%zu.txt",
                cmd->logger->path,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday,
                cmd->logger->file_idx);
        }

        cmd->logger->file = _wfsopen(file_full_path, L"a", _SH_DENYWR);
        if (!cmd->logger->file)
        {
            char sz_file_full_path[MAX_LOG_FILE_PATH];
            int len = wc_to_mb(CP_UTF8, file_full_path, file_full_path_len, sz_file_full_path, MAX_LOG_FILE_PATH);
            sz_file_full_path[len] = 0;
            //fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno);
            err_msg = fmt::system_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno).what();
            err_msg.append("\n");
            return false;
        }

        fseek(cmd->logger->file, 0, SEEK_END);
        cmd->logger->file_size = ftell(cmd->logger->file);
    }

    return true;
}

bool log_thread::_reset_log(log_cmd* cmd, log_proc* proc, std::string& err_msg)
{
    wchar_t file_full_path[MAX_LOG_FILE_PATH];

    time_t cur_time = std::chrono::system_clock::to_time_t(cmd->tpms);

    if (cmd->logger->file)
    {
        fclose(cmd->logger->file);
        cmd->logger->file = 0;
    }

    if (!cmd->logger->file)
    {
        localtime_s(&cmd->logger->file_time, &cur_time);

        int file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
            L"%ls/%ls_%04d-%02d-%02d.txt",
            cmd->logger->path, cmd->logger->name,
            cmd->logger->file_time.tm_year + 1900,
            cmd->logger->file_time.tm_mon + 1,
            cmd->logger->file_time.tm_mday);

        if (wcslen(cmd->logger->name) <= 0)
        {
            file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
                L"%ls/%04d-%02d-%02d.txt",
                cmd->logger->path,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday);
        }

        cmd->logger->file = _wfsopen(file_full_path, L"w", _SH_DENYWR);
        if (!cmd->logger->file)
        {
            char sz_file_full_path[MAX_LOG_FILE_PATH];
            int len = wc_to_mb(CP_UTF8, file_full_path, file_full_path_len, sz_file_full_path, MAX_LOG_FILE_PATH);
            sz_file_full_path[len] = 0;
            //fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno);
            err_msg = fmt::system_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno).what();
            err_msg.append("\n");

            return false;
        }

        cmd->logger->file_idx = 0;

        fseek(cmd->logger->file, 0, SEEK_END);
        cmd->logger->file_size = ftell(cmd->logger->file);
    }

    long long time_del = cur_time - proc->last_log_time;

    if (time_del)
    {
        if (time_del < 60 && time_del > -60)
        {
            int new_sec = proc->last_log_tm.tm_sec + (int)time_del;

            if (new_sec >= 0 && new_sec <= 59)
            {
                proc->last_log_tm.tm_sec = new_sec;
            }
            else
            {
                localtime_s(&proc->last_log_tm, &cur_time);
            }
        }
        else
        {
            localtime_s(&proc->last_log_tm, &cur_time);
        }

        proc->last_log_time = cur_time;
        strftime(proc->time_str, sizeof(proc->time_str), "%Y-%m-%d %H:%M:%S", &proc->last_log_tm);

        if (proc->last_log_tm.tm_yday != cmd->logger->file_time.tm_yday)
        {
            cmd->logger->file_time = proc->last_log_tm;

            if (cmd->logger->file)
            {
                fclose(cmd->logger->file);
                cmd->logger->file = 0;
            }

            int file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
                L"%ls/%ls_%04d-%02d-%02d.txt",
                cmd->logger->path, cmd->logger->name,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday);

            if (wcslen(cmd->logger->name) <= 0)
            {
                file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
                    L"%ls/%04d-%02d-%02d.txt",
                    cmd->logger->path,
                    cmd->logger->file_time.tm_year + 1900,
                    cmd->logger->file_time.tm_mon + 1,
                    cmd->logger->file_time.tm_mday);
            }

            cmd->logger->file = _wfsopen(file_full_path, L"w", _SH_DENYWR);
            if (!cmd->logger->file)
            {
                char sz_file_full_path[MAX_LOG_FILE_PATH];
                int len = wc_to_mb(CP_UTF8, file_full_path, file_full_path_len, sz_file_full_path, MAX_LOG_FILE_PATH);
                sz_file_full_path[len] = 0;
                //fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno);
                err_msg = fmt::system_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno).what();
                err_msg.append("\n");
                return false;
            }

            fseek(cmd->logger->file, 0, SEEK_END);
            cmd->logger->file_size = ftell(cmd->logger->file);
        }
    }

    while (cmd->logger->file_size >= MAX_LOG_FILE_SIZE)
    {
        cmd->logger->file_idx++;

        fclose(cmd->logger->file);
        cmd->logger->file = 0;

        int file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
            L"%ls/%ls_%04d-%02d-%02d_%zu.txt",
            cmd->logger->path, cmd->logger->name,
            cmd->logger->file_time.tm_year + 1900,
            cmd->logger->file_time.tm_mon + 1,
            cmd->logger->file_time.tm_mday,
            cmd->logger->file_idx);

        if (wcslen(cmd->logger->name) <= 0)
        {
            file_full_path_len = _snwprintf_s(file_full_path, MAX_LOG_FILE_PATH, _TRUNCATE,
                L"%ls/%04d-%02d-%02d_%zu.txt",
                cmd->logger->path,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday,
                cmd->logger->file_idx);
        }

        cmd->logger->file = _wfsopen(file_full_path, L"w", _SH_DENYWR);
        if (!cmd->logger->file)
        {
            char sz_file_full_path[MAX_LOG_FILE_PATH];
            int len = wc_to_mb(CP_UTF8, file_full_path, file_full_path_len, sz_file_full_path, MAX_LOG_FILE_PATH);
            sz_file_full_path[len] = 0;
            //fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno);
            err_msg = fmt::system_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", sz_file_full_path, errno).what();
            err_msg.append("\n");
            return false;
        }

        fseek(cmd->logger->file, 0, SEEK_END);
        cmd->logger->file_size = ftell(cmd->logger->file);
    }

    return true;
}

#elif __GNUC__
bool _mk_dir(const char* dir)
{
    char szPath[MAX_LOG_FILE_PATH] = { 0 };
    strncpy(szPath, dir, MAX_LOG_FILE_PATH);
    char* p1 = szPath + 1;

    char* p = szPath;

    for (size_t i = 0; i < strlen(szPath); i++)
    {
        if (*(p + i) == '\\')
        {
            *(p + i) = '/';
        }
    }

    do
    {
        p1 = strchr(p1, '/');
        if (p1 != 0)
        {
            *p1 = '\0';
        }

        if (-1 == mkdir(szPath, 0755))
        {
            if ((0 == p1) && (errno != EEXIST))
            {
                return false;
            }
        }
        if (p1 != 0)
        {
            *p1++ = '/';
        }

    } while (p1 != 0);

    return true;
}

bool log_thread::_check_log(log_cmd* cmd, log_proc* proc, std::string& err_msg)
{
    char file_full_path[MAX_LOG_FILE_PATH];

    time_t cur_time = std::chrono::system_clock::to_time_t(cmd->tpms);

    if (!cmd->logger->file)
    {
        localtime_r(&cur_time, &cmd->logger->file_time);

        snprintf(file_full_path, MAX_LOG_FILE_PATH,
            "%s/%s_%04d-%02d-%02d.txt",
            cmd->logger->path, cmd->logger->name,
            cmd->logger->file_time.tm_year + 1900,
            cmd->logger->file_time.tm_mon + 1,
            cmd->logger->file_time.tm_mday);

        if (strlen(cmd->logger->name) <= 0)
        {
            snprintf(file_full_path, MAX_LOG_FILE_PATH,
                "%s/%04d-%02d-%02d.txt",
                cmd->logger->path,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday);
        }

        cmd->logger->file = fopen(file_full_path, "a");
        if (!cmd->logger->file)
        {
            fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", file_full_path, errno);
            err_msg = sys_error.what();
            err_msg.append("\n");
            return false;
        }

        cmd->logger->file_idx = 0;

        fseek(cmd->logger->file, 0, SEEK_END);
        cmd->logger->file_size = ftell(cmd->logger->file);
    }

    long long time_del = cur_time - proc->last_log_time;

    if (time_del)
    {
        if (time_del < 60 && time_del > -60)
        {
            int new_sec = proc->last_log_tm.tm_sec + (int)time_del;

            if (new_sec >= 0 && new_sec <= 59)
            {
                proc->last_log_tm.tm_sec = new_sec;
            }
            else
            {
                localtime_r(&cur_time, &proc->last_log_tm);
            }
        }
        else
        {
            localtime_r(&cur_time, &proc->last_log_tm);
        }

        proc->last_log_time = cur_time;
        strftime(proc->time_str, sizeof(proc->time_str), "%Y-%m-%d %H:%M:%S", &proc->last_log_tm);

        if (proc->last_log_tm.tm_yday != cmd->logger->file_time.tm_yday)
        {
            cmd->logger->file_time = proc->last_log_tm;

            if (cmd->logger->file)
            {
                fclose(cmd->logger->file);
                cmd->logger->file = 0;
            }

            snprintf(file_full_path, MAX_LOG_FILE_PATH,
                "%s/%s_%04d-%02d-%02d.txt",
                cmd->logger->path, cmd->logger->name,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday);

            if (strlen(cmd->logger->name) <= 0)
            {
                snprintf(file_full_path, MAX_LOG_FILE_PATH,
                    "%s/%04d-%02d-%02d.txt",
                    cmd->logger->path,
                    cmd->logger->file_time.tm_year + 1900,
                    cmd->logger->file_time.tm_mon + 1,
                    cmd->logger->file_time.tm_mday);
            }

            cmd->logger->file = fopen(file_full_path, "a");
            if (!cmd->logger->file)
            {
                fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", file_full_path, errno);
                err_msg = sys_error.what();
                err_msg.append("\n");
                return false;
            }

            fseek(cmd->logger->file, 0, SEEK_END);
            cmd->logger->file_size = ftell(cmd->logger->file);
        }
    }

    while (cmd->logger->file_size >= MAX_LOG_FILE_SIZE)
    {
        cmd->logger->file_idx++;

        fclose(cmd->logger->file);
        cmd->logger->file = 0;

        snprintf(file_full_path, MAX_LOG_FILE_PATH,
            "%s/%s_%04d-%02d-%02d_%zu.txt",
            cmd->logger->path, cmd->logger->name,
            cmd->logger->file_time.tm_year + 1900,
            cmd->logger->file_time.tm_mon + 1,
            cmd->logger->file_time.tm_mday,
            cmd->logger->file_idx);

        if (strlen(cmd->logger->name) <= 0)
        {
            snprintf(file_full_path, MAX_LOG_FILE_PATH,
                "%s/%04d-%02d-%02d_%zu.txt",
                cmd->logger->path,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday,
                cmd->logger->file_idx);
        }

        cmd->logger->file = fopen(file_full_path, "a");
        if (!cmd->logger->file)
        {
            fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: open file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", file_full_path, errno);
            err_msg = sys_error.what();
            err_msg.append("\n");
            return false;
        }

        fseek(cmd->logger->file, 0, SEEK_END);
        cmd->logger->file_size = ftell(cmd->logger->file);
    }

    return true;
}

bool log_thread::_reset_log(log_cmd* cmd, log_proc* proc, std::string& err_msg)
{
    char file_full_path[MAX_LOG_FILE_PATH];

    time_t cur_time = std::chrono::system_clock::to_time_t(cmd->tpms);

    if (cmd->logger->file)
    {
        fclose(cmd->logger->file);
        cmd->logger->file = 0;
    }

    if (!cmd->logger->file)
    {
        localtime_r(&cur_time, &cmd->logger->file_time);

        snprintf(file_full_path, MAX_LOG_FILE_PATH,
            "%s/%s_%04d-%02d-%02d.txt",
            cmd->logger->path, cmd->logger->name,
            cmd->logger->file_time.tm_year + 1900,
            cmd->logger->file_time.tm_mon + 1,
            cmd->logger->file_time.tm_mday);

        if (strlen(cmd->logger->name) <= 0)
        {
            snprintf(file_full_path, MAX_LOG_FILE_PATH,
                "%s/%04d-%02d-%02d.txt",
                cmd->logger->path,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday);
        }

        cmd->logger->file = fopen(file_full_path, "w");
        if (!cmd->logger->file)
        {
            fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", file_full_path, errno);
            err_msg = sys_error.what();
            err_msg.append("\n");
            return false;
        }

        cmd->logger->file_idx = 0;

        fseek(cmd->logger->file, 0, SEEK_END);
        cmd->logger->file_size = ftell(cmd->logger->file);
    }

    long long time_del = cur_time - proc->last_log_time;

    if (time_del)
    {
        if (time_del < 60 && time_del > -60)
        {
            int new_sec = proc->last_log_tm.tm_sec + (int)time_del;

            if (new_sec >= 0 && new_sec <= 59)
            {
                proc->last_log_tm.tm_sec = new_sec;
            }
            else
            {
                localtime_r(&cur_time, &proc->last_log_tm);
            }
        }
        else
        {
            localtime_r(&cur_time, &proc->last_log_tm);
        }

        proc->last_log_time = cur_time;
        strftime(proc->time_str, sizeof(proc->time_str), "%Y-%m-%d %H:%M:%S", &proc->last_log_tm);

        if (proc->last_log_tm.tm_yday != cmd->logger->file_time.tm_yday)
        {
            cmd->logger->file_time = proc->last_log_tm;

            if (cmd->logger->file)
            {
                fclose(cmd->logger->file);
                cmd->logger->file = 0;
            }

            snprintf(file_full_path, MAX_LOG_FILE_PATH,
                "%s/%s_%04d-%02d-%02d.txt",
                cmd->logger->path, cmd->logger->name,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday);

            if (strlen(cmd->logger->name) <= 0)
            {
                snprintf(file_full_path, MAX_LOG_FILE_PATH,
                    "%s/%04d-%02d-%02d.txt",
                    cmd->logger->path,
                    cmd->logger->file_time.tm_year + 1900,
                    cmd->logger->file_time.tm_mon + 1,
                    cmd->logger->file_time.tm_mday);
            }

            cmd->logger->file = fopen(file_full_path, "w");
            if (!cmd->logger->file)
            {
                fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", file_full_path, errno);
                err_msg = sys_error.what();
                err_msg.append("\n");
                return false;
            }

            fseek(cmd->logger->file, 0, SEEK_END);
            cmd->logger->file_size = ftell(cmd->logger->file);
        }
    }

    while (cmd->logger->file_size >= MAX_LOG_FILE_SIZE)
    {
        cmd->logger->file_idx++;

        fclose(cmd->logger->file);
        cmd->logger->file = 0;

        snprintf(file_full_path, MAX_LOG_FILE_PATH,
            "%s/%s_%04d-%02d-%02d_%zu.txt",
            cmd->logger->path, cmd->logger->name,
            cmd->logger->file_time.tm_year + 1900,
            cmd->logger->file_time.tm_mon + 1,
            cmd->logger->file_time.tm_mday,
            cmd->logger->file_idx);

        if (strlen(cmd->logger->name) <= 0)
        {
            snprintf(file_full_path, MAX_LOG_FILE_PATH,
                "%s/%04d-%02d-%02d_%zu.txt",
                cmd->logger->path,
                cmd->logger->file_time.tm_year + 1900,
                cmd->logger->file_time.tm_mon + 1,
                cmd->logger->file_time.tm_mday,
                cmd->logger->file_idx);
        }

        cmd->logger->file = fopen(file_full_path, "w");
        if (!cmd->logger->file)
        {
            fmt::system_error sys_error(errno, "{}.{:<4}<{:<5}> {}: reset file {} fail-{}: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, "[ERR]", file_full_path, errno);
            err_msg = sys_error.what();
            err_msg.append("\n");
            return false;
        }

        fseek(cmd->logger->file, 0, SEEK_END);
        cmd->logger->file_size = ftell(cmd->logger->file);
    }

    return true;
}

#else
#error "unknown compiler"
#endif

//const char* log_lv_to_str(file_logger_level lv)
//{
//    switch (lv)
//    {
//    case log_nul:
//        return "";
//        break;
//    case log_dbg:
//        return "[DBG]";
//        break;
//    case log_inf:
//        return "[INF]";
//        break;
//    case log_wrn:
//        return "[WRN]";
//        break;
//    case log_cri:
//        return "[CRI]";
//        break;
//    case log_sys:
//        return "[SYS]";
//        break;
//    default:
//        return "[OMG]";
//    }
//}



void log_thread::_do_cmd(log_cmd* cmd, log_proc* proc)
{
    switch (cmd->option)
    {
    case opt_write_c:
    case opt_write:
    {
        fmt::basic_memory_buffer<char, 64> out_prefix;
        my_fmt_memory_buffer out_data;
        std::string err_msg;

        bool check_ret = _check_log(cmd, proc, err_msg);

        fmt::format_to(std::back_inserter(out_prefix), "{}.{:<4}<{:<5}> [{}]: ", proc->time_str, cmd->tpms.time_since_epoch().count() % 1000, proc->t_id, cmd->prefix/*log_lv_to_str(cmd->lv)*/);
        if (opt_write_c == cmd->option)
        {
            cmd->fmt_args->format_c_to_buffer(out_data);
        }
        else
        {
            cmd->fmt_args->format_to_buffer(out_data);
        }
        
        out_data.push_back('\n');

        if (check_ret)
        {
            size_t write_size = fwrite(out_prefix.data(), sizeof(char), out_prefix.size(), cmd->logger->file);
            write_size += fwrite(out_data.data(), sizeof(char), out_data.size(), cmd->logger->file);

            cmd->logger->file_size += write_size;
        }
        else
        {
            print_cmd err_pt_cmd;
            err_pt_cmd.data_len = err_msg.length();
            err_pt_cmd.color = print_color::red;
            //err_pt_cmd.lv = log_cri;

            if (loop_cache_free_size(m_print_data_cache) >= sizeof(print_cmd) + err_pt_cmd.data_len)
            {
                loop_cache_push_data(m_print_data_cache, &err_pt_cmd, sizeof(print_cmd));
                loop_cache_push_data(m_print_data_cache, err_msg.c_str(), err_msg.length());
            }
        }

        cmd->logger->log_ack++;

        print_cmd pt_cmd;
        pt_cmd.data_len = out_prefix.size() + out_data.size();
        pt_cmd.color = cmd->color;

        if (loop_cache_free_size(m_print_data_cache) > sizeof(print_cmd) + pt_cmd.data_len)
        {
            if (!loop_cache_push_data(m_print_data_cache, &pt_cmd, sizeof(print_cmd)))
            {
                CRUSH_CODE();
            }
            if (!loop_cache_push_data(m_print_data_cache, out_prefix.data(), out_prefix.size()))
            {
                CRUSH_CODE();
            }
            if (!loop_cache_push_data(m_print_data_cache, out_data.data(), out_data.size()))
            {
                CRUSH_CODE();
            }
        }
    }
    break;
    case opt_flush:
    {
        if (cmd->logger->file)
        {
            fflush(cmd->logger->file);
        }

        cmd->logger->log_ack++;
    }
    break;
    case opt_close:
    {
        if (cmd->logger->file)
        {
            fclose(cmd->logger->file);
            free(cmd->logger);
        }
    }
    break;
    case opt_reset:
    {
        std::string err_msg;

        if (!_reset_log(cmd, proc, err_msg))
        {
            print_cmd err_pt_cmd;
            err_pt_cmd.data_len = err_msg.length();
            err_pt_cmd.color = print_color::red;

            if (loop_cache_free_size(m_print_data_cache) >= sizeof(print_cmd) + err_pt_cmd.data_len)
            {
                loop_cache_push_data(m_print_data_cache, &err_pt_cmd, sizeof(print_cmd));
                loop_cache_push_data(m_print_data_cache, err_msg.c_str(), err_msg.length());
            }
        }

        cmd->logger->log_ack++;
    }
    break;
    default:
    {
        CRUSH_CODE();
    }
    }
}

unsigned int log_thread::_proc_log()
{
    unsigned int proc_count = 0;

    log_proc* proc = g_logger_manager->log_proc_head;

    while (proc)
    {
        log_cmd* cmd;

        if (loop_cache_pop_data(proc->queue[m_idx].cmd_queue, &cmd, sizeof(log_cmd*)))
        {
            _do_cmd(cmd, proc);
            ++proc_count;

            if (proc->queue[m_idx].rcy_cache_queue->empty())
            {
                if (!loop_cache_push_data(proc->queue[m_idx].rcy_queue, &cmd, sizeof(log_cmd*)))
                {
                    proc->queue[m_idx].rcy_cache_queue->push(cmd);
                }
            }
            else
            {
                proc->queue[m_idx].rcy_cache_queue->push(cmd);
            }
        }
        else
        {
            if (!proc->is_run)
            {
                while (!proc->queue[m_idx].rcy_cache_queue->empty())
                {
                    cmd = proc->queue[m_idx].rcy_cache_queue->front();
                    proc->queue[m_idx].rcy_cache_queue->pop();
                    
                    S_DELETE(cmd->fmt_args);
                    S_DELETE(cmd);
                }

                while (loop_cache_pop_data(proc->queue[m_idx].rcy_queue, &cmd, sizeof(log_cmd*)))
                {
                    S_DELETE(cmd->fmt_args);
                    S_DELETE(cmd);
                }
            }
        }

        while (!proc->queue[m_idx].rcy_cache_queue->empty())
        {
            cmd = proc->queue[m_idx].rcy_cache_queue->front();

            if (loop_cache_push_data(proc->queue[m_idx].rcy_queue, &cmd, sizeof(log_cmd*)))
            {
                proc->queue[m_idx].rcy_cache_queue->pop();
            }
            else
            {
                break;
            }
        }

        proc = proc->next_proc;
    }

    return proc_count;
}

void log_thread::_proc_log_end()
{
    log_proc* proc = g_logger_manager->log_proc_head;
    bool is_busy = false;

    for (;;)
    {
        if (!proc)
        {
            if (is_busy)
            {
                proc = g_logger_manager->log_proc_head;
                is_busy = false;
            }
            else
            {
                return;
            }
        }

        log_cmd* cmd;

        if (loop_cache_pop_data(proc->queue[m_idx].cmd_queue, &cmd, sizeof(log_cmd*)))
        {
            _do_cmd(cmd, proc);
            is_busy = true;

            if (proc->queue[m_idx].rcy_cache_queue->empty())
            {
                if (!loop_cache_push_data(proc->queue[m_idx].rcy_queue, &cmd, sizeof(log_cmd*)))
                {
                    proc->queue[m_idx].rcy_cache_queue->push(cmd);
                }
            }
            else
            {
                proc->queue[m_idx].rcy_cache_queue->push(cmd);
            }
        }
        else
        {
            if (!proc->is_run)
            {
                while (!proc->queue[m_idx].rcy_cache_queue->empty())
                {
                    cmd = proc->queue[m_idx].rcy_cache_queue->front();
                    proc->queue[m_idx].rcy_cache_queue->pop();

                    S_DELETE(cmd->fmt_args);
                    S_DELETE(cmd);
                }

                while (loop_cache_pop_data(proc->queue[m_idx].rcy_queue, &cmd, sizeof(log_cmd*)))
                {
                    S_DELETE(cmd->fmt_args);
                    S_DELETE(cmd);
                }
            }
        }

        proc = proc->next_proc;
    }
}

void log_thread::_log_func()
{
#ifdef __GNUC__
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sigset_t new_set, old_set;
    sigemptyset(&new_set);
    sigemptyset(&old_set);
    sigaddset(&new_set, SIGHUP);
    sigaddset(&new_set, SIGINT);
    sigaddset(&new_set, SIGQUIT);
    sigaddset(&new_set, SIGTERM);
    sigaddset(&new_set, SIGUSR1);
    sigaddset(&new_set, SIGUSR2);
    sigaddset(&new_set, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &new_set, &old_set);
#endif

    m_is_run = true;
    unsigned int cur_do_proc_count = 0;
    unsigned int run_loop_check = 0;
    unsigned int last_do_proc_count = 0;
    while (g_logger_manager->is_run)
    {
        last_do_proc_count = _proc_log();
        cur_do_proc_count += last_do_proc_count;
        ++run_loop_check;

        if (run_loop_check >= 100)
        {
            m_do_proc_count = cur_do_proc_count;
            run_loop_check = 0;
            cur_do_proc_count = 0;
        }

        if (!last_do_proc_count)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

    _proc_log_end();

    m_is_run = false;
}

//////////////////////////////////////////////////////////////////////////

print_thread::print_thread()
{
    m_print_cache = create_loop_cache(0, g_logger_manager->print_cache_size * g_logger_manager->log_thread_num);

    m_print_thread = std::thread(&print_thread::_print_func, this);
    //m_last_level = log_nul;
    m_last_color = print_color::null;
    
}

print_thread::~print_thread()
{
    m_print_thread.join();
    destroy_loop_cache(m_print_cache);
}

#ifdef _MSC_VER
void print_thread::_check_print(print_cmd* cmd)
{
    if (cmd->color != m_last_color)
    {
        m_last_color = cmd->color;

        switch (m_last_color)
        {
        case print_color::null:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
            break;
        case print_color::red:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
            break;
        case print_color::green:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
            break;
        case print_color::blue:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
            break;
        case print_color::yellow:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_RED);
            break;
        case print_color::pink:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
            break;
        case print_color::cyan:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case print_color::white:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        default:
            break;
        }
    }
}
#elif __GNUC__
void print_thread::_check_print(print_cmd* cmd)
{
    if (cmd->color != m_last_color)
    {
        m_last_color = cmd->color;

        switch (m_last_color)
        {
        case print_color::null:
            printf("\033[0m");
            break;
        case print_color::red:
            printf("\033[0;31m");
            break;
        case print_color::green:
            printf("\033[0;32m");
            break;
        case print_color::blue:
            printf("\033[0;34m");
            break;
        case print_color::yellow:
            printf("\033[1;33m");
            break;
        case print_color::pink:
            printf("\033[0;35m");
            break;
        case print_color::cyan:
            printf("\033[0;36m");
            break;
        case print_color::white:
            printf("\033[1;37m");
            break;
        default:
            break;
        }
    }
}
#else
#error "unknown compiler"
#endif

void print_thread::_print_func()
{
#ifdef __GNUC__
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sigset_t new_set, old_set;
    sigemptyset(&new_set);
    sigemptyset(&old_set);
    sigaddset(&new_set, SIGHUP);
    sigaddset(&new_set, SIGINT);
    sigaddset(&new_set, SIGQUIT);
    sigaddset(&new_set, SIGTERM);
    sigaddset(&new_set, SIGUSR1);
    sigaddset(&new_set, SIGUSR2);
    sigaddset(&new_set, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &new_set, &old_set);
#endif

    while (g_logger_manager->is_run)
    {
        if (!_proc_print())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    for (size_t i = 0; i < g_logger_manager->log_thread_num;)
    {
        if (g_logger_manager->log_thread_array[i].is_run())
        {
            i = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        else
        {
            i++;
        }
    }

    _proc_print_end();
}

unsigned int print_thread::_proc_print()
{
    unsigned int print_count = 0;

    for (size_t i = 0; i < g_logger_manager->log_thread_num; i++)
    {
        if (_do_pt_cmd(g_logger_manager->log_thread_array[i].print_data_cache()))
        {
            ++print_count;
        }
    }

    return print_count;
}

void print_thread::_proc_print_end()
{
    while (_proc_print())
    {
    }

    print_cmd end_print_cmd;
    end_print_cmd.data_len = 0;
    end_print_cmd.color = print_color::null;
    _check_print(&end_print_cmd);
}

bool print_thread::_do_pt_cmd(HLOOPCACHE print_cache)
{
    print_cmd cmd;

    if (loop_cache_pop_data(print_cache, &cmd, sizeof(print_cmd)))
    {
        _check_print(&cmd);

        size_t print_len = cmd.data_len;

        while (cmd.data_len)
        {
            char* data = (char*)loop_cache_get_data(print_cache, &print_len);
            fwrite(data, sizeof(char), print_len, stdout);

            loop_cache_push_data(m_print_cache, data, print_len);
#ifdef _DEBUG
            fflush(stdout);
#endif
            loop_cache_pop(print_cache, print_len);
            cmd.data_len -= print_len;
            print_len = cmd.data_len;
        }
        
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////

size_t _get_idle_log_thread_idx(void)
{
    size_t idle_log_thread_idx = 0;
    unsigned int do_proc_count = g_logger_manager->log_thread_array[0].get_proc_count();

    for (size_t i = 1; i < g_logger_manager->log_thread_num; i++)
    {
        if (do_proc_count > g_logger_manager->log_thread_array[i].get_proc_count())
        {
            do_proc_count = g_logger_manager->log_thread_array[i].get_proc_count();
            idle_log_thread_idx = i;
        }
    }

    return idle_log_thread_idx;
}

CThreadLogProc& GetThreadLogProc(void)
{
    static thread_local CThreadLogProc thread_log_proc;

    return thread_log_proc;
}

log_proc* _get_log_proc(void)
{
    return GetThreadLogProc().GetLogProc();
}

log_proc** _get_log_proc_addr(void)
{
    return GetThreadLogProc().GetLogProcAddr();
}

//void update_logger_mem_pool(HMEMORYMANAGER new_mem_pool)
//{
//    log_mem_pool* new_pool = (log_mem_pool*)malloc(sizeof(log_mem_pool));
//    new_pool->mem_pool = new_mem_pool;
//
//    log_proc* proc = _get_log_proc();
//
//    new_pool->next_pool = proc->log_mem_pool_head;
//    proc->log_mem_pool_head = new_pool;
//}
//
//void update_logger_obj_pool(SMemory::IClassMemory* new_obj_pool)
//{
//    log_obj_pool* new_pool = (log_obj_pool*)malloc(sizeof(log_obj_pool));
//    new_pool->obj_pool = new_obj_pool;
//
//    log_proc* proc = _get_log_proc();
//
//    new_pool->next_pool = proc->log_obj_pool_head;
//    proc->log_obj_pool_head = new_pool;
//}

//HMEMORYMANAGER logger_mem_pool(void)
//{
//    static TLS_VAR HMEMORYMANAGER mem_mgr = 0;
//
//    if (!mem_mgr)
//    {
//        mem_mgr = create_memory_manager(8, 128, 65536, 4 * 1024, 2);
//
//        update_logger_mem_pool(mem_mgr);
//    }
//
//    return mem_mgr;
//}

void _free_log_cmd(log_proc* proc)
{
    for (size_t i = 0; i < g_logger_manager->log_thread_num; i++)
    {
        for (;;)
        {
            log_cmd* cmd;

            if (loop_cache_pop_data(proc->queue[i].rcy_queue, &cmd, sizeof(log_cmd*)))
            {
                S_DELETE(cmd->fmt_args);
                S_DELETE(cmd);
                //SMemory::Delete(cmd->fmt_args);
                //SMemory::Delete(cmd);
            }
            else
            {
                break;
            }
        }
    }
}

log_cmd* _get_log_cmd(log_proc* proc)
{
    log_cmd* cmd = 0;
    log_cmd* last_cmd = 0;

    for (size_t i = 0; i < g_logger_manager->log_thread_num; i++)
    {
        for (;;)
        {
            if (loop_cache_pop_data(proc->queue[i].rcy_queue, &last_cmd, sizeof(log_cmd*)))
            {
                if (cmd)
                {
                    //SMemory::Delete(cmd->fmt_args);
                    //SMemory::Delete(cmd);
                    S_DELETE(cmd->fmt_args);
                    S_DELETE(cmd);
                }
                cmd = last_cmd;
            }
            else
            {
                break;
            }
        }
    }

    if (cmd)
    {
        //SMemory::Delete(cmd->fmt_args);
        S_DELETE(cmd->fmt_args);
    }
    else
    {
        //cmd = logger_obj_pool<log_cmd>()->New(1);
        cmd = S_NEW(log_cmd, 1);
    }

    return cmd;
}

bool file_logger_async_log(file_logger* logger, print_color color, const char* prefix, SFormatArgs<>* fmt_args, bool is_block)
{
    log_proc* proc = _get_log_proc();

    log_cmd* cmd = _get_log_cmd(proc);

    cmd->option = opt_write;

    //if (is_c_format)
    //{
    //    cmd->option = opt_write_c;
    //}

    cmd->fmt_args = fmt_args;
    cmd->logger = logger;
    cmd->prefix = prefix;
    cmd->color = color;
    //cmd->lv = lv;

    cmd->tpms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    if (loop_cache_push_data(proc->queue[logger->log_thread_idx].cmd_queue, &cmd, sizeof(log_cmd*)))
    {
        if (logger->create_proc == proc)
        {
            logger->log_req++;
        }
        else
        {
#ifdef _MSC_VER
            InterlockedIncrement64((LONG64*)(&logger->log_req_mt));
#elif __GNUC__
            __atomic_add_fetch(&logger->log_req_mt, 1, __ATOMIC_SEQ_CST);
#else
#error "unknown compiler"
#endif
        }
        return true;
    }
    else
    {
        if (is_block)
        {
            while (!loop_cache_push_data(proc->queue[logger->log_thread_idx].cmd_queue, &cmd, sizeof(log_cmd*)))
            {
                _free_log_cmd(proc);
            }

            if (logger->create_proc == proc)
            {
                logger->log_req++;
            }
            else
            {
#ifdef _MSC_VER
                InterlockedIncrement64((LONG64*)(&logger->log_req_mt));
#elif __GNUC__
                __atomic_add_fetch(&logger->log_req_mt, 1, __ATOMIC_SEQ_CST);
#else
#error "unknown compiler"
#endif
            }
            return true;
        }
        else
        {
            //SMemory::Delete(cmd->fmt_args);
            //SMemory::Delete(cmd);
            S_DELETE(cmd->fmt_args);
            S_DELETE(cmd);

            return false;
        }
    }
}

bool init_logger_manager(size_t log_thread_num, size_t max_log_event_num, size_t print_cache_size)
{
    if (!g_logger_manager)
    {
        g_logger_manager = new logger_manager;
        g_logger_manager->is_run = true;
        g_logger_manager->log_proc_head = 0;
        g_logger_manager->log_thread_num = log_thread_num;
        g_logger_manager->log_queue_size = max_log_event_num;
        g_logger_manager->print_cache_size = print_cache_size;
        g_logger_manager->log_thread_array = new log_thread[log_thread_num];
        g_logger_manager->main_log_proc = _get_log_proc_addr();
        for (size_t i = 0; i < log_thread_num; i++)
        {
            g_logger_manager->log_thread_array[i].set_idx(i);
            g_logger_manager->log_thread_array[i].start();
        }
        g_logger_manager->print_thread_pt = new print_thread;
    }

    return true;
}

void uninit_logger_manager(void)
{
    if (g_logger_manager)
    {
        g_logger_manager->is_run = false;

        delete g_logger_manager->print_thread_pt;
        delete[] g_logger_manager->log_thread_array;

        log_proc* proc = g_logger_manager->log_proc_head;

        while (proc)
        {
            log_proc* cur_proc = proc;
            proc = proc->next_proc;

            for (size_t i = 0; i < g_logger_manager->log_thread_num; i++)
            {
                log_cmd* cmd;
                while (loop_cache_pop_data(cur_proc->queue[i].cmd_queue, &cmd, sizeof(log_cmd*)))
                {
                    S_DELETE(cmd->fmt_args);
                    S_DELETE(cmd);
                }

                while (loop_cache_pop_data(cur_proc->queue[i].rcy_queue, &cmd, sizeof(log_cmd*)))
                {
                    S_DELETE(cmd->fmt_args);
                    S_DELETE(cmd);
                }

                while (!cur_proc->queue[i].rcy_cache_queue->empty())
                {
                    cmd = cur_proc->queue[i].rcy_cache_queue->front();
                    cur_proc->queue[i].rcy_cache_queue->pop();

                    S_DELETE(cmd->fmt_args);
                    S_DELETE(cmd);
                }


                destroy_loop_cache(cur_proc->queue[i].cmd_queue);
                destroy_loop_cache(cur_proc->queue[i].rcy_queue);
                delete cur_proc->queue[i].rcy_cache_queue;
            }

            //log_obj_pool* obj_pool = cur_proc->log_obj_pool_head;
            //while (obj_pool)
            //{
            //    log_obj_pool* cur_pool = obj_pool;
            //    obj_pool = obj_pool->next_pool;

            //    delete cur_pool->obj_pool;
            //    free(cur_pool);
            //}

            //log_mem_pool* mem_pool = cur_proc->log_mem_pool_head;

            //while (mem_pool)
            //{
            //    log_mem_pool* cur_pool = mem_pool;
            //    mem_pool = mem_pool->next_pool;

            //    destroy_memory_manager(cur_pool->mem_pool);
            //    free(cur_pool);
            //}

            free(cur_proc->queue);
            free(cur_proc);
            cur_proc = 0;
            
        }

        if (g_logger_manager->main_log_proc)
        {
            *(g_logger_manager->main_log_proc) = 0;
        }

        delete g_logger_manager;
        g_logger_manager = 0;
    }
}

HLOOPCACHE logger_manager_print_cache(void)
{
    if (g_logger_manager)
    {
        return g_logger_manager->print_thread_pt->print_cache();
    }

    return nullptr;
}

#ifdef _MSC_VER
file_logger* create_file_logger(const char* path_utf8, const char* name_utf8)
{
    wchar_t w_path[MAX_LOG_FILE_PATH];
    wchar_t w_name[MAX_LOG_FILE_PATH];

    if (!is_valid_utf8((const unsigned char*)path_utf8, strlen(path_utf8)))
    {
        return 0;
    }

    if (!is_valid_utf8((const unsigned char*)name_utf8, strlen(name_utf8)))
    {
        return 0;
    }

    int w_path_len = MultiByteToWideChar(CP_UTF8, 0, path_utf8, -1, w_path, MAX_LOG_FILE_PATH);
    int w_name_len = MultiByteToWideChar(CP_UTF8, 0, name_utf8, -1, w_name, MAX_LOG_FILE_PATH);

    if (!w_path_len || !w_name_len)
    {
        return 0;
    }

    if (!_mk_dir(w_path))
    {
        return 0;
    }

    char* ptr = (char*)malloc(sizeof(file_logger) + sizeof(wchar_t)*(w_path_len + w_name_len));

    file_logger* logger = (file_logger*)ptr;
    logger->log_req_mt = 0;
    logger->log_req = 0;
    logger->log_ack = 0;
    logger->path = (wchar_t*)(ptr + sizeof(file_logger));
    logger->name = (wchar_t*)(ptr + sizeof(file_logger) + sizeof(wchar_t)*w_path_len);

    wcscpy_s(logger->path, w_path_len, w_path);
    wcscpy_s(logger->name, w_name_len, w_name);

    logger->log_thread_idx = _get_idle_log_thread_idx();
    logger->file = 0;
    logger->file_size = 0;
    logger->file_idx = 0;
    logger->create_proc = _get_log_proc();

    return logger;
}
#elif __GNUC__
file_logger* create_file_logger(const char* path_utf8, const char* name_utf8)
{
    size_t path_len = strlen(path_utf8);
    size_t name_len = strlen(name_utf8);
    if (!is_valid_utf8((const unsigned char*)path_utf8, path_len))
    {
        return 0;
    }

    if (!is_valid_utf8((const unsigned char*)name_utf8, name_len))
    {
        return 0;
    }

    if (!_mk_dir(path_utf8))
    {
        return 0;
    }

    char* ptr = (char*)malloc(sizeof(file_logger) + sizeof(char)*(path_len + name_len + 2));

    file_logger* logger = (file_logger*)ptr;
    logger->log_req_mt = 0;
    logger->log_req = 0;
    logger->log_ack = 0;

    logger->path = (char*)(ptr + sizeof(file_logger));
    logger->name = (char*)(ptr + sizeof(file_logger) + sizeof(char)*(path_len + 1));

    strncpy(logger->path, path_utf8, path_len + 1);
    strncpy(logger->name, name_utf8, name_len + 1);

    logger->log_thread_idx = _get_idle_log_thread_idx();
    logger->file = 0;
    logger->file_time = { 0 };
    logger->file_size = 0;
    logger->file_idx = 0;
    logger->create_proc = _get_log_proc();

    return logger;
}
#else
#error "unknown compiler"
#endif



void destroy_file_logger(file_logger* logger)
{
    log_proc* proc = _get_log_proc();

    log_cmd* cmd = _get_log_cmd(proc);

    cmd->option = opt_close;
    cmd->fmt_args = 0;
    cmd->logger = logger;

    while ((logger->log_req + logger->log_req_mt) != logger->log_ack)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    while (!loop_cache_push_data(proc->queue[logger->log_thread_idx].cmd_queue, &cmd, sizeof(log_cmd*)))
    {
        _free_log_cmd(proc);
    }
}

void reset_file_logger(file_logger* logger)
{
    log_proc* proc = _get_log_proc();

    log_cmd* cmd = _get_log_cmd(proc);

    cmd->option = opt_reset;
    cmd->fmt_args = 0;
    cmd->logger = logger;
    cmd->tpms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

    if (loop_cache_push_data(proc->queue[logger->log_thread_idx].cmd_queue, &cmd, sizeof(log_cmd*)))
    {
        if (logger->create_proc == proc)
        {
            logger->log_req++;
        }
        else
        {
#ifdef _MSC_VER
            InterlockedIncrement64((LONG64*)(&logger->log_req_mt));
#elif __GNUC__
            __atomic_add_fetch(&logger->log_req_mt, 1, __ATOMIC_SEQ_CST);
#else
#error "unknown compiler"
#endif
        }
    }
    else
    {
        S_DELETE(cmd);
    }
}

void file_logger_flush(file_logger* logger)
{
    log_proc* proc = _get_log_proc();

    log_cmd* cmd = _get_log_cmd(proc);

    cmd->option = opt_flush;
    cmd->fmt_args = 0;
    cmd->logger = logger;

    if (loop_cache_push_data(proc->queue[logger->log_thread_idx].cmd_queue, &cmd, sizeof(log_cmd*)))
    {
        if (logger->create_proc == proc)
        {
            logger->log_req++;
        }
        else
        {
#ifdef _MSC_VER
            InterlockedIncrement64((LONG64*)(&logger->log_req_mt));
#elif __GNUC__
            __atomic_add_fetch(&logger->log_req_mt, 1, __ATOMIC_SEQ_CST);
#else
#error "unknown compiler"
#endif
        }
    }
    else
    {
        S_DELETE(cmd);
    }
}
