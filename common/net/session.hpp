#pragma once
#include "timer_system.hpp"
#include "net_tcp.h"
#include "meyers_singleton.hpp"
#include "protocol_system.hpp"

class IListener;
class ISession;

class DataCheckTimer
    :public ITimer
{
public:
    DataCheckTimer(ISession* session);

    ~DataCheckTimer();

public:
    void OnTimer(void) override;
private:
    ISession* m_session;
};

class ISession
{
public:
    ISession(HSESSION session = nullptr, IListener* listener = nullptr);
    virtual ~ISession();

    virtual void OnEstablish(void) = 0;

    virtual void OnTerminate(void) = 0;

    virtual void OnError(net_tcp_error moduleerror, int32_t systemerror) = 0;

    virtual void OnRecvRawData(const char *data, uint32_t len) = 0;

    //////////////////////////////////////////////////////////////////////////
    static uint32_t ParsePacket(HSESSION session,
        const char* pData, const uint32_t len);

    static void Establish(HLISTENER handle, HSESSION session);

    static void Terminate(HSESSION session);

    static void Error(HSESSION session, net_tcp_error moduleerror,
        int32_t systemerror);

    static void Recv(HSESSION session, const char* data,
        uint32_t len);
    //////////////////////////////////////////////////////////////////////////
public:
    void RecvRawData(const char* data, uint32_t len);

    bool SendRawData(const void* data, uint32_t len);

    void CacheSendRawData(const void* data, uint32_t len);

    bool SendRawDataNoDelay(const void* data, uint32_t len);

    bool Connect(const std::string& ip, uint16_t port, uint32_t recvbuf, uint32_t sendbuf);

    void Disconnect(void);

    void ActiveDataCheck(void);

    void OnDataCheck(void);

    uint32_t OnParsePacket(const char* data, const uint32_t len);

    inline size_t SendCacheSize(void) { return m_send_cache.size(); }
    inline size_t RecvCacheSize(void) { return m_recv_cache.size(); }

    inline HSESSION GetSession(void) { return m_session; }
    inline IListener* GetListener(void) { return m_listener; }

    bool GetPeerIPPort(ip_info& info);
    bool GetLocalIPPort(ip_info& info);

    bool GetPeerSockAddr(addr_info& info);
    bool GetLocalSockAddr(addr_info& info);

    void SetSendControl(uint32_t pkg_size, uint32_t delay_tick);
    inline bool IsConnected(void) { return m_session != nullptr; }

    uint32_t LeftSendDataSize(void);

    template<typename PS, typename P>
    bool SendProtocol(TProtocol<P>& proto)
    {
        uint8_t* head = nullptr;

        if (MeyersSingleton<PS>::Instance().EnCodeProtocol(proto, &head, sizeof(uint32_t)))
        {
            *reinterpret_cast<uint32_t*>(head) = static_cast<uint32_t>(MeyersSingleton<PS>::Instance().EnCodeData().Length());

            return SendRawData(MeyersSingleton<PS>::Instance().EnCodeData().Data(), static_cast<uint32_t>(MeyersSingleton<PS>::Instance().EnCodeData().Length()));
        }

        return false;
    }

private:
    HSESSION        m_session;
    IListener*      m_listener;
    DataCheckTimer  m_data_check_timer;
    std::string     m_send_cache;
    std::string     m_recv_cache;
};

class IListener
{
public:
    IListener(HLISTENER listener = nullptr);

    virtual ~IListener();

    virtual ISession* OnOpen(HSESSION session) = 0;

    virtual void OnClose(ISession* s) = 0;

    inline HLISTENER GetListener(void) { return m_listener; }
    //inline void SetListener(HLISTENER listener) { m_listener = listener; }
public:
    bool Listen(const std::string& ip, uint16_t port, uint32_t recvbuf, uint32_t sendbuf);
    void Close(void);
private:
    HLISTENER m_listener;
};

