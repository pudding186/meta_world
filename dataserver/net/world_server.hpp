#pragma once
#include "meyers_singleton.hpp"
#include "session.hpp"
#include "protocol/database_protocol.hpp"

//class WorldServerListener: public IListener
//{
//public:
//    ISession* OnOpen(HSESSION session) override;
//
//    void OnClose(ISession* session) override;
//protected:
//private:
//};
//
//#define sWorldServerListener MeyersSingleton<WorldServerListener>::Instance()
enum class ServerSessionState:int32_t
{
    Init = 0,
    Connecting,
    Establish,
    WaitHandReq,
    WaitHandAck,
    Work,
};



class WorldServer: public ISession
{
public:
    WorldServer();
    ~WorldServer() = default;

    void OnEstablish(void) override;

    void OnTerminate(void) override;

    void OnError(net_tcp_error moduleerror, int32_t systemerror) override;

    void OnRecvRawData(const char* data, uint32_t len) override;

    bool ReConnect(void);

    inline bool InWork(void) { return m_state == ServerSessionState::Work; }

    template<typename T>
    inline bool SendPkg(TProtocol<T>& proto)
    {
        return ISession::SendProtocol<DatabaseProtocolSystem>(proto);
    }

    template<typename T>
    inline bool SendGSWrapperPkg(TProtocol<T>& proto, uint32_t index)
    {
        GSWrapperPkg pkg;
        pkg.index = index;

        uint8_t* head = nullptr;

        if (sDatabaseProtocolSystem.EnCodeProtocol(proto, &head, sizeof(uint32_t)))
        {
            *reinterpret_cast<uint32_t*>(head) = static_cast<uint32_t>(sDatabaseProtocolSystem.EnCodeData().Length());
            pkg.wrapper.append_data(
                reinterpret_cast<uint8_t*>(sDatabaseProtocolSystem.EnCodeData().Data()),
                static_cast<uint32_t>(sDatabaseProtocolSystem.EnCodeData().Length())
            );

            return SendPkg(pkg);
        }

        return false;
    }

    bool SendHandReq(void);
    bool VerifyHandRet(const char* data, uint32_t len);

protected:
private:
    ServerSessionState  m_state;
};

#define sWorldServer MeyersSingleton<WorldServer>::Instance()