#include "database_table_system.hpp"
#include "database_system.hpp"

std::string MWDatabaseTableSystem::Initialize(void)
{
    FUNC_PERFORMANCE_CHECK();

    std::string ret = "";

    this->SetMysqlConnection<
        mt_zone,
        mt_user,
        mt_role
    >(sDatabaseSystem.GetMysqlAsyncConnection(DBConnectionFlag::ASYNC1));

    this->SetMysqlConnection<
        mt_ranklist,
        mt_pasture,
        mt_ladder,
        mt_mail
    >(sDatabaseSystem.GetMysqlAsyncConnection(DBConnectionFlag::ASYNC2));

    this->SetMysqlConnection<
        mt_guild,
        mt_guild_applicant,
        mt_guild_member,
        mt_forbid,
        mt_forbidtalk
    >(sDatabaseSystem.GetMysqlAsyncConnection(DBConnectionFlag::ASYNC3));

    this->SetMysqlConnection<
        mt_dbvar,
        mt_contact,
        mt_billlog,
        mt_billlog2,
        mt_auction_cookie,
        mt_auction_object
    >(sDatabaseSystem.GetMysqlAsyncConnection(DBConnectionFlag::ASYNC4));

    ret = this->CheckTables();

    if (ret != "")
    {
        return ret;
    }

    ret = this->CheckFields();

    if (ret != "")
    {
        return ret;
    }

    ret = this->CheckIndexs();

    if (ret != "")
    {
        return ret;
    }

    return ret;
}
