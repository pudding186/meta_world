#include "app_util.hpp"
#include "timer.h"
#include "rb_tree.h"
#include "utility.hpp"
#include "smemory.hpp"
#include "memory_trace.hpp"
#include "log_system.hpp"


#define TRACE_MEMORY_NAME u8"trace_memory"
#define MEMORY_INCREMENT_NAME u8"increment_memory"

#ifdef __GNUC__
#include <libgen.h>
#include <iconv.h>
#define MAX_PATH 260
#endif

#ifdef IsDatabaseServer
#include "player_system.hpp"
#endif

//char *AppUtil::UI642A64(uint64_t value, char *buf, int32_t count)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    static char map[] =
//    {
//        '0', '1', '2', '3', '4', '5', '6', '7',
//        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
//        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
//        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
//        'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
//        'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
//        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
//        'U', 'V', 'W', 'X', 'Y', 'Z', '$', '#'
//    };
//
//    char *p = buf + count - 1;
//    *p-- = 0;
//
//    while (p >= buf && value)
//    {
//        *p-- = map[value & 63];
//        value >>= 6;
//    }
//
//    if (value)
//    {
//        return 0;
//    }
//
//    return ++p;
//}

static char ia64_map[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '+', '#'
};

void AppUtil::StrSplitEx2(const std::string& str, const std::string& split, std::vector<std::string>& results)
{
    size_t max_split_count = str.length() / split.length();

    mem_seg* seg_arry = static_cast<mem_seg*>(S_MALLOC_EX(sizeof(mem_seg) * max_split_count, u8"mem_seg"));

    size_t real_split_count = split_mem_to_segments(str.c_str(), str.length(), split.c_str(), split.length(), seg_arry, max_split_count);

    if (real_split_count > max_split_count)
    {
        throw "split string overflow";
    }

    for (size_t i = 0; i < real_split_count; i++)
    {
        results.push_back(std::string(static_cast<const char*>(seg_arry[i].mem), seg_arry[i].mem_size));
    }
}

char* AppUtil::UI642A64(uint64_t value, char* buf, size_t buf_size)
{
    char* p = buf + buf_size - 1;
    *p-- = 0;

    while (p >= buf && value)
    {
        *p-- = ia64_map[value & 63];
        value >>= 6;
    }

    if (value)
    {
        return 0;
    }

    return ++p;
}

const char* AppUtil::A642UI64(const char* a64, size_t a64_len, uint64_t& value)
{
    const char* p = a64;
    const char* e = a64 + a64_len;

    int32_t shl_bit = 0;
    value = 0;
    if (!a64_len)
    {
        return p;
    }



    for (;;)
    {
        int32_t c = *p;
        p++;

        if (c >= 'a' && c <= 'z')
        {
            c = 10 + (c - 'a');
        }
        else if (c >= 'A' && c <= 'Z')
        {
            c = 36 + (c - 'A');
        }
        else if (c >= '0' && c <= '9')
        {
            c = c - '0';
        }
        else if (c == '+')
        {
            c = 62;
        }
        else if (c == '#')
        {
            c = 63;
        }
        else
        {
            value >>= 6;
            return p - 1;
        }

        value |= c;

        if (p < e)
        {
            if (shl_bit < 58)
            {
                value <<= 6;
                shl_bit += 6;
            }
            else
            {
                return p - 1;
            }
        }
        else
        {
            break;
        }
    }

    return nullptr;
}

bool AppUtil::A642UI64Ex(const std::string& s, uint64_t& value)
{
    //FUNC_PERFORMANCE_CHECK();

    auto fn = [](char c)
    {
        if (c >= 'a' && c <= 'z')
        {
            c = 10 + (c - 'a');
        }
        else if (c >= 'A' && c <= 'Z')
        {
            c = 36 + (c - 'A');
        }
        else if (c >= '0' && c <= '9')
        {
            c = c - '0';
        }
        else if (c == '$')
        {
            c = 62;
        }
        else if (c == '#')
        {
            c = 63;
        }
        else
        {
            return -1;
        }

        return (int32_t)c;
    };

    value = 0;

    const char* p = s.c_str();
    const char* e = p + s.length();
    while (p < e)
    {
        int32_t v = fn(*p++);
        if (v == -1)
        {
            return false;
        }

        value |= v;

        if (p < e)
        {
            value <<= 6;
        }
    }

    return true;
}

