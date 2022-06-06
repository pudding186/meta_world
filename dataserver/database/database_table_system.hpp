#pragma once
#include "meyers_singleton.hpp"
#include "common/dbtable/dbtable_system.hpp"

#include "dbtable/mt_auction_cookie.hpp"
#include "dbtable/mt_auction_object.hpp"
#include "dbtable/mt_billlog.hpp"
#include "dbtable/mt_billlog2.hpp"
#include "dbtable/mt_contact.hpp"
#include "dbtable/mt_dbvar.hpp"
#include "dbtable/mt_forbid.hpp"
#include "dbtable/mt_forbidtalk.hpp"
#include "dbtable/mt_guild.hpp"
#include "dbtable/mt_guild_applicant.hpp"
#include "dbtable/mt_guild_member.hpp"
#include "dbtable/mt_ladder.hpp"
#include "dbtable/mt_mail.hpp"
#include "dbtable/mt_pasture.hpp"
#include "dbtable/mt_ranklist.hpp"
#include "dbtable/mt_role.hpp"
#include "dbtable/mt_user.hpp"
#include "dbtable/mt_zone.hpp"

class MWDatabaseTableSystem:
    public DatabaseTableSystem<
    mt_auction_cookie,
    mt_auction_object,
    mt_billlog,
    mt_billlog2,
    mt_contact,
    mt_dbvar,
    mt_forbid,
    mt_forbidtalk,
    mt_guild,
    mt_guild_applicant,
    mt_guild_member,
    mt_ladder,
    mt_mail,
    mt_pasture,
    mt_ranklist,
    mt_role,
    mt_user,
    mt_zone
    >
{
public:
    MWDatabaseTableSystem() = default;
    ~MWDatabaseTableSystem() = default;

    std::string Initialize(void);

protected:
private:
};

#define sWMDatabaseTableSystem MeyersSingleton<MWDatabaseTableSystem>::Instance()
