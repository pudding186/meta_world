#ifdef _MSC_VER
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "version.h"
#include "dump_system.hpp"
#pragma comment(lib, "Dbghelp.lib")
#define ST_FULL_DUMP

#ifdef STRPRODUCTVER
DumpSystem g_DumpSystem(STRPRODUCTVER);
#else
DumpSystem g_DumpSystem;
#endif

static void DisableSetUnhandledExceptionFilter(void)
{
    void* addr = (void*)GetProcAddress
    (
        LoadLibrary(_T(u8"kernel32.dll")),
        u8"SetUnhandledExceptionFilter"
    );
    if (addr)
    {
        unsigned char code[16];
        int32_t size = 0;

#ifdef _WIN64
        code[size++] = 0x48;
        code[size++] = 0x33;
        code[size++] = 0xC0;
        code[size++] = 0xC3;
#else
        code[size++] = 0x33;
        code[size++] = 0xC0;
        code[size++] = 0xC2;
        code[size++] = 0x04;
#endif
        code[size++] = 0x00;

        DWORD old_flag, temp_flag;
        VirtualProtect
        (
            addr,
            size,
            PAGE_EXECUTE_READWRITE,
            &old_flag
        );

        WriteProcessMemory
        (
            GetCurrentProcess(),
            addr,
            code,
            size,
            0
        );

        VirtualProtect
        (
            addr,
            size,
            old_flag,
            &temp_flag
        );
    }
}

DumpSystem::DumpSystem(const TCHAR* pre /*= 0*/)
{
    m_pre_filter = SetUnhandledExceptionFilter(_MyUnhandledExceptionFilter);

    TCHAR module_name[MAX_PATH];
    GetModuleFileName(0, module_name, MAX_PATH);

    PTSTR dot = _tcsrchr(module_name, _T('.'));
    if (dot)
    {
        *dot = 0;
    }

    if (pre)
    {
        _sntprintf_s(m_dump_pre, sizeof(m_dump_pre), _TRUNCATE, _T("%s_%s"), module_name, pre);
    }
    else
    {
        _sntprintf_s(m_dump_pre, sizeof(m_dump_pre), _TRUNCATE, _T("%s"), module_name);
    }

    DisableSetUnhandledExceptionFilter();
}

DumpSystem::~DumpSystem()
{
    SetUnhandledExceptionFilter(m_pre_filter);
}

bool DumpSystem::GenMiniDump(PEXCEPTION_POINTERS ei)
{
    static bool gen_mini_dump = false;

    if (gen_mini_dump)
    {
        return true;
    }
#ifndef _DEBUG
    SetErrorMode(SEM_NOGPFAULTERRORBOX);
#endif

    TCHAR name_buf[MAX_PATH] = { 0 };
    TCHAR time_buf[32] = { 0 };
    time_t times = time(0);
    struct tm stm;
    localtime_s(&stm, &times);
    _tcsftime(time_buf, sizeof(time_buf) - 1, _T("%Y-%m-%d_%H_%M_%S"), &stm);

#ifdef PRODUCTVER
    _sntprintf_s(name_buf, sizeof(name_buf), _TRUNCATE, _T("%s_%s-%s.dmp"), g_DumpSystem.m_dump_pre, STRPRODUCTVER, time_buf);
#else
    _sntprintf_s(name_buf, sizeof(name_buf), _TRUNCATE, _T("%s_%s.dmp"), g_DumpSystem.m_dump_pre, time_buf);
#endif

    HANDLE handle = CreateFile
    (
        name_buf,
        GENERIC_WRITE,
        0,
        0,
        OPEN_ALWAYS,
        FILE_FLAG_WRITE_THROUGH,
        0
    );
    if (handle != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION info;
        info.ThreadId = GetCurrentThreadId();
        info.ExceptionPointers = ei;
        info.ClientPointers = FALSE;

        MINIDUMP_TYPE dumptype = (MINIDUMP_TYPE)(MiniDumpWithDataSegs);

#ifdef ST_FULL_DUMP
        dumptype = (MINIDUMP_TYPE)(MiniDumpWithFullMemory | MiniDumpWithHandleData);
#if API_VERSION_NUMBER >= 11
        dumptype = (MINIDUMP_TYPE)(dumptype | MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo |
            MiniDumpWithUnloadedModules);
#endif
#endif


        BOOL ret = MiniDumpWriteDump
        (
            GetCurrentProcess(),
            GetCurrentProcessId(),
            handle,
            dumptype,
            &info,
            0,
            0
        );

        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;

        if (ret)
        {
            gen_mini_dump = true;
            return true;
        }
    }

    return false;
}

LONG WINAPI DumpSystem::_MyUnhandledExceptionFilter(PEXCEPTION_POINTERS ei)
{
    if (!GenMiniDump(ei))
    {
        if (g_DumpSystem.m_pre_filter)
        {
            return g_DumpSystem.m_pre_filter(ei);
        }
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

#endif