//const char* AppUtil::A642UI64(const std::string &s, uint64_t &value)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    auto fn = [](char c)
//    {
//        if (c >= 'a' && c <= 'z')
//        {
//            c = 10 + (c - 'a');
//        }
//        else if (c >= 'A' && c <= 'Z')
//        {
//            c = 36 + (c - 'A');
//        }
//        else if (c >= '0' && c <= '9')
//        {
//            c = c - '0';
//        }
//        else if (c == '$')
//        {
//            c = 62;
//        }
//        else if (c == '#')
//        {
//            c = 63;
//        }
//        else
//        {
//            return -1;
//        }
//
//        return (int32_t)c;
//    };
//
//    value = 0;
//
//    const char *p = s.c_str();
//    const char *e = p + s.length();
//    while (p < e)
//    {
//        int32_t v = fn(*p++);
//        if (v == -1)
//        {
//            return p;
//        }
//
//        value |= v;
//
//        if (p < e)
//        {
//            value <<= 6;
//        }
//    }
//
//    return nullptr;
//}


//const char* AppUtil::A642UI64(const char* a64, size_t a64_len, uint64_t& value)
//{
//    //FUNC_PERFORMANCE_CHECK();
//
//    auto fn = [](char c)
//    {
//        if (c >= 'a' && c <= 'z')
//        {
//            c = 10 + (c - 'a');
//        }
//        else if (c >= 'A' && c <= 'Z')
//        {
//            c = 36 + (c - 'A');
//        }
//        else if (c >= '0' && c <= '9')
//        {
//            c = c - '0';
//        }
//        else if (c == '$')
//        {
//            c = 62;
//        }
//        else if (c == '#')
//        {
//            c = 63;
//        }
//        else
//        {
//            return -1;
//        }
//
//        return (int32_t)c;
//    };
//
//    value = 0;
//
//    const char* p = a64;
//    const char* e = a64 + a64_len;
//
//    if (p < e)
//    {
//        int32_t v = fn(*p++);
//        if (v == -1)
//        {
//            return p;
//        }
//
//        value |= v;
//    }
//    else
//    {
//        return p;
//    }
//
//    while (p < e)
//    {
//        value <<= 6;
//
//        int32_t v = fn(*p++);
//        if (v == -1)
//        {
//            value >>= 6;
//            return p;
//        }
//
//        value |= v;
//    }
//
//    return nullptr;
//}


//const char* AppUtil::A642UI64(const char* a64, size_t a64_len, uint64_t& value)
//{
//    value = 0;
//
//    const char* p = a64;
//    const char* e = a64 + a64_len;
//
//    if (p < e)
//    {
//        char c = *p;
//        p++;
//        if (c >= 'a' && c <= 'z')
//        {
//            value |= (10 + (c - 'a'));
//        }
//        else if (c >= 'A' && c <= 'Z')
//        {
//            value |= (36 + (c - 'A'));
//        }
//        else if (c >= '0' && c <= '9')
//        {
//            value |= (c - '0');
//        }
//        else if (c == '$')
//        {
//            value |= 62;
//        }
//        else if (c == '#')
//        {
//            value |= 63;
//        }
//        else
//        {
//            return p;
//        }
//    }
//    else
//    {
//        return p;
//    }
//
//    while (p < e)
//    {
//        value <<= 6;
//
//        const char& c = *p;
//        p++;
//        if (c >= 'a' && c <= 'z')
//        {
//            value |= 10 + (c - 'a');
//        }
//        else if (c >= 'A' && c <= 'Z')
//        {
//            value |= (36 + (c - 'A'));
//        }
//        else if (c >= '0' && c <= '9')
//        {
//            value |= (c - '0');
//        }
//        else if (c == '$')
//        {
//            value |= 62;
//        }
//        else if (c == '#')
//        {
//            value |= 63;
//        }
//        else
//        {
//            value >>= 6;
//            return p;
//        }
//    }
//
//    return nullptr;
//}

