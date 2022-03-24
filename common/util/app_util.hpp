#pragma once
#include <type_traits>
#include <string>
#include <vector>
#include <string.h>
class AppUtil
{
public:
    template <
        typename T
    >
    static std::enable_if<std::is_pod<T>::value> MemSafeCopy(T &arg1, const T &arg2)
    {
        memcpy((void *)&arg1, (const void *)&arg2, sizeof(T));
    }

    template <
        typename T
    >
    static std::enable_if<std::is_pod<T>::value> MemSafeCopy(T *arg1, const T *arg2)
    {
        memcpy((void *)arg1, (const void *)arg2, sizeof(T));
    }

    template <
        typename T,
        int32_t S
    >
    static std::enable_if<std::is_pod<T>::value> MemSafeCopy(T(&arg1)[S], const T *arg2, int32_t l)
    {
        l = std::min(S, l);
        memcpy((void *)arg1, (const void *)arg2, sizeof(T) * l);
    }

    template <
        typename T,
        int32_t S1,
        int32_t S2
    >
    static std::enable_if<std::is_pod<T>::value> MemSafeCopy(T(&arg1)[S1], T(&arg2)[S2])
    {
        int32_t l = std::min(S1, S2);
        memcpy((void *)arg1, (const void *)arg2, sizeof(T) * l);
    }

    template <
        typename T
    >
    static void MemSafeZero(T &arg)
    {
        memset(&arg, 0, sizeof(T));
    }

    template <
        typename T
    >
    static void MemSafeZero(T *arg, int32_t l)
    {
        memset(arg, 0, sizeof(T) * l);
    }

    template <
        typename T,
        int32_t S
    >
    static void MemSafeZero(T(&arg)[S])
    {
        memset(arg, 0, sizeof(T) * S);
    }

    template <
        typename FN
    >
    static void StrSplit(const std::string &str, 
        const std::string &split, const FN &fn)
    {
        std::string s = str;
        const char *p = strtok((char *)s.c_str(), split.c_str());
        while (p)
        {
            fn(p);
            p = strtok(0, split.c_str());
        }
    }

    static void StrSplit(const std::string& str,
        const std::string& split, std::vector<std::string>& results)
    {
        results.clear();
        StrSplit(str, split, [&](const char* p) {
            results.push_back(p);
        });
    }

    template <
        typename FN
    >
    static void StrSplitEx(const std::string& str,
        const std::string& split, const FN& fn)
    {
        size_t pos = 0;
        size_t last_pos = 0;
        size_t str_size = str.size();
        size_t split_size = split.size();
        if (!str_size || !split_size)
        {
            return;
        }
        std::string sub_str;
        while (true)
        {
            pos = str.find(split, pos);
            if (pos != std::string::npos)
            {
                if (0 != last_pos || 0 != pos)
                {
                    sub_str = str.substr(last_pos, pos - last_pos);
                    fn(sub_str.data());
                }
                pos += split_size;
                last_pos = pos;
            }
            else
            {
                if (str_size != last_pos)
                {
                    sub_str = str.substr(last_pos, str_size - last_pos);
                    fn(sub_str.data());
                }
                break;
            }
        }
    }

    static void StrSplitEx(const std::string& str,
        const std::string& split, std::vector<std::string>& results)
    {
        results.clear();
        StrSplitEx(str, split, [&](const char* p) {
            results.push_back(p);
            });
    }

    //static char *UI642A64(uint64_t value, char *buf, int32_t count);

    //static const char* A642UI64(const std::string &s, uint64_t &value);
    static char* UI642A64(uint64_t value, char* buf, size_t buf_size);
    static const char* A642UI64(const char* a64, size_t a64_len, uint64_t& value);

    static bool A642UI64Ex(const std::string& s, uint64_t& value);

    static bool Str2Digit(const std::string &str, int32_t &digit);

    //static std::string IPN2Str(uint32_t ip);

    template<size_t N>
    static bool Bin2Str(uint8_t* data, int32_t l, char(&buf)[N])
    {
        return bin2str(data, l, buf, N);
    }

    template<size_t N>
    static bool Str2Bin(const char* str, char(&buf)[N])
    {
        return str2bin(str, buf, N);
    }

    static void TraceMemoryPool(const std::string& prefix);

    static void LogMemoryPool(size_t limit);

    static void TidyMemoryPool(void);

    static void LogMemoryPoolIncrement(const std::string& prefix);

    static std::string memory_size_format(size_t mem_size);

    static bool IsDigit(const std::string& s);

    static const char* CompiledDate(void);

    static const char* CompiledDateEx(void);

#ifdef _MSC_VER
    static int32_t MultiByteToUnicode(unsigned int code_page, const char* src, int src_len, wchar_t* dst, int dst_size);
    
    static int32_t UnicodeToMultiByte(unsigned int code_page, const wchar_t* src, int src_len, char* dst, int dst_size);
    
    static int32_t MultiByteToUnicodeLength(unsigned int code_page, const char* src, int src_len);
    
    static int32_t UnicodeToMultiByteLength(unsigned int code_page, const wchar_t* src, int src_len);

    static bool MultiByteToOther(unsigned int src_code_page, const std::string& src_str, unsigned int dst_code_page, std::string& dst_str);

    static bool ReportDingDing(const std::string& msg);
#elif __GNUC__
    static bool MultiByteToOther(const std::string& input_code, const std::string& input, const std::string& output_code, std::string& output);
#else
#error "unknown compiler"
#endif

    static bool DefaultToUtf8(const std::string &src, std::string &dst);

    static bool Utf8ToDefault(const std::string &src, std::string &dst);

    static int32_t RandomPick(const std::vector<uint32_t>& probs);

    static void SplitPath(const std::string &fullpath, std::string &path, std::string &file);
};
