#pragma once
#include "session.hpp"

class TcpListener:public IListener
{
public:
    ISession* OnOpen(HSESSION session) override;

    void OnClose(ISession* s) override;
protected:
private:
};