bool AppUtil::Str2Digit(const std::string &str, int32_t &digit)
{
    FUNC_PERFORMANCE_CHECK();

    int32_t i = 0;
    if (str[0] == '+' ||
        str[0] == '-')
    {
        i = 1;
    }

    for (; i < (int32_t)str.length(); ++i)
    {
        if (str[i] < '0' ||
            str[i] > '9')
        {
            return false;
        }

        if (!isdigit(str[i]))
        {
            return false;
        }          
    }

    char *end_str = nullptr;
    digit = (int32_t)strtol(str.c_str(), &end_str, 10);
    return true;
}

//std::string AppUtil::IPN2Str(uint32_t ip)
//{
//    FUNC_PERFORMANCE_CHECK();
//
//    sockaddr_in si;
//    si.sin_family = AF_INET;
//    si.sin_addr.s_addr = ip;
//
//    return inet_ntoa(si.sin_addr);
//}

ptrdiff_t trace_info_size_cmp(const void* info1, const void* info2)
{
    mem_trace_info* t1 = (mem_trace_info*)info1;
    mem_trace_info* t2 = (mem_trace_info*)info2;

    if (t1->size > t2->size)
    {
        return -1;
    }
    else if (t1->size < t2->size)
    {
        return 1;
    }
    else
    {
        size_t t1_remain = t1->alloc - t1->free;
        size_t t2_remain = t2->alloc - t2->free;

        if (t1_remain > t2_remain)
        {
            return -1;
        }
        else if (t1_remain < t2_remain)
        {
            return 1;
        }
        else
        {
            return t1 - t2;
        }
    }
}

void AppUtil::LogMemoryPool(size_t limit)
{
    FUNC_PERFORMANCE_CHECK();

    HRBTREE trace_info_map = create_rb_tree(trace_info_size_cmp);

    HRBNODE mem_info_node = mem_trace_info_head();
    while (mem_info_node)
    {
        mem_trace_info* trace_info = (mem_trace_info*)rb_node_key_user(mem_info_node);
        mem_info_node = rb_next(mem_info_node);

        rb_tree_insert_user(trace_info_map, trace_info, trace_info);

        if (rb_tree_size(trace_info_map) > limit)
        {
            rb_tree_erase(trace_info_map, rb_last(trace_info_map));
        }
    }

    HRBNODE trace_info_node = rb_first(trace_info_map);
    while (trace_info_node)
    {
        mem_trace_info* trace_info = (mem_trace_info*)rb_node_key_user(trace_info_node);
        trace_info_node = rb_next(trace_info_node);

        std::string stack_string;

        if (trace_info->stack)
        {
            for (int i = 0; i < trace_info->stack->m_stack_size; i++)
            {
                if (i)
                {
                    stack_string += "->";
                    stack_string += trace_info->stack->m_stack[i]->func_name;
                }
                else
                {
                    stack_string = trace_info->stack->m_stack[i]->func_name;
                }
            }
        }

        LogSYS
        (
            u8"$size${}$alloc${}$free${}$remain${}$name${}$trace${}:{}$stack${}.",
            trace_info->size,
            trace_info->alloc,
            trace_info->free,
            trace_info->alloc - trace_info->free,
            trace_info->name,
            trace_info->file,
            trace_info->line,
            stack_string
        );
    }

    destroy_rb_tree(trace_info_map);
}

void AppUtil::TraceMemoryPool(const std::string& prefix)
{
    FUNC_PERFORMANCE_CHECK();
    LogSYS(u8"begin dump memory pool [wait].");

    std::string trace_memory_file_name = prefix;
    trace_memory_file_name += u8"_";
    trace_memory_file_name += TRACE_MEMORY_NAME;

    HRBNODE mem_info_node = mem_trace_info_head();
    while (mem_info_node)
    {
        mem_trace_info *trace_info = (mem_trace_info *)rb_node_key_user(mem_info_node);
        mem_info_node = rb_next(mem_info_node);

        std::string stack_string;

        if (trace_info->stack)
        {
            for (int i = 0; i < trace_info->stack->m_stack_size; i++)
            {
                if (i)
                {
                    stack_string += "->";
                    stack_string += trace_info->stack->m_stack[i]->func_name;
                }
                else
                {
                    stack_string = trace_info->stack->m_stack[i]->func_name;
                }
            }
        }

        LogSYSEx
        (
            trace_memory_file_name,
            u8"$size${}$alloc${}$free${}$remain${}$name${}$trace${}:{}$stack${}.",
            trace_info->size,
            trace_info->alloc,
            trace_info->free,
            trace_info->alloc - trace_info->free,
            trace_info->name,            
            trace_info->file, 
            trace_info->line,
            stack_string
        );
    }

    LogSYSEx(
        trace_memory_file_name,
        u8"$alloc${}$total${}",
        memory_alloc_size(),
        memory_total_size()
    );

#ifdef IsDatabaseServer
    LogSYSEx(trace_memory_file_name, u8"***begin cache trace***");
    CacheQueueBuilder::TraverseTrace([&](const std::string &name, 
        const CacheQueueBuilder::CacheQueueTraceData &data)
    {
        LogSYSEx
        (
            trace_memory_file_name,
            u8"$size${}$alloc${}$free${}$remain${}$name${}$trace${}:{}.",
            data.TotalSize,
            data.Alloc,
            data.Free,
            data.Remain,
            name,
            u8"",
            0
        );
        return true;
    });

    auto cache = sPlayerSystem.GetPlayerCache();
    if (cache)
    {
        LogSYSEx
        (
            trace_memory_file_name,
            u8"$size${}$alloc${}$free${}$remain${}$name${}$trace${}:{}.",
            cache->Size() * sizeof(PlayerData),
            cache->Size(),
            0,
            cache->Size(),
            u8"PlayerData",
            u8"",
            0
        );
    }
    LogSYSEx(trace_memory_file_name, u8"***end cache trace***");
#endif

    sLogSystem.DestroySpecifiedLogger(trace_memory_file_name);

    LogSYS(u8"end dump memory pool [ok].");
}

void AppUtil::TidyMemoryPool(void)
{
    FUNC_PERFORMANCE_CHECK();

    size_t before_total_size = memory_total_size();
    size_t before_alloc_size = memory_alloc_size();

    LogSYS(u8"{:*^30}", u8"begin memory tidy");
    LogSYS(u8"before tidy alloc= {} total= {} utility= {}%",
        AppUtil::memory_size_format(before_alloc_size),
        AppUtil::memory_size_format(before_total_size),
        before_alloc_size * 100 / before_total_size);

    memory_tidy();

    size_t after_total_size = memory_total_size();
    size_t after_alloc_size = memory_alloc_size();

    LogSYS(u8"after tidy alloc= {} total= {} utility= {}%",
        AppUtil::memory_size_format(after_alloc_size),
        AppUtil::memory_size_format(after_total_size),
        after_alloc_size * 100 / after_total_size);

    LogSYS(u8"release= {}", AppUtil::memory_size_format(before_total_size - after_total_size));
    LogSYS(u8"{:*^30}", u8"end memory tidy");
}

