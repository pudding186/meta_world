#include "http_task.hpp"
#include "log_system.hpp"
#include "config/agent_server_config.hpp"

size_t HttpTaskGetGateList::write_data(void* buffer, size_t size, size_t nmemb)
{
    m_result.append(reinterpret_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}

void HttpTaskGetGateList::OnResult(CURLcode code)
{
    if (code == CURLE_OK)
    {
        sAgentServerConfig.UpdateGetListXML(m_result);
    }
    else
    {
        LogERR(u8"获取网关列表失败..., 错误原因 {}:{}", code, curl_easy_strerror(code));
    }
}

void HttpTaskGetGateList::OnRelease()
{
    S_DELETE(this);
}
