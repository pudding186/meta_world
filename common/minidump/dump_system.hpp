#pragma once

#ifdef _MSC_VER
#include <windows.h>
#include <imagehlp.h>

//class ExceptionSystem
//{
//public:
//    ExceptionSystem(const TCHAR* pre = 0);
//    ~ExceptionSystem();
//
//public:
//    static LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS p);
//
//    static bool GenMiniDump(PEXCEPTION_POINTERS p);
//
//    static void GenExceptionReport(PEXCEPTION_POINTERS p);
//
//private:
//    static char* m_memCache;
//    static LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
//    static TCHAR m_dumpName[MAX_PATH];
//    static bool gen_dump;
//};
//
//extern ExceptionSystem g_ExceptionSystem;

class DumpSystem
{
public:
    DumpSystem(const TCHAR* pre = 0);
    ~DumpSystem();

    static bool GenMiniDump(PEXCEPTION_POINTERS ei);
protected:
    static LONG WINAPI _MyUnhandledExceptionFilter(PEXCEPTION_POINTERS ei);
private:
    LPTOP_LEVEL_EXCEPTION_FILTER m_pre_filter;
    TCHAR m_dump_pre[MAX_PATH];
};

#endif

