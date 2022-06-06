#include "world_server.hpp"
#include "log_system.hpp"
#include "config/data_server_config.hpp"
#include "application/data_server_app.hpp"
#include "protocol/database_protocol.hpp"
#include "application/data_server_app.hpp"

WorldServer::WorldServer()
    :ISession(nullptr, nullptr),
    m_state(ServerSessionState::Init)
{

}

void WorldServer::OnEstablish(void)
{
    FUNC_PERFORMANCE_CHECK();

    LogSYS(u8"World Server Establish!");
    m_state = ServerSessionState::Establish;

    if (SendHandReq())
    {
        m_state = ServerSessionState::WaitHandAck;
    }
    else
    {
        Disconnect();
        LogERR(u8"send hand req fail!");
    }
}

void WorldServer::OnTerminate(void)
{
    LogSYS(u8"World Server Terminate!");
    m_state = ServerSessionState::Init;

    if (!sDataServerApp.IsStopping())
    {
        ReConnect();
    }
}

void WorldServer::OnError(net_tcp_error moduleerror, int32_t systemerror)
{
    LogSYS(u8"World Server Error {}-{}!", moduleerror, systemerror);
    if (moduleerror == net_tcp_error::error_connect_fail)
    {
        m_state = ServerSessionState::Init;

        if (!sDataServerApp.IsStopping())
        {
            ReConnect();
        }
    }
}

void WorldServer::OnRecvRawData(const char* data, uint32_t len)
{
    data += sizeof(uint32_t);
    len -= sizeof(uint32_t);

    switch (m_state)
    {
    case ServerSessionState::WaitHandAck:
        VerifyHandRet(data, len);
    	break;
    case ServerSessionState::Work:
        sDatabaseProtocolSystem.HandleProtocol(data, len);
        break;
    default:
        return;
    }
}

bool WorldServer::ReConnect(void)
{
    if (IsConnected())
    {
        return true;
    }

    SessionConfigData* world_server_session_config_data = sDataServerConfig.GetWorldServerConfigData();

    if (!world_server_session_config_data)
    {
        LogERR(u8"Can not find world server session config data!");
        return false;
    }

    if (!Connect(
        world_server_session_config_data->ip,
        world_server_session_config_data->port,
        world_server_session_config_data->recv_cache,
        world_server_session_config_data->send_cache
    ))
    {
        LogERR(u8"Connect World Server Ip={} Port={} Fail!",
            world_server_session_config_data->ip,
            world_server_session_config_data->port);
        return false;
    }

    LogSYS(u8"Connecting World Server Ip={} Port={} ...!",
        world_server_session_config_data->ip,
        world_server_session_config_data->port);

    return true;
}

#pragma pack(push, 4)

using SSvrHandPkg = struct stSSvrHandPkg
{
    uint16_t wSvrType;
    uint16_t wSvrIndex;
    uint32_t dwZoneID;
    uint16_t wVersion;
    uint16_t wLength;
    char cbExt[32];
};

using SSvrHandPkgRet = struct stSSvrHandPkgRet
{
    uint32_t dwSvrID;
    uint32_t dwZoneID;
    uint16_t wVersion;
    uint16_t wLength;
    char cbExt[32];
};

#pragma pack(pop)

#define MakeNetVersion(main, major, minor) \
(uint16_t)(((main) << 12) | ((major) << 6) | (minor))

#define NetVersion() MakeNetVersion(1, 1, 1)

bool WorldServer::SendHandReq(void)
{
    FUNC_PERFORMANCE_CHECK();

    char buf[64] = { 0 };
    SSvrHandPkg* pkg = (SSvrHandPkg*)(buf + sizeof(int32_t));
    pkg->wSvrType = 4;
    pkg->wSvrIndex = 1;
    pkg->dwZoneID = 1;
    pkg->wVersion = NetVersion();

    pkg->wLength = 0;

    uint32_t l = sizeof(SSvrHandPkg) + sizeof(int32_t);

    *(int32_t*)buf = l;
    return SendRawData(buf, l);
}

bool WorldServer::VerifyHandRet(const char* data, uint32_t len)
{
    FUNC_PERFORMANCE_CHECK();

    if (len != sizeof(SSvrHandPkgRet))
    {
        LogERR(u8"'world server' verify handret fail, data size illegal.");
        Disconnect();
        return false;
    }

    const SSvrHandPkgRet* pkg = reinterpret_cast<const SSvrHandPkgRet*>(data);
    if (pkg->wVersion != NetVersion())
    {
        LogERR(u8"'world server' verify handret fail, net version [{}, {}] illegal.",
            NetVersion(),
            pkg->wVersion);
        Disconnect();
        return false;
    }

    if (pkg->dwZoneID != sDataServerApp.GetZoneId())
    {
        LogERR(u8"'world server' verify handret fail, zone id [{}, {}] illegal.",
            sDataServerApp.GetZoneId(),
            pkg->dwZoneID);
        Disconnect();
        return false;
    }

    if (1 != (uint8_t)pkg->dwSvrID)
    {
        LogERR(u8"'world server' verify handret fail, unknown index [{}].",
            pkg->dwSvrID);
        Disconnect();
        return false;
    }

    m_state = ServerSessionState::Work;

    LogSYS(u8"World Server into Work State!");

    return true;
}
