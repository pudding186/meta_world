#pragma once
#include "meyers_singleton.hpp"
#include "protocol_system.hpp"

#include "database_world_protocol.hpp"
#include "global_protocol.hpp"

class DatabaseProtocolSystem:
    public ProtocolSystem<
    DatabaseWorldProtocolImpl,
    GlobalProtocolImpl>
{
public:
    DatabaseProtocolSystem() = default;
    ~DatabaseProtocolSystem() = default;

    bool HandleProtocol(const void* data, uint32_t size);
protected:
private:
};

#define sDatabaseProtocolSystem MeyersSingleton<DatabaseProtocolSystem>::Instance()