void AppUtil::LogMemoryPoolIncrement(const std::string& prefix)
{
    FUNC_PERFORMANCE_CHECK();
    LogSYS(u8"begin dump memory pool increment [wait].");

    std::string trace_memory_file_name = prefix;
    trace_memory_file_name += u8"_";
    trace_memory_file_name += MEMORY_INCREMENT_NAME;

    LogResetEx(trace_memory_file_name);

    HRBNODE mem_info_node = mem_trace_info_head();
    while (mem_info_node)
    {
        mem_trace_info* trace_info = (mem_trace_info*)rb_node_key_user(mem_info_node);
        mem_info_node = rb_next(mem_info_node);

        mem_trace_info* dump_trace_info = get_dump_mem_trace_info(trace_info);

        std::string stack_string;

        if (trace_info->stack)
        {
            for (int i = 0; i < trace_info->stack->m_stack_size; i++)
            {
                if (i)
                {
                    stack_string += "->";
                    stack_string += trace_info->stack->m_stack[i]->func_name;
                }
                else
                {
                    stack_string = trace_info->stack->m_stack[i]->func_name;
                }
            }
        }

        if (dump_trace_info)
        {
            LogSYSEx(
                trace_memory_file_name,
                u8"$increment${}$size${}$alloc${}$free${}$remain${}$name${}$trace${}:{}$stack${}.",
                (int64_t)((trace_info->alloc - trace_info->free) - (dump_trace_info->alloc - dump_trace_info->free)),
                trace_info->size,
                trace_info->alloc,
                trace_info->free,
                trace_info->alloc - trace_info->free,
                trace_info->name,
                trace_info->file,
                trace_info->line,
                stack_string
            );
        }
        else
        {
            LogSYSEx(
                trace_memory_file_name,
                u8"$increment${}$size${}$alloc${}$free${}$remain${}$name${}$trace${}:{}$stack${}.",
                trace_info->alloc - trace_info->free,
                trace_info->size,
                trace_info->alloc,
                trace_info->free,
                trace_info->alloc - trace_info->free,
                trace_info->name,
                trace_info->file,
                trace_info->line,
                stack_string
            );
        }
    }

#ifdef IsDatabaseServer
    LogSYSEx(trace_memory_file_name, u8"***begin cache trace***");
    CacheQueueBuilder::TraverseTrace([&](const std::string &name, 
        const CacheQueueBuilder::CacheQueueTraceData &data)
        {
            LogSYSEx
            (
                trace_memory_file_name,
                u8"$size${}$alloc${}$free${}$remain${}$name${}$trace${}:{}.",
                data.TotalSize,
                data.Alloc,
                data.Free,
                data.Remain,
                name,
                u8"",
                0
            );
            return true;
        });

    auto cache = sPlayerSystem.GetPlayerCache();
    if (cache)
    {
        LogSYSEx
        (
            trace_memory_file_name,
            u8"$size${}$alloc${}$free${}$remain${}$name${}$trace${}:{}.",
            cache->Size() * sizeof(PlayerData),
            cache->Size(),
            0,
            cache->Size(),
            u8"PlayerData",
            u8"",
            0
        );
    }
    LogSYSEx(trace_memory_file_name, u8"***end cache trace***");
#endif

    sLogSystem.DestroySpecifiedLogger(trace_memory_file_name);

    LogSYS(u8"end dump memory pool increment [ok].");

    dump_mem_trace_info();
}

std::string AppUtil::memory_size_format(size_t mem_size)
{
    FUNC_PERFORMANCE_CHECK();

    if (mem_size >= 1024 * 1024 * 1024)
    {
        size_t integer_part = mem_size / (1024 * 1024 * 1024);
        size_t fraction_part = (mem_size % (1024 * 1024 * 1024)) * 100 / (1024 * 1024 * 1024);

        return fmt::format(u8"{}.{}GB", integer_part, fraction_part);
    }
    else if (mem_size >= 1024 * 1024)
    {
        size_t integer_part = mem_size / (1024 * 1024);
        size_t fraction_part = (mem_size % (1024 * 1024)) * 100 / (1024 * 1024);

        return fmt::format(u8"{}.{}MB", integer_part, fraction_part);
    }
    else if (mem_size >= 1024)
    {
        size_t integer_part = mem_size / (1024);
        size_t fraction_part = (mem_size % (1024)) * 100 / (1024);

        return fmt::format(u8"{}.{}KB", integer_part, fraction_part);
    }
    else
    {
        return fmt::format(u8"{}B", mem_size);
    }
}

bool AppUtil::IsDigit(const std::string& s)
{
    for (char c : s) {
        if (c >= '0' && c <= '9') continue;

        return false;
    }

    return true;
}

