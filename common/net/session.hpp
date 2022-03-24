#pragma once
#include "timer_system.hpp"
#include "net_tcp.h"

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

    virtual void OnRecv(const char *data, uint32_t len) = 0;

    //////////////////////////////////////////////////////////////////////////
public:
    void RecvRawData(const char* data, uint32_t len);

    bool SendRawData(const void* data, uint32_t len);

    bool Connect(const std::string& ip, uint16_t port, uint32_t recvbuf, uint32_t sendbuf);

    void Disconnect(void);

    void ActiveDataCheck(void);

    void OnDataCheck(void);

    uint32_t OnParsePacket(const char* data, const uint32_t len);

    inline size_t SendCacheSize(void) { return m_send_cache.size(); }
    inline size_t RecvCacheSize(void) { return m_recv_cache.size(); }

    inline void SetSession(HSESSION session) { m_session = session; }
    inline HSESSION GetSession(void) { return m_session; }

    inline void SetListener(IListener* listener) { m_listener = listener; }
    inline IListener* GetListener(void) { return m_listener; }

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

    virtual ISession* OnAccept(HSESSION session) = 0;

    virtual void OnClose(ISession* s) = 0;

    inline HLISTENER GetListener(void) { return m_listener; }
    inline void SetListener(HLISTENER listener) { m_listener = listener; }
public:
    bool Listen(const std::string& ip, uint16_t port, uint32_t sendbuf, uint32_t recvbuf);
    void Close(void);
private:
    HLISTENER m_listener;
};

