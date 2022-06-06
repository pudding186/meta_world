#pragma once
#include "protocol/WSDB.hpp"

class DatabaseWorldProtocolImpl:public CWSDB<DatabaseWorldProtocolImpl>
{
public:
    void OnRecv_LoadForbidReq(LoadForbidReq& rstProtocol);

    void OnRecv_LoadForbidTalkReq(LoadForbidTalkReq& rstProtocol);

    void OnRecv_LoadUserReq(LoadUserReq& rstProtocol);

    void OnRecv_UpdateUserNtf(UpdateUserNtf& rstProtocol);

    void OnRecv_PlayerBriefsReq(PlayerBriefsReq& rstProtocol);
protected:
private:
};