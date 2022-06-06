#include "application/agent_server_app.hpp"

int32_t main(int32_t argc, char** argv)
{
    SetConsoleOutputCP(65001);

    if (!sAgentServerApp.Start())
    {
        return -1;
    }

    char cmd[512] = { 0 };
    while (!sAgentServerApp.IsStopping())
    {
        if (_kbhit())
        {
            gets_s(cmd);

            if (!_stricmp(u8"@stop", cmd) ||
                !_stricmp(u8"2", cmd))
            {
                sAgentServerApp.Stop(SvrAppStopType::NORMAL);

                break;
            }
            else
            {

            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    _chmod(0);

    sAgentServerApp.WaitFor();

    return 0;
}