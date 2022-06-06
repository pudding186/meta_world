#pragma once
#include "session.hpp"
#include "protocol/database_protocol.hpp"

class RechargeServer : public ISession
{
public:
    RechargeServer() = default;
    ~RechargeServer() = default;

    void OnEstablish(void) override;

    void OnTerminate(void) override;

    void OnError(net_tcp_error moduleerror, int32_t systemerror) override;

    void OnRecvRawData(const char* data, uint32_t len) override;

    template<typename T>
    inline bool SendPkg(TProtocol<T>& proto)
    {
        return ISession::SendProtocol<DatabaseProtocolSystem>(proto);
    }
protected:
private:
};