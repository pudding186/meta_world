#include "utility.hpp"
#include "timer.h"
#include "config_system.hpp"
#include "app_util.hpp"

bool ConfigSystem::Initialize(void)
{
    return false;
}

void ConfigSystem::Uninitialized(void)
{

}

uint32_t ConfigSystem::GetZoneId(void)
{
    return 1;
}

std::string ConfigSystem::GetRunLogDir(void)
{
    return "./";
}

uint16_t ConfigSystem::GetDBPort(void)
{
    return 3306;
}

std::string ConfigSystem::GetDBHost(void)
{
    return "127.0.0.1";
}

std::string ConfigSystem::GetDBUser(void)
{
    return "root";
}

std::string ConfigSystem::GetDBPassowrd(void)
{
    return "root";
}

std::string ConfigSystem::GetDBCharacterSet(void)
{
    return "utf8";
}

std::string ConfigSystem::GetDBDataBase(void)
{
    return "meta_db";
}
