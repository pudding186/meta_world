#include "database_world_protocol.hpp"
#include "net/world_server.hpp"
#include "database/database_table_system.hpp"
#include "protocol/ErrorSystem.hpp"
#include "log_system.hpp"

void DatabaseWorldProtocolImpl::OnRecv_LoadForbidReq(LoadForbidReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    LogSYS(u8"{}", __FUNCTION__);

    sWMDatabaseTableSystem.GetTable<mt_forbid>().Cache().LoadAll(
        [](bool ret, const std::vector<mt_forbid::Row*>& data)
        {
            LoadForbidAck ack;

            if (ret)
            {
                ack.errcode = ErrorCode::OK;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::OK>());
            }
            else
            {
                ack.errcode = ErrorCode::Database;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Database>());
                goto END;
            }

            ack.forbids.resize(static_cast<uint32_t>(data.size()));

            for (uint32_t i = 0; i < ack.forbids.size(); i++)
            {
                ForbidData& forbid_data = ack.forbids[i];
                mt_forbid::Row& row = *data[i];

                forbid_data.sid = row.Field<mt_forbid::sid>();
                forbid_data.keyword.append_data(row.Field<mt_forbid::keyword>().GetData());

                forbid_data.keytype = row.Field<mt_forbid::keytype>();
                forbid_data.fbdtype = row.Field<mt_forbid::fbdtype>();
                forbid_data.begin = UTC_TO_LOCAL(row.Field<mt_forbid::begin>());
                forbid_data.end = UTC_TO_LOCAL(row.Field<mt_forbid::end>());

                forbid_data.desc.append_data(row.Field<mt_forbid::desc>().GetData());
            }

        END:
            LogSYS(u8"Send LoadForbidAck {}", ack.forbids.size());
            sWorldServer.SendPkg(ack);
        }
    );
}

void DatabaseWorldProtocolImpl::OnRecv_LoadForbidTalkReq(LoadForbidTalkReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    LogSYS(u8"{}", __FUNCTION__);

    sWMDatabaseTableSystem.GetTable<mt_forbidtalk>().Cache().LoadAll(
        [](bool ret, const std::vector<mt_forbidtalk::Row*>& data)
        {
            LoadForbidTalkAck ack;

            if (ret)
            {
                ack.errcode = ErrorCode::OK;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Database>());
            }
            else
            {
                ack.errcode = ErrorCode::Database;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Database>());
                goto END;
            }

            ack.forbids.resize(static_cast<uint32_t>(data.size()));

            for (uint32_t i = 0; i < ack.forbids.size(); i++)
            {
                ForbidTalkData& forbid_talk_data = ack.forbids[i];

                mt_forbidtalk::Row& row = *data[i];

                forbid_talk_data.guid = row.Field<mt_forbidtalk::guid>();
                forbid_talk_data.begin = UTC_TO_LOCAL(row.Field<mt_forbidtalk::begin>());
                forbid_talk_data.end = UTC_TO_LOCAL(row.Field<mt_forbidtalk::end>());

                forbid_talk_data.desc.append_data(row.Field<mt_forbidtalk::desc>().GetData());
            }

        END:
            LogSYS(u8"Send LoadForbidTalkAck {}", ack.forbids.size());
            sWorldServer.SendPkg(ack);
        }
    );
}

void DatabaseWorldProtocolImpl::OnRecv_LoadUserReq(LoadUserReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    if (!sWMDatabaseTableSystem.GetTable<mt_user>().Cache().Load(
        mt_user::PriKey(rstProtocol.uid, rstProtocol.sid),
        [](bool ret, const mt_user::Row* data)
        {
            LoadUserAck ack;

            if (ret)
            {
                ack.errcode = ErrorCode::OK;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Database>());
            }
            else
            {
                ack.errcode = ErrorCode::Database;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Database>());
                goto END;
            }

            if (!data)
            {
                ack.errcode = ErrorCode::NoUserData;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::NoUserData>());
                goto END;
            }

            ack.userdata.uid        = data->Field<mt_user::uid>();
            ack.userdata.sid        = data->Field<mt_user::sid>();
            StrSafeCopy(ack.userdata.uname, data->Field<mt_user::uname>().GetData());
            ack.userdata.billinyb   = data->Field<mt_user::billinyb>();
            ack.userdata.billoutyb  = data->Field<mt_user::billoutyb>();
            ack.userdata.gameinyb   = data->Field<mt_user::gameinyb>();
            ack.userdata.gameoutyb  = data->Field<mt_user::gameoutyb>();
            ack.userdata.createtime = data->Field<mt_user::createtime>();
            ack.userdata.lastlogin  = data->Field<mt_user::lastlogin>();
        
        END:
            sWorldServer.SendPkg(ack);
        }
    ))
    {
        LoadUserAck ack;
        ack.errcode = ErrorCode::DatabaseBusy;
        StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::DatabaseBusy>());

        sWorldServer.SendPkg(ack);
    }
}

