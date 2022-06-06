#pragma once
#include "protocol/Global.hpp"

class GlobalProtocolImpl:public CGlobal<GlobalProtocolImpl>
{
public:
    void OnRecv_ServerStopNtf(ServerStopNtf& rstProtocol);

    void OnRecv_LoadDBVarDataReq(LoadDBVarDataReq& rstProtocol);

    void OnRecv_LoadAuctionObjectDataReq(LoadAuctionObjectDataReq& rstProtocol);

    void OnRecv_LoadAuctionCookieDataReq(LoadAuctionCookieDataReq& rstProtocol);

    void OnRecv_LoadGuildDataReq(LoadGuildDataReq& rstProtocol);

    void OnRecv_LoadContactInfoReq(LoadContactInfoReq& rstProtocol);

    void OnRecv_LoadContactDataReq(LoadContactDataReq& rstProtocol);

    void OnRecv_LoadMailDataReq(LoadMailDataReq& rstProtocol);

    void OnRecv_LoadRanklistDataReq(LoadRanklistDataReq& rstProtocol);

    void OnRecv_LoadPasturePetDataReq(LoadPasturePetDataReq& rstProtocol);

    void OnRecv_LoadLadderDataReq(LoadLadderDataReq& rstProtocol);
protected:
private:
};