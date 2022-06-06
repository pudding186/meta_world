#pragma once
#include "./lib_svr_def.h"
#include "./format_args.hpp"
#include "./special_decay_type.hpp"

#ifdef __cplusplus
extern "C" {
#endif

extern bool (init_logger_manager)(size_t log_thread_num, size_t max_log_event_num, size_t print_cache_size);

extern void (uninit_logger_manager)(void);

extern HFILELOGGER (create_file_logger)(const char* path_utf8, const char* name_utf8);

extern void (destroy_file_logger)(HFILELOGGER file_logger);

extern void (reset_file_logger)(HFILELOGGER file_logger);

extern void (file_logger_flush)(HFILELOGGER file_logger);

extern HLOOPCACHE (logger_manager_print_cache)(void);

#ifdef __cplusplus
}

extern bool file_logger_async_log(HFILELOGGER file_logger, print_color color, const char* prefix, SFormatArgs<>* fmt_args, bool is_block);

//template<size_t N, typename P, typename... Args>
//void file_logger_print(HFILELOGGER file_logger, const char(&fmt)[N], Args&&... args)
//{
//    using LogFormatArgs = SFormatArgs<const char(&)[N], special_decay_type<Args>...>;
//
//    SFormatArgs<>* fmt_args = S_NEW(LogFormatArgs, 1, fmt, (special_decay_type<Args>&&)(args)...);
//
//    file_logger_async_log(file_logger, true, P::prefix, fmt_args, true);
//}

template<typename P, size_t N, typename... Args>
void file_logger_log(HFILELOGGER file_logger, const char(&fmt)[N], Args&&... args)
{
    using LogFormatArgs = SFormatArgs<const char(&)[N], special_decay_type<Args>...>;

    SFormatArgs<>* fmt_args = S_NEW(LogFormatArgs, 1, fmt, (special_decay_type<Args>&&)(args)...);

    file_logger_async_log(file_logger, P::color, P::prefix, fmt_args, true);
}

#endif