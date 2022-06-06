#include "database_protocol.hpp"

bool DatabaseProtocolSystem::HandleProtocol(const void* data, uint32_t size)
{
    switch (*reinterpret_cast<const uint16_t*>(data))
    {
    case GlobalProtocolImpl::module_id:
        return DeCodeProtocol<GlobalProtocolImpl>(data, size);
        break;
    case DatabaseWorldProtocolImpl::module_id:
        return DeCodeProtocol<DatabaseWorldProtocolImpl>(data, size);
        break;
    default:
        return false;
    }
}
