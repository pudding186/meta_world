#pragma once
#include <stdint.h>
#include <string>
#include "meyers_singleton.hpp"

class ConfigSystem
{
public:
    bool Initialize(void);
    void Uninitialized(void);

    uint32_t GetZoneId(void);
    std::string GetRunLogDir(void);

    uint16_t GetDBPort(void);
    std::string GetDBHost(void);
    std::string GetDBUser(void);
    std::string GetDBPassowrd(void);
    std::string GetDBCharacterSet(void);
    std::string GetDBDataBase(void);
};

#define sConfigSystem MeyersSingleton<ConfigSystem>::Instance()