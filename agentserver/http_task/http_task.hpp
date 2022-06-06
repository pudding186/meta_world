#pragma once
#include "meyers_singleton.hpp"
#include "http_system.hpp"

class HttpTaskGetGateList : public IHttpTask
{
public:
    HttpTaskGetGateList(const std::string& url) : IHttpTask(url) {}

    size_t write_data(void* buffer, size_t size, size_t nmemb) override;
    void OnResult(CURLcode code) override;
    void OnRelease()override;
protected:
private:
    std::string m_result;
};