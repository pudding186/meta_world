#include "utility.hpp"
#include "timer.h"
#include "guid_system.hpp"
#include "app_util.hpp"

/*
64 bit UUID info:
     type        time         inc
 |63......48|47........16|15........0
     16bit       32 bit      16 bit
*/

#define UNID_TYPE_PART(guid) (uint16_t)(uint64_t(guid) >> 48)
#define UNID_TIME_PART(guid) (uint32_t)((uint64_t(guid) >> 16) & (0x00000000FFFFFFFF))
#define UNID_INC_PART(guid) (uint16_t)((uint64_t(guid)) & (0x000000000000FFFF))

#define UNID_CUT_TYPE(type) ((type) & 0xffff)
#define UNID_CUT_TIME(time) ((time) & 0xffffffff)
#define UNID_CUT_INC(inc) ((inc) & 0xffff)

#define MAKE_UNID(type, time, inc) \
((uint64_t)UNID_CUT_TYPE(type) << 48) | \
((uint64_t)UNID_CUT_TIME(time) << 16) | (uint64_t)UNID_CUT_INC(inc)

bool Guid::operator<(const Guid& guid) const
{
    if (unid < guid.unid)
    {
        return true;
    }
    else if (unid == guid.unid)
    {
        return zid < guid.zid;
    }

    return false;
}

bool Guid::operator==(const Guid& guid) const
{
    if (unid == guid.unid)
    {
        return zid == guid.zid;
    }

    return false;
}

bool Guid::operator!=(const Guid& guid) const
{
    return !(*this == guid);
}

Guid& Guid::operator=(const Guid& guid)
{
    if (this != &guid)
    {
        unid = guid.unid;
        zid = guid.zid;
    }

    return *this;
}

bool Guid::Marshal(const Guid& guid, std::string& out)
{
    char sz_guid[32];

    char* ptr = AppUtil::UI642A64(guid.zid, sz_guid, sizeof(sz_guid));
    char* sep = --ptr;
    ptr = AppUtil::UI642A64(guid.unid, sz_guid, ptr - sz_guid + 1);
    *sep = '-';
    out.clear();
    out.append(ptr, sizeof(sz_guid) - (ptr - sz_guid) - 1);

    return true;
}

bool Guid::UnMarshal(Guid& guid, const std::string& in)
{
    const char* p = AppUtil::A642UI64(in.c_str(), in.length(), guid.unid);

    if (p && p > in.c_str() && (*p == '-'))
    {
        uint64_t zid;
        p = AppUtil::A642UI64(p + 1, in.c_str() + in.length() - p - 1, zid);
        if (!p || *p == '-')
        {
            guid.zid = static_cast<uint32_t>(zid);
            return true;
        }
    }

    return false;
}

bool GuidSystem::Initialize(uint32_t zone_id)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t time = static_cast<uint32_t>(get_time());

    for (uint16_t i = 0; i < static_cast<uint16_t>(GuidType::GUID_MAX); i++)
    {
        m_guid_table[i].unid = MAKE_UNID(i, time, 0);
        m_guid_table[i].zid = zone_id;
    }

    return true;
}

void GuidSystem::Uninitialized(void)
{
    FUNC_PERFORMANCE_CHECK();
}

Guid GuidSystem::CreateGuid(GuidType type)
{
    Guid id;
    id.unid = ++m_guid_table[static_cast<uint16_t>(type)].unid;
    id.zid = m_guid_table[static_cast<uint16_t>(type)].zid;

    return id;
}
