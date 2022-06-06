#include "global_protocol.hpp"
#include "application/data_server_app.hpp"
#include "database/database_table_system.hpp"
#include "protocol/ErrorSystem.hpp"
#include "net/world_server.hpp"

void GlobalProtocolImpl::OnRecv_ServerStopNtf(ServerStopNtf& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();
    (void)rstProtocol;
    sDataServerApp.Stop(SvrAppStopType::NORMAL);
}

void GlobalProtocolImpl::OnRecv_LoadDBVarDataReq(LoadDBVarDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;
    sWMDatabaseTableSystem.GetTable<mt_dbvar>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_dbvar::Row*>& data)
        {
            LoadDBVarDataAck ack;
            ack.index = gs_idx;

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

            ack.datas.resize(static_cast<uint32_t>(data.size()));

            for (uint32_t i = 0; i < ack.datas.size(); i++)
            {
                DBVarData& db_var_data = ack.datas[i];
                mt_dbvar::Row& row = *data[i];
                
                db_var_data.key.append_data(row.Field<mt_dbvar::key>().GetData());
                db_var_data.value.append_data(row.Field<mt_dbvar::value>().GetData());
                db_var_data.value_type = static_cast<uint8_t>(row.Field<mt_dbvar::value_type>());
                db_var_data.merge_type = static_cast<uint8_t>(row.Field<mt_dbvar::merge_type>());
            }

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );
}
enum AuctionDataType
{
    AuctionTypeNull = 0,
    AuctionTypeItem,
    AuctionTypePet,
    AuctionTypeMax
};

#define OLD_GUID_TYPE_PART(guid) (uint8_t)(uint64_t(guid) >> 56)
#define OLD_GUID_SID_PART(guid) (uint8_t)((uint64_t(guid) >> 48) & (0x00000000000000FF))
#define OLD_GUID_TIME_PART(guid) (uint32_t)((uint64_t(guid) >> 16) & (0x00000000FFFFFFFF))
#define OLD_GUID_INC_PART(guid) (uint16_t)((uint64_t(guid)) & (0x000000000000FFFF))

void GlobalProtocolImpl::OnRecv_LoadAuctionObjectDataReq(LoadAuctionObjectDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;
    sWMDatabaseTableSystem.GetTable<mt_auction_object>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_auction_object::Row*>& data)
        {
            LoadAuctionObjectDataAck ack;
            uint32_t cur = 0;

            ack.index = gs_idx;

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

            ack.datas.resize(static_cast<uint32_t>(data.size()));

            
            for (uint32_t i = 0; i < ack.datas.size(); i++)
            {
                AuctionObjectData& auction_object_data = ack.datas[i];
                mt_auction_object::Row& row = *data[i];

                auction_object_data.guid = row.Field<mt_auction_object::guid>();
                if (OLD_GUID_SID_PART(auction_object_data.guid) != gs_idx)
                {
                    continue;
                }
                ++cur;

                auction_object_data.id              = static_cast<uint16_t>(row.Field<mt_auction_object::id>());
                auction_object_data.type            = row.Field<mt_auction_object::type>();
                auction_object_data.custom1         = row.Field<mt_auction_object::custom1>();
                auction_object_data.custom2         = row.Field<mt_auction_object::custom2>();
                auction_object_data.custom3         = row.Field<mt_auction_object::custom3>();
                auction_object_data.level           = row.Field<mt_auction_object::level>();
                auction_object_data.fight_value     = row.Field<mt_auction_object::fight_value>();
                auction_object_data.coin_type       = row.Field<mt_auction_object::coin_type>();
                auction_object_data.coin_value      = row.Field<mt_auction_object::coin_value>();
                auction_object_data.amount          = row.Field<mt_auction_object::amount>();
                auction_object_data.state           = row.Field<mt_auction_object::state>();
                auction_object_data.puton_times     = static_cast<uint32_t>(UTC_TO_LOCAL(row.Field<mt_auction_object::puton_times>()));
                auction_object_data.pulloff_times   = static_cast<uint32_t>(UTC_TO_LOCAL(row.Field<mt_auction_object::pulloff_times>()));
                auction_object_data.seller_guid     = row.Field<mt_auction_object::seller_guid>();

                StrSafeCopy(auction_object_data.seller_name, row.Field<mt_auction_object::seller_name>().GetData());

                bool to_json = false;

                switch (auction_object_data.type)
                {
                case AuctionTypeItem:
                    to_json = auction_object_data.item_data.UnmarshalJson(row.Field<mt_auction_object::data>().GetData());
                	break;
                case AuctionTypePet:
                    to_json = auction_object_data.pet_data.UnmarshalJson(row.Field<mt_auction_object::data>().GetData());
                    break;
                default:
                    break;
                }

                if (!to_json)
                {
                    LogERR(u8"[guid:{}]: load auction object fail : {}", 
                        auction_object_data.guid, row.Field<mt_auction_object::data>().GetData()
                        );
                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }
            }

            ack.datas.resize(cur);

            ack.remain_num = 0;
            ack.total_num = ack.datas.size();

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );
}