void DatabaseWorldProtocolImpl::OnRecv_UpdateUserNtf(UpdateUserNtf& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    RowCache<mt_user>* row_cache = 
        sWMDatabaseTableSystem.GetTable<mt_user>().Cache().GetCache(mt_user::PriKey(
            rstProtocol.userdata.uid, rstProtocol.userdata.sid));

    if (row_cache)
    {
        if (row_cache->GetState() == CacheRowState::DELETE_SYNC)
        {
            row_cache->Update(mt_user::Row(
                rstProtocol.userdata.uid,
                rstProtocol.userdata.sid,
                rstProtocol.userdata.uname,
                rstProtocol.userdata.billinyb,
                rstProtocol.userdata.billoutyb,
                rstProtocol.userdata.gameinyb,
                rstProtocol.userdata.gameoutyb,
                rstProtocol.userdata.createtime,
                rstProtocol.userdata.lastlogin
            ));
        }
        else
        {
            row_cache->Update(SFieldList<
                mt_user::uname,
                mt_user::billoutyb,
                mt_user::gameinyb,
                mt_user::gameoutyb,
                mt_user::createtime,
                mt_user::lastlogin
            >(
                rstProtocol.userdata.uname,
                rstProtocol.userdata.billoutyb,
                rstProtocol.userdata.gameinyb,
                rstProtocol.userdata.gameoutyb,
                rstProtocol.userdata.createtime,
                rstProtocol.userdata.lastlogin
                ));
        }
    }
}

void DatabaseWorldProtocolImpl::OnRecv_PlayerBriefsReq(PlayerBriefsReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t uid = rstProtocol.uid;
    uint8_t sid = rstProtocol.sid;

    std::function<void(bool, const std::vector < SFieldList <
        mt_role::uid,
        mt_role::sid,
        mt_role::sn,
        mt_role::guid,
        mt_role::unid,
        mt_role::name,
        mt_role::roleid,
        mt_role::race,
        mt_role::gender,
        mt_role::level,
        mt_role::job1,
        mt_role::job2,
        mt_role::job3,
        mt_role::last_common_map,
        mt_role::last_common_x,
        mt_role::last_common_y,
        mt_role::state,
        mt_role::createtime,
        mt_role::lastlogin,
        mt_role::destroytime,
        mt_role::weapon,
        mt_role::amulet,
        mt_role::mount,
        mt_role::color1,
        mt_role::color2,
        mt_role::reincarnation,
        mt_role::customs
        >> &) > on_select = [uid, sid](bool ret, const std::vector < SFieldList <
            mt_role::uid,
            mt_role::sid,
            mt_role::sn,
            mt_role::guid,
            mt_role::unid,
            mt_role::name,
            mt_role::roleid,
            mt_role::race,
            mt_role::gender,
            mt_role::level,
            mt_role::job1,
            mt_role::job2,
            mt_role::job3,
            mt_role::last_common_map,
            mt_role::last_common_x,
            mt_role::last_common_y,
            mt_role::state,
            mt_role::createtime,
            mt_role::lastlogin,
            mt_role::destroytime,
            mt_role::weapon,
            mt_role::amulet,
            mt_role::mount,
            mt_role::color1,
            mt_role::color2,
            mt_role::reincarnation,
            mt_role::customs
            >> &datas)
    {
        PlayerBriefsAck ack;

        ack.uid = uid;
        ack.sid = sid;

        if (ret)
        {
            ack.errcode = ErrorCode::OK;
            StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Database>());
        }
        else
        {
            ack.errcode = ErrorCode::Database;
            StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Database>());
            goto END;
        }

        ack.briefs.resize(static_cast<uint8_t>(datas.size()));

        for (uint8_t i = 0; i < ack.briefs.size(); i++)
        {
            PlayerBrief& brief = ack.briefs[i];
            auto& row = datas[i];
        }

    END:
        sWorldServer.SendPkg(ack);
    };

    sWMDatabaseTableSystem.GetTable<mt_role>().AsyncSelect(
        SFieldList<mt_role::uid, mt_role::sid>(rstProtocol.uid, rstProtocol.sid),
        std::move(on_select)
    );
}
