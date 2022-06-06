#pragma once
#include <stdint.h>
#include "lib_svr_def.h"
#include "front_session.hpp"
#include "back_session.hpp"

enum class hand_error: int32_t
{
    ok = 0,
    version_not_match,     //版本不匹配
    zone_unconnect,        //该区目前无法连接
    zone_not_exsit,        //该区目前不存在
    gen_key_fail,          //生成握手密钥失败
    check_key_fail,        //握手密钥验证失败
};

enum class SessionState:uint32_t
{
    NONE,
    ESTABLISH,
    CONNECT,
    TERMINATE,
    WAIT_CONNECT,
    WAIT_CHECK,
    WORK,
    MAX
};


class CAgent
{
public:
    CAgent(HSESSION session, IListener* listener);
    ~CAgent(void);

    void ActiveTerminateCheck(void);
    void ActiveSessionCheck(void);

    void CheckTerminate(void);
    void CheckSession(void);

    inline void SetFrontState(SessionState state) { m_front_state = state; }
    inline SessionState GetFrontState(void) { return m_front_state; }

    inline void SetBackState(SessionState state) { m_back_state = state; }
    inline SessionState GetBackState(void) { return m_back_state; }

    inline CFrontSession& GetFrontSession(void) { return m_front_session; }
    inline CBackSession& GetBackSession(void) { return m_back_session; }

    inline void SetZoneId(uint32_t zond_id) { m_zone_id = zond_id; }
    inline uint32_t GetZoneId(void) { return m_zone_id; }

    inline void SetFrontSendBytes(uint64_t bytes) { m_front_send_bytes = bytes; }
    inline uint64_t GetFrontSendBytes(void) { return m_front_send_bytes; }
    inline void SetFrontRecvBytes(uint64_t bytes) { m_front_recv_bytes = bytes; }
    inline uint64_t GetFrontRecvBytes(void) { return m_front_recv_bytes; }

    inline void SetBackSendBytes(uint64_t bytes) { m_back_send_bytes = bytes; }
    inline uint64_t GetBackSendBytes(void) { return m_back_send_bytes; }
    inline void SetBackRecvBytes(uint64_t bytes) { m_back_recv_bytes = bytes; }
    inline uint64_t GetBackRecvBytes(void) { return m_back_recv_bytes; }

protected:
private:
    CFrontSession m_front_session;
    CBackSession m_back_session;

    SessionState m_front_state;
    SessionState m_back_state;

    ITimer* m_call_check_terminate;
    ITimer* m_call_check_session;

    uint32_t m_zone_id;

    uint64_t m_front_send_bytes;
    uint64_t m_front_recv_bytes;
    uint64_t m_back_send_bytes;
    uint64_t m_back_recv_bytes;

    uint64_t m_last_front_send_bytes;
    uint64_t m_last_front_recv_bytes;
    uint64_t m_last_back_send_bytes;
    uint64_t m_last_back_recv_bytes;

    time_t m_last_front_recv_time;
    time_t m_last_back_recv_time;
};