void GlobalProtocolImpl::OnRecv_LoadAuctionCookieDataReq(LoadAuctionCookieDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;
    sWMDatabaseTableSystem.GetTable<mt_auction_cookie>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_auction_cookie::Row*>& data)
        {
            LoadAuctionCookieDataAck ack;
            uint32_t cur = 0;

            ack.index = gs_idx;

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

            ack.datas.resize(static_cast<uint32_t>(data.size()));

            
            for (uint32_t i = 0; i < ack.datas.size(); i++)
            {
                AuctionCookieData& auction_cookie_data = ack.datas[i];
                mt_auction_cookie::Row& row = *data[i];

                auction_cookie_data.guid = row.Field<mt_auction_cookie::guid>();
                if (OLD_GUID_SID_PART(auction_cookie_data.guid) != gs_idx)
                {
                    continue;
                }
                ++cur;

                if (!auction_cookie_data.coins.unmarshal_json(row.Field<mt_auction_cookie::coins>().GetData()))
                {
                    LogERR(u8"load auction cookie fail coins: {}", row.Field<mt_auction_cookie::coins>().GetData());

                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }

                if (!auction_cookie_data.favorites.unmarshal_json(row.Field<mt_auction_cookie::favorites>().GetData()))
                {
                    LogERR(u8"load auction cookie fail favorites: {}", row.Field<mt_auction_cookie::favorites>().GetData());

                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }

                if (!auction_cookie_data.records.unmarshal_json(row.Field<mt_auction_cookie::records>().GetData()))
                {
                    LogERR(u8"load auction cookie fail records: {}", row.Field<mt_auction_cookie::records>().GetData());

                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }
            }

            ack.datas.resize(cur);
            ack.remain_num = 0;
            ack.total_num = ack.datas.size();

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );
}

void GlobalProtocolImpl::OnRecv_LoadGuildDataReq(LoadGuildDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;
    sWMDatabaseTableSystem.GetTable<mt_guild>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_guild::Row*>& datas)
        {
            LoadGuildDataAck ack;
            ack.index = gs_idx;

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

            ack.datas.resize(static_cast<uint16_t>(datas.size()));

            for (uint16_t i = 0; i < ack.datas.size(); i++)
            {
                DBGuildData& data = ack.datas[i];
                mt_guild::Row& row = *datas[i];


                data.guild.guid = row.Field<mt_guild::guid>();
                data.guild.guild_id = row.Field<mt_guild::id>();
                StrSafeCopy(data.guild.name, row.Field<mt_guild::name>().GetData());
                StrSafeCopy(data.guild.declaration, row.Field<mt_guild::declaration>().GetData());
                data.guild.leader_guid = row.Field<mt_guild::leader_guid>();
                StrSafeCopy(data.guild.leader_name, row.Field<mt_guild::leader_name>().GetData());
                data.guild.level = row.Field<mt_guild::level>();
                data.guild.base_level = row.Field<mt_guild::base_level>();
                data.guild.wing_room_level = row.Field<mt_guild::wing_room_level>();
                data.guild.vault_level = row.Field<mt_guild::vault_level>();
                data.guild.academy_level = row.Field<mt_guild::academy_level>();
                data.guild.pharmacy_level = row.Field<mt_guild::pharmacy_level>();
                data.guild.upgrade_cooldown = row.Field<mt_guild::upgrade_cooldown>();
                data.guild.fund = row.Field<mt_guild::fund>();
                data.guild.liveness = row.Field<mt_guild::liveness>();
                data.guild.build_degree = row.Field<mt_guild::build_degree>();
                data.guild.prestige = row.Field<mt_guild::prestige>();
                data.guild.fight_score = row.Field<mt_guild::fight_score>();
                data.guild.config = row.Field<mt_guild::config>();
                data.guild.create_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild::create_time>());
                data.guild.dismissed_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild::dismissed_time>());
                data.guild.impeach_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild::impeach_time>());
                data.guild.impeach_guid = row.Field<mt_guild::impeach_guid>();
                data.notice.notice_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild::notice_time>());
                StrSafeCopy(data.notice.notice, row.Field<mt_guild::notice>().GetData());

                if (!data.guild.events.unmarshal_json(row.Field<mt_guild::events>().GetData()))
                {
                    LogERR(u8"Guild {} Unmarshal events Fail json: {}", data.guild.name, row.Field<mt_guild::events>().GetData());
                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }

                if (!data.items.unmarshal_json(row.Field<mt_guild::items>().GetData()))
                {
                    LogERR(u8"Guild {} Unmarshal items Fail json: {}", data.guild.name, row.Field<mt_guild::items>().GetData());
                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }

                if (!data.guild.customs.UnmarshalJson(row.Field<mt_guild::customs>().GetData()))
                {
                    LogERR(u8"Guild {} Unmarshal customs Fail json: {}", data.guild.name, row.Field<mt_guild::customs>().GetData());
                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }
            }

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );

    sWMDatabaseTableSystem.GetTable<mt_guild_member>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_guild_member::Row*>& datas)
        {
            LoadGuildMemberDataAck ack;
            ack.index = gs_idx;

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

            ack.datas.resize(static_cast<uint32_t>(datas.size()));

            for (uint32_t i = 0; i < ack.datas.size(); i++)
            {
                DBGuildMemberData& data = ack.datas[i];
                mt_guild_member::Row& row = *datas[i];

                data.member.player_guid = row.Field<mt_guild_member::player_guid>();
                data.guild_guid = row.Field<mt_guild_member::guild_guid>();
                StrSafeCopy(data.member.player_name, row.Field<mt_guild_member::player_name>().GetData());
                data.member.level = row.Field<mt_guild_member::level>();
                data.member.role = (uint8_t)row.Field<mt_guild_member::role>();
                data.member.guild_job = (uint8_t)row.Field<mt_guild_member::guild_job>();
                data.member.total_contrb = row.Field<mt_guild_member::total_contrb>();
                data.member.surplus_contrb = row.Field<mt_guild_member::surplus_contrb>();
                data.member.last_week_contrb = row.Field<mt_guild_member::last_week_contrb>();
                data.member.this_week_contrb = row.Field<mt_guild_member::this_week_contrb>();
                data.member.permission = row.Field<mt_guild_member::permission>();
                data.member.join_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild_member::join_time>());
                data.member.donate_bind_gold = row.Field<mt_guild_member::donate_bind_gold>();
                data.member.last_donate_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild_member::last_donate_time>());
                data.member.last_dividend_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild_member::last_dividend_time>());
                data.member.last_login_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild_member::last_login_time>());
                data.member.last_logout_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild_member::last_logout_time>());
                data.member.forbid_end_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild_member::forbid_end_time>());
                data.member.status = row.Field<mt_guild_member::status>();
                data.member.vip = row.Field<mt_guild_member::vip>();
                data.member.reincarnation = row.Field<mt_guild_member::reincarnation>();
                data.member.job = row.Field<mt_guild_member::job>();
                data.total_fight_value = row.Field<mt_guild_member::total_fight_value>();

                if (!data.member.customs.UnmarshalJson(row.Field<mt_guild_member::customs>().GetData()))
                {
                    LogERR(u8"Guild Member {} Unmarshal customs Fail json: {}", data.member.player_name, row.Field<mt_guild_member::customs>().GetData());
                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }
            }

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );

    sWMDatabaseTableSystem.GetTable<mt_guild_applicant>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_guild_applicant::Row*>& datas)
        {
            LoadGuildApplicantDataAck ack;
            ack.index = gs_idx;

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

            ack.datas.resize(static_cast<uint32_t>(datas.size()));

            for (uint32_t i = 0; i < ack.datas.size(); i++)
            {
                DBGuildApplicantData& data = ack.datas[i];
                mt_guild_applicant::Row& row = *datas[i];

                data.applicant.guid = row.Field<mt_guild_applicant::player_guid>();
                data.guild_guid = row.Field<mt_guild_applicant::guild_guid>();
                StrSafeCopy(data.applicant.name, row.Field<mt_guild_applicant::player_name>().GetData());
                data.applicant.level = row.Field<mt_guild_applicant::level>();
                data.applicant.role = (uint8_t)row.Field<mt_guild_applicant::role>();
                data.applicant.sn = row.Field<mt_guild_applicant::sn>();
                data.applicant.guild_contrb = row.Field<mt_guild_applicant::guild_contrb>();
                data.applicant.last_apply_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_guild_applicant::last_apply_time>());
                data.applicant.vip = row.Field<mt_guild_applicant::vip>();
                data.applicant.reincarnation = row.Field<mt_guild_applicant::reincarnation>();
                data.total_fight_value = row.Field<mt_guild_applicant::total_fight_value>();
            }

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );
}

void GlobalProtocolImpl::OnRecv_LoadContactInfoReq(LoadContactInfoReq& rstProtocol)
{
    uint32_t gs_idx = rstProtocol.index;

    std::function<void(bool, const std::vector<
        SFieldList<
        mt_role::guid,
        mt_role::name,
        mt_role::roleid,
        mt_role::job1,
        mt_role::level,
        mt_role::sn,
        mt_role::vip,
        mt_role::reincarnation,
        mt_role::lastlogin,
        mt_role::lastlogout>>&)> on_select = [gs_idx](bool ret, const std::vector<
            SFieldList<
            mt_role::guid,
            mt_role::name,
            mt_role::roleid,
            mt_role::job1,
            mt_role::level,
            mt_role::sn,
            mt_role::vip,
            mt_role::reincarnation,
            mt_role::lastlogin,
            mt_role::lastlogout>>& datas) 
    {
        LoadContactInfoDataAck ack;
        ack.index = gs_idx;

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

        ack.datas.resize(static_cast<uint32_t>(datas.size()));

        for (uint32_t i = 0; i < ack.datas.size(); i++)
        {
            ContactInfoData& data = ack.datas[i];
            auto& row = datas[i];

            data.guid = row.Field<mt_role::guid>();
            StrSafeCopy(data.name, row.Field<mt_role::name>().GetData());
            data.role = (uint8_t)row.Field<mt_role::roleid>();
            data.job = row.Field<mt_role::job1>();
            data.level = row.Field<mt_role::level>();
            data.sn = row.Field<mt_role::sn>();
            data.vip = row.Field<mt_role::vip>();
            data.reincarnation = row.Field<mt_role::reincarnation>();
            data.last_login_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_role::lastlogin>());
            data.last_logout_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_role::lastlogout>());

            data.total_fight_value = 0;
        }

    END:
        sWorldServer.SendGSWrapperPkg(ack, gs_idx);
    };

    sWMDatabaseTableSystem.GetTable<mt_role>().AsyncSelect(
        SFieldList<>(), std::move(on_select));
}

void GlobalProtocolImpl::OnRecv_LoadContactDataReq(LoadContactDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;

    sWMDatabaseTableSystem.GetTable<mt_contact>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_contact::Row*>& datas)
        {
            LoadContactDataAck ack;
            ack.index = gs_idx;

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

            ack.datas.resize(static_cast<uint32_t>(datas.size()));

            for (uint32_t i = 0; i < ack.datas.size(); i++)
            {
                DBContactData& data = ack.datas[i];
                auto& row = *datas[i];

                data.player_guid = row.Field<mt_contact::player_guid>();
                data.contact_guid = row.Field<mt_contact::contact_guid>();
                data.contact_type = (uint8_t)row.Field<mt_contact::contact_type>();
                data.friendship = row.Field<mt_contact::friendship>();
                data.last_contact_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_contact::last_contact_time>().GetData());

                if (!data.customs.UnmarshalJson(row.Field<mt_contact::customs>().GetData()))
                {
                    LogERR(u8"Unmarshal Fail Json {}", row.Field<mt_contact::customs>().GetData());
                    data.customs.UnmarshalJson(u8"{\"s\":[],\"i\":[]}");
                }
            }

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );
}

void GlobalProtocolImpl::OnRecv_LoadMailDataReq(LoadMailDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;
    uint64_t recv_guid = rstProtocol.recv_guid;

    std::function<void(bool, const std::vector<mt_mail::Row>&)> on_select =
        [gs_idx, recv_guid](bool ret, const std::vector<mt_mail::Row>& datas)
    {
        LoadMailDataAck ack;
        ack.index = gs_idx;
        ack.recv_guid = recv_guid;

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

        ack.datas.resize(static_cast<uint16_t>(datas.size()));

        for (uint16_t i = 0; i < ack.datas.size(); i++)
        {
            DBMailData& data = ack.datas[i];
            auto& row = datas[i];

            data.head.mail_guid = row.Field<mt_mail::mail_guid>();
            data.head.mail_type = (uint8_t)row.Field<mt_mail::mail_type>();
            data.head.title.append_data(row.Field<mt_mail::title>().GetData());
            data.recv_guid = row.Field<mt_mail::recv_guid>();
            data.head.send_guid = row.Field<mt_mail::send_guid>();
            StrSafeCopy(data.head.send_name, row.Field<mt_mail::send_name>().GetData());
            data.head.send_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_mail::send_time>());
            data.head.status = row.Field<mt_mail::status>();
            data.body.content.append_data(row.Field<mt_mail::content>().GetData());

            if (!data.body.attrs.unmarshal_json(row.Field<mt_mail::attrs>().GetData()))
            {
                ack.errcode = ErrorCode::Json;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                goto END;
            }


            if (!data.body.items.unmarshal_json(row.Field<mt_mail::items>().GetData()))
            {
                ack.errcode = ErrorCode::Json;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                goto END;
            }

            if (!data.body.pets.unmarshal_json(row.Field<mt_mail::pets>().GetData()))
            {
                ack.errcode = ErrorCode::Json;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                goto END;
            }

            if (!data.guids.unmarshal_json(row.Field<mt_mail::guids>().GetData()))
            {
                ack.errcode = ErrorCode::Json;
                StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                goto END;
            }
        }

    END:
        sWorldServer.SendGSWrapperPkg(ack, gs_idx);
    };

    sWMDatabaseTableSystem.GetTable<mt_mail>().AsyncSelect(
        SFieldList<mt_mail::recv_guid>(rstProtocol.recv_guid),
        std::move(on_select)
    );
}

void GlobalProtocolImpl::OnRecv_LoadRanklistDataReq(LoadRanklistDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;

    sWMDatabaseTableSystem.GetTable<mt_ranklist>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_ranklist::Row*>& datas)
        {
            LoadRanklistDataAck ack;
            ack.index = gs_idx;

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

            ack.data.resize(static_cast<uint32_t>(datas.size()));

            for (uint32_t i = 0; i < ack.data.size(); i++)
            {
                DBRanklistData& data = ack.data[i];
                auto& row = *datas[i];

                data.rank_type = row.Field<mt_ranklist::type>();
                data.rank_guid = row.Field<mt_ranklist::rank_guid>();
                data.object_guid = row.Field<mt_ranklist::object_guid>();
                data.rank_data1 = row.Field<mt_ranklist::rank_data1>();
                data.rank_data2 = row.Field<mt_ranklist::rank_data2>();
                data.rank_data3 = row.Field<mt_ranklist::rank_data3>();

                StrSafeCopy(data.name, row.Field<mt_ranklist::name>().GetData());
                StrSafeCopy(data.rank_info, row.Field<mt_ranklist::info>().GetData());
            }

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );
}

void GlobalProtocolImpl::OnRecv_LoadPasturePetDataReq(LoadPasturePetDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;

    sWMDatabaseTableSystem.GetTable<mt_pasture>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_pasture::Row*>& datas)
        {
            LoadPasturePetDataAck ack;
            ack.index = gs_idx;

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

            ack.data.resize(static_cast<uint32_t>(datas.size()));

            for (uint32_t i = 0; i < ack.data.size(); i++)
            {
                PasturePetData& data = ack.data[i];
                auto& row = *datas[i];

                data.owner_guid = row.Field<mt_pasture::owner_guid>();
                data.map = (uint16_t)row.Field<mt_pasture::map>();
                data.x = (uint16_t)row.Field<mt_pasture::x>();
                data.y = (uint16_t)row.Field<mt_pasture::y>();
                data.placed_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_pasture::placed_time>());
                data.placed_status = row.Field<mt_pasture::placed_status>();

                data.replaced_time = (uint32_t)UTC_TO_LOCAL(row.Field<mt_pasture::replaced_time>());
                data.replaced_player_guid = row.Field<mt_pasture::replaced_player_guid>();
                StrSafeCopy(data.replaced_player_name, row.Field<mt_pasture::replaced_player_name>().GetData());
                data.replaced_pet_guid = row.Field<mt_pasture::replaced_pet_guid>();
                StrSafeCopy(data.replaced_pet_name, row.Field<mt_pasture::replaced_pet_name>().GetData());

                if (!data.pet.UnmarshalJson(row.Field<mt_pasture::pet_data>().GetData()))
                {
                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }
            }

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );
}

void GlobalProtocolImpl::OnRecv_LoadLadderDataReq(LoadLadderDataReq& rstProtocol)
{
    FUNC_PERFORMANCE_CHECK();

    uint32_t gs_idx = rstProtocol.index;

    sWMDatabaseTableSystem.GetTable<mt_ladder>().Cache().LoadAll(
        [gs_idx](bool ret, const std::vector<mt_ladder::Row*>& datas)
        {
            LoadLadderDataAck ack;
            ack.index = gs_idx;

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

            ack.data.resize(static_cast<uint32_t>(datas.size()));

            for (uint32_t i = 0; i < ack.data.size(); i++)
            {
                LadderData& data = ack.data[i];
                auto& row = *datas[i];

                data.player_guid = row.Field<mt_ladder::player_guid>();
                data.rank = row.Field<mt_ladder::rank>();

                if (!data.player.UnmarshalJson(row.Field<mt_ladder::player_data>().GetData()))
                {
                    ack.errcode = ErrorCode::Json;
                    StrSafeCopy(ack.errmsg, ErrorDesc<ErrorCode::Json>());
                    goto END;
                }
            }

        END:
            sWorldServer.SendGSWrapperPkg(ack, gs_idx);
        }
    );
}
