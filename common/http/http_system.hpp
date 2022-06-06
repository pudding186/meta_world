#pragma once
#include "curl/curl.h"
#include "curl/easy.h"
#include "meyers_singleton.hpp"
#include "utility.hpp"
#include <map>
#include <string>
#include <thread>

using HeaderMap = std::map<std::string, std::string>;

class IHttpTask {
public:
    IHttpTask(std::string url, std::string post_data = "", unsigned int time_out = 60)
        : m_url(std::move(url)), m_post_data(std::move(post_data)), m_timeout(time_out) {};
    virtual ~IHttpTask() = default;

public:
    virtual size_t write_data(void* buffer, size_t size, size_t nmemb)
    {
        m_result.append((char*)buffer, size * nmemb);
        return size * nmemb;
    }

    virtual void   OnResult(CURLcode code) = 0;
    virtual void   OnRelease() = 0;

    void        SetHeaders(const HeaderMap& headers);
    curl_slist* GetCurlHeaderList() { return m_headers; }
    void        CleanCurlRes();

private:
    IHttpTask(const IHttpTask&) = delete;
    IHttpTask& operator=(const IHttpTask&) = delete;

private:
    DEFREFMEMBER(std::string, m_url, URL)
    DEFREFMEMBER(std::string, m_post_data, PostData)
    DEFREFMEMBER(std::string, m_result, Result)
    DEFMEMBER(CURLcode, m_code, URLCode)
    DEFMEMBER(unsigned int, m_timeout, TimeOut)
    curl_slist* m_headers {nullptr};
};

class CHttpWork
{
public:
    bool Start();
    void Stop();

    void ThreadProc();

    bool AsyncURLRequest(IHttpTask* task);

    bool ProcResult();

    static size_t write_data(void* buffer, size_t size, size_t nmemb, void* stream);

private:
    bool m_is_run;
    bool m_is_closing_;
    bool m_is_thread_over_;

    HLOOPCACHE m_req_list;
    HLOOPCACHE m_ack_list;

    std::thread m_threadHttp;
};

class HttpSystem
{
public:
    HttpSystem() = default;
    ~HttpSystem() = default;

    bool Start();
    void Stop();

    bool Run(CFuncPerformanceInfo** info);

    bool AsyncURLRequest(IHttpTask* task);

private:
    CHttpWork*  m_work_lis;
    int         m_work_num;
};

#define sHttpSystem MeyersSingleton<HttpSystem>::Instance()

//////////////////////////////////////////////////////////////////////////


//class LuaHttpTask:public IHttpTask
//{
//public:
//    LuaHttpTask(
//        const std::string& url,
//        const std::string& content,
//        const std::string& meta,
//        const std::string& callback,
//        const std::string& params,
//        unsigned int timeout):
//        IHttpTask(url, content, timeout), 
//        m_meta(meta), m_callback(callback), m_params(params){}
//
//    void   OnResult(CURLcode code) override;
//    void   OnRelease() override;
//
//public:
//    DEFREFMEMBER(std::string, m_meta, Meta)
//    DEFREFMEMBER(std::string, m_callback, CallBack)
//    DEFREFMEMBER(std::string, m_params, Params)
//private:
//};
//
//class CrossHttpTask:public IHttpTask
//{
//public:
//    CrossHttpTask(
//        const std::string& url, 
//        UINT32 zone_id):
//        IHttpTask("", "", 30),
//        m_zone_id(zone_id)
//    {
//        set_URL(url + fmt::format(u8"?act=getgatebysid&sid={}", m_zone_id));
//    }
//    void   OnResult(CURLcode code) override;
//    void   OnRelease() override;
//protected:
//    DEFMEMBER(UINT32, m_zone_id, ZoneId)
//private:
//};
