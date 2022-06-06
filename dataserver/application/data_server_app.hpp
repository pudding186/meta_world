#pragma once
#include "svr_app.hpp"
#include "version.h"
#include "meyers_singleton.hpp"

class CDataServerApp:public SvrApp<CDataServerApp>
{
public:

    std::string OnInitialize(void);
    bool OnUpdate(CFuncPerformanceInfo** info);
    void OnUninitialized(void);

    AsyncRet OnAsyncInit(void);
    AsyncRet OnAsyncUninit(void);

    static inline std::string GetName() { return "DataServer"; }
    static inline std::string GetVersion() { return STRPRODUCTVER; }
    static inline uint32_t GetZoneId() { return 1; }

protected:
private:
};

#define sDataServerApp MeyersSingleton<CDataServerApp>::Instance()