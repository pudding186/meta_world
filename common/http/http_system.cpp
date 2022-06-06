#include "http_system.hpp"
#include "log_system.hpp"
#include "loop_cache.h"
#include <functional>

static CURL* GenCURLHandle(IHttpTask* task) {
    CURL* curl = curl_easy_init();
    auto* headers = task->GetCurlHeaderList();

    if (task->get_PostData().length()) {
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, task->get_PostData().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, task->get_PostData().length());
    }

    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_URL, task->get_URL().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CHttpWork::write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, task);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, task->get_TimeOut());
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);

    // trim left
    std::string s = task->get_URL();
    //s.erase(s.begin(),
    //    std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c); }));

#ifdef _MSC_VER
    if (0 == _strnicmp(s.c_str(), "https://", 8)) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    }
#elif __GNUC__
    if (0 == strncasecmp(s.c_str(), "https://", 8)) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    }
#else
#error "unknown compiler"
#endif

    return curl;
}

void IHttpTask::SetHeaders(const HeaderMap& headers) {
    if (m_headers != nullptr) {
        curl_slist_free_all(m_headers);
        m_headers = nullptr;
    }
    for (const auto& head : headers) {
        m_headers =
            curl_slist_append(m_headers, fmt::format("{}:{}", head.first, head.second).c_str());
    }
}

void IHttpTask::CleanCurlRes() {
    if (m_headers != nullptr) {
        curl_slist_free_all(m_headers);
        m_headers = nullptr;
    }
}

bool CHttpWork::Start()
{
    FUNC_PERFORMANCE_CHECK();

    m_is_run = true;
    m_is_closing_ = false;
    m_is_thread_over_ = true;

    m_req_list = create_loop_cache(nullptr, 64 * 1024 * sizeof(IHttpTask*));
    m_ack_list = create_loop_cache(nullptr, 64 * 1024 * sizeof(IHttpTask*));

    m_threadHttp = std::thread(&CHttpWork::ThreadProc, this);

    return true;
}

void CHttpWork::Stop()
{
    FUNC_PERFORMANCE_CHECK();
    m_is_run = false;
    m_is_closing_ = true;

    if (m_threadHttp.get_id() != std::this_thread::get_id())
    {
        if (m_threadHttp.joinable())
        {
            m_threadHttp.join();
        }
    }
}

void CHttpWork::ThreadProc()
{
    m_is_thread_over_ = false;

    // init curl content
    CURLM* curlmHandle = curl_multi_init();
    std::map<CURL*, IHttpTask*> coTaskMap;
    int                         runningHandles = 0;
    CURLMcode                   mcode = CURLM_OK;
    int                         rc = 0;  // select return code

    while (m_is_run) {
        if (runningHandles > 0) {
            /* get file descriptors from the transfers */
            fd_set fdread;
            fd_set fdwrite;
            fd_set fdexcep;
            int    maxfd = -1;

            FD_ZERO(&fdread);
            FD_ZERO(&fdwrite);
            FD_ZERO(&fdexcep);

            struct timeval timeout;
            /* set a suitable timeout to play around with */
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            // get timeout setting from multi-handle
            long curl_timeo = -1;
            curl_multi_timeout(curlmHandle, &curl_timeo);

            if (curl_timeo >= 0) {
                timeout.tv_sec = curl_timeo / 1000;

                if (timeout.tv_sec > 1) {
                    timeout.tv_sec = 1;
                }
                else {
                    timeout.tv_usec = (curl_timeo % 1000) * 1000;
                }
            }

            mcode = curl_multi_fdset(curlmHandle, &fdread, &fdwrite, &fdexcep, &maxfd);
            if (CURLM_OK == mcode) {
                // do wait action
                if (maxfd == -1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    rc = 0;
                }
                else {
                    rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
                }

                if (rc < 0) {
                    LogERR("<%s>: select return unexpect code: %d", __FUNCTION__, rc);
                }
            }
        }

        if (coTaskMap.size() > 0) {
            mcode = curl_multi_perform(curlmHandle, &runningHandles);

            if (CURLM_OK == mcode) {
                struct CURLMsg* m;
                do {
                    int msgq = 0;
                    m = curl_multi_info_read(curlmHandle, &msgq);
                    if (m && (m->msg == CURLMSG_DONE)) {
                        CURL* curlHandle = m->easy_handle;
                        CURLcode errCode = m->data.result;

                        IHttpTask* done_task = coTaskMap[curlHandle];

                        // remove from multi-handle
                        curl_multi_remove_handle(curlmHandle, curlHandle);

                        done_task->set_URLCode(errCode);

                        loop_cache_push_data(m_ack_list, &done_task, sizeof(IHttpTask*));

                        done_task->CleanCurlRes();
                        curl_easy_cleanup(curlHandle);

                        // remove from coTaskMap
                        coTaskMap.erase(curlHandle);
                    }
                } while (m);
            }
        }

        while (coTaskMap.size() < 32) {
            IHttpTask* task = nullptr;

            if (loop_cache_pop_data(m_req_list, &task, sizeof(IHttpTask*))) {
                CURL* curlHandle = GenCURLHandle(task);

                // add curl handle to process list
                mcode = curl_multi_add_handle(curlmHandle, curlHandle);
                if (CURLM_OK != mcode) {
                    task->set_URLCode(CURLE_FAILED_INIT);
                    loop_cache_push_data(m_ack_list, &task, sizeof(IHttpTask*));

                    continue;
                }

                coTaskMap[curlHandle] = task;
            }
            else {
                break;
            }
        }

        if (coTaskMap.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    auto it = coTaskMap.begin();
    for (; it != coTaskMap.end(); it++) {
        CURL* curlHandle = it->first;
        IHttpTask* task = it->second;
        // remove from multi-handle
        curl_multi_remove_handle(curlmHandle, curlHandle);

        task->set_URLCode(CURLE_RECV_ERROR);

        loop_cache_push_data(m_ack_list, &task, sizeof(IHttpTask*));

        curl_easy_cleanup(curlHandle);
    }

    curl_multi_cleanup(curlmHandle);

    m_is_thread_over_ = true;
}

bool CHttpWork::AsyncURLRequest(IHttpTask* task)
{
    return loop_cache_push_data(m_req_list, &task, sizeof(IHttpTask*));
}

bool CHttpWork::ProcResult()
{
    IHttpTask* task = nullptr;

    if (loop_cache_pop_data(m_ack_list, &task, sizeof(IHttpTask*))) {
        task->OnResult(task->get_URLCode());
        task->OnRelease();

        return true;
    }

    return false;
}

size_t CHttpWork::write_data(void* buffer, size_t size, size_t nmemb, void* stream)
{
    return static_cast<IHttpTask*>(stream)->write_data(buffer, size, nmemb);
}

bool HttpSystem::Start()
{
    if (curl_global_init(CURL_GLOBAL_ALL)) {
        LogERR("init libcurl fali!");
        return false;
    }

    m_work_num = 1;
    m_work_lis = new CHttpWork[m_work_num];

    for (int i = 0; i < m_work_num; i++) {
        if (!m_work_lis[i].Start()) {
            for (int j = 0; j < i; j++) {
                m_work_lis[j].Stop();
            }

            return false;
        }
    }

    return true;
}

void HttpSystem::Stop()
{
    for (int i = 0; i < m_work_num; i++) {
        m_work_lis[i].Stop();
    }

    delete[] m_work_lis;
    m_work_lis = nullptr;
}

bool HttpSystem::Run(CFuncPerformanceInfo** info)
{
    FUNC_PERFORMANCE_CHECK();
    *info = &s_func_perf_info;

    static unsigned int index = 0;
    index++;

    return m_work_lis[index % ((unsigned int)m_work_num)].ProcResult();
}

bool HttpSystem::AsyncURLRequest(IHttpTask* task)
{
    FUNC_PERFORMANCE_CHECK();
    static int index = 0;
    index++;

    return m_work_lis[index % m_work_num].AsyncURLRequest(task);
}