const char* AppUtil::CompiledDate(void)
{
    static char fmt_cp_date_time[64] = { 0 };

    if (!fmt_cp_date_time[0])
    {
        int month = 0;
        static const char* static_month_buf[] = {
        "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
        const char* cp_date = __DATE__;
        memcpy(fmt_cp_date_time, cp_date + 7, 4);
        for (int i = 0; i < 12; i++)
        {
            if (memcmp(static_month_buf[i], cp_date, 3) == 0)
            {
                month = i + 1;
                break;
            }
        }

        fmt_cp_date_time[4] = '-';
        fmt_cp_date_time[5] = month / 10 % 10 + '0';
        fmt_cp_date_time[6] = month % 10 + '0';
        fmt_cp_date_time[7] = '-';

        if (cp_date[4] == ' ') {
            fmt_cp_date_time[8] = '0';
        }
        else {
            fmt_cp_date_time[8] = cp_date[4];
        }
        fmt_cp_date_time[9] = cp_date[5];
    }

    return fmt_cp_date_time;
}

const char* AppUtil::CompiledDateEx(void)
{
    static char fmt_cp_date_time[64] = { 0 };

    if (!fmt_cp_date_time[0])
    {
        int month = 0;
        static const char* static_month_buf[] = {
        "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
        const char* cp_date = __DATE__;
        const char* cp_time = __TIME__;
        memcpy(fmt_cp_date_time, cp_date + 7, 4);
        for (int i = 0; i < 12; i++)
        {
            if (memcmp(static_month_buf[i], cp_date, 3) == 0)
            {
                month = i + 1;
                break;
            }
        }

        //fmt_cp_date_time[4] = '-';
        fmt_cp_date_time[4] = month / 10 % 10 + '0';
        fmt_cp_date_time[5] = month % 10 + '0';
        //fmt_cp_date_time[7] = '-';

        if (cp_date[4] == ' ') {
            fmt_cp_date_time[6] = '0';
        }
        else {
            fmt_cp_date_time[6] = cp_date[4];
        }
        fmt_cp_date_time[7] = cp_date[5];

        fmt_cp_date_time[8] = '-';
        
        fmt_cp_date_time[9] = cp_time[0];
        fmt_cp_date_time[10] = cp_time[1];
        fmt_cp_date_time[11] = cp_time[3];
        fmt_cp_date_time[12] = cp_time[4];
        fmt_cp_date_time[13] = cp_time[6];
        fmt_cp_date_time[14] = cp_time[7];
    }

    return fmt_cp_date_time;
}

#ifdef _MSC_VER
#include <windows.h>

int32_t AppUtil::MultiByteToUnicode(unsigned int code_page, const char* src, int src_len, wchar_t* dst, int dst_size)
{
    FUNC_PERFORMANCE_CHECK();

    return MultiByteToWideChar(code_page, 0, src, src_len, dst, dst_size);
}

int32_t AppUtil::UnicodeToMultiByte(unsigned int code_page, const wchar_t* src, int src_len, char* dst, int dst_size)
{
    FUNC_PERFORMANCE_CHECK();

    return WideCharToMultiByte(code_page, 0, src, src_len, dst, dst_size, 0, 0);
}

int32_t AppUtil::MultiByteToUnicodeLength(unsigned int code_page, const char* src, int src_len)
{
    FUNC_PERFORMANCE_CHECK();

    return MultiByteToWideChar(code_page, 0, src, src_len, 0, 0);
}

int32_t AppUtil::UnicodeToMultiByteLength(unsigned int code_page, const wchar_t* src, int src_len)
{
    FUNC_PERFORMANCE_CHECK();

    return WideCharToMultiByte(code_page, 0, src, src_len, 0, 0, 0, 0);
}

bool AppUtil::MultiByteToOther(unsigned int src_code_page, const std::string& src_str, unsigned int dst_code_page, std::string& dst_str)
{
#define CACHE_SIZE 2048
#define PER_TRANS_SIZE 512
    dst_str.clear();
    wchar_t unicode_cache[CACHE_SIZE];
    char dst_cache[CACHE_SIZE];
    const char* ptr = src_str.c_str();
    const char* end = ptr + src_str.length();
    int per_trans = PER_TRANS_SIZE;
    while (ptr < end)
    {
        if (ptr + per_trans < end)
        {
            int32_t ret = MultiByteToWideChar(
                src_code_page,
                MB_ERR_INVALID_CHARS,
                ptr,
                per_trans,
                unicode_cache,
                CACHE_SIZE);
            if (!ret)
            {
                if (GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
                {
                    per_trans--;
                    if (per_trans > 0)
                    {
                        continue;
                    }
                }
                return false;
            }

            ret = WideCharToMultiByte(
                dst_code_page,
                0,
                unicode_cache,
                ret,
                dst_cache,
                CACHE_SIZE,
                0,
                0);
            if (!ret)
            {
                return false;
            }

            dst_str.append(dst_cache, ret);

            ptr += per_trans;
            per_trans = PER_TRANS_SIZE;
        }
        else
        {
            int32_t ret = MultiByteToWideChar(
                src_code_page,
                0,
                ptr,
                (int32_t)(end - ptr),
                unicode_cache,
                CACHE_SIZE);
            if (!ret)
            {
                return false;
            }

            ret = WideCharToMultiByte(
                dst_code_page,
                0,
                unicode_cache,
                ret,
                dst_cache,
                CACHE_SIZE,
                0,
                0);
            if (!ret)
            {
                return false;
            }

            dst_str.append(dst_cache, ret);

            return true;
        }
    }

    return false;
}

bool AppUtil::ReportDingDing(const std::string& msg)
{
    STARTUPINFO         si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfo(&si);
    si.wShowWindow = SW_SHOWNORMAL;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.lpTitle = nullptr;

    std::string args = u8"./\"DingDing.exe\" ";
    args += u8"\"";
    args += msg;
    args += u8"\"";

    std::string gbk_args;

    AppUtil::Utf8ToDefault(args, gbk_args);

    if (!CreateProcess("./DingDing.exe", (LPSTR)gbk_args.c_str(), NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, "./", &si,
        &pi))  //创建子进程
    {
        return false;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

#elif __GNUC__

class iconverter
{
public:
    iconverter(const std::string& out_encode,
        const std::string& in_encode)
    {
        iconv_ = ::iconv_open(out_encode.c_str(), in_encode.c_str());
    }

    ~iconverter()
    {
        if (iconv_ != (iconv_t)-1)
        {
            iconv_close(iconv_);
            iconv_ = (iconv_t)-1;
        }
    }

    bool convert(const std::string& input, std::string& output) const
    {
        std::vector<char> in_buf(input.begin(), input.end());
        char* src_ptr = &in_buf[0];
        size_t src_size = input.size();

        std::vector<char> buf(src_size * 6);
        std::string dst;

        while (0 < src_size)
        {
            char* dst_ptr = &buf[0];
            size_t dst_size = buf.size();
            size_t res = ::iconv(iconv_, &src_ptr, &src_size, &dst_ptr, &dst_size);

            if (res == (size_t)-1)
            {
                if (errno == E2BIG)
                {
                    // ignore this error
                }
                else
                {
                    return false;
                }
            }

            dst.append(&buf[0], buf.size() - dst_size);
        }

        dst.swap(output);

        return true;
    }

private:

    iconv_t iconv_;
};

bool AppUtil::MultiByteToOther(const std::string& input_code, const std::string& input, const std::string& output_code, std::string& output)
{
    iconverter icv(output_code, input_code);

    return icv.convert(input, output);
}

#else
#error "unknown compiler"
#endif

bool AppUtil::DefaultToUtf8(const std::string& src, std::string& dst)
{
    FUNC_PERFORMANCE_CHECK();
#ifdef _MSC_VER
    return AppUtil::MultiByteToOther(CP_ACP, src, CP_UTF8, dst);
#else
    dst = src;
    return true;
#endif
}

bool AppUtil::Utf8ToDefault(const std::string& src, std::string& dst)
{
    FUNC_PERFORMANCE_CHECK();
#ifdef _MSC_VER
    return AppUtil::MultiByteToOther(CP_UTF8, src, CP_ACP, dst);
#else
    dst = src;
    return true;
#endif
}

int32_t AppUtil::RandomPick(const std::vector<uint32_t>& probs)
{
    uint32_t total = 0;
    for (size_t i = 0; i < probs.size(); i++)
    {
        total += probs[i];
    }

    int32_t idx = 0;
    uint32_t hit = (uint32_t)rand_integer(0, total);
    while (hit > 0)
    {
        if (hit <= probs[idx])
        {
            break;
        }
        hit -= probs[idx];
        idx++;
    }

    return idx;
}

void AppUtil::SplitPath(const std::string &fullpath, 
    std::string &path, std::string &file)
{
#ifdef _MSC_VER
    char drive[MAX_PATH] = { 0 };
    char dir[MAX_PATH] = { 0 };
    char name[MAX_PATH] = { 0 };
    char ext[MAX_PATH] = { 0 };
    _splitpath_s(fullpath.c_str(), drive, dir, name, ext);

    path = std::string(drive) + "/" + dir;
    file = std::string(name) + ext;
#elif __GNUC__
    char sz_full_path[MAX_PATH];
    char sz_full_path2[MAX_PATH];
    StrSafeCopy(sz_full_path, fullpath);
    StrSafeCopy(sz_full_path2, fullpath);

    path = dirname(sz_full_path);
    file = basename(sz_full_path2);
#else
#error "unknown compiler"
#endif
}

