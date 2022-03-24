#pragma once
#include <stdint.h>
#include <string>
#include "meyers_singleton.hpp"

enum class GuidType :uint16_t {
    GUID_NULL = 0,
    GUID_SYSTEM,
    GUID_MAP,
    GUID_PLAYER,
    GUID_NPC,
    GUID_ITEM,
    GUID_MONSTER,
    GUID_MAX
};

struct Guid 
{
    uint64_t unid;
    uint32_t zid;

    bool operator < (const Guid& guid) const;
    bool operator == (const Guid& guid) const;
    bool operator != (const Guid& guid) const;

    static bool Marshal(const Guid& guid, std::string& out);
    static bool UnMarshal(Guid& guid, const std::string& in);


};

class GuidSystem
{
public:
    bool Initialize(void);
    void Uninitialized(void);

    Guid CreateGuid(GuidType type);
protected:
private:
    Guid m_guid_table[static_cast<uint16_t>(GuidType::GUID_MAX)];
};

#define sGuidSystem MeyersSingleton<GuidSystem>::Instance()