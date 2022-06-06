#include "application/data_server_app.hpp"

//void DisableConsoleClose(void)
//{
//    HWND console_wnd = ::GetConsoleWindow();
//
//    if (console_wnd)
//    {
//        ::RemoveMenu(::GetSystemMenu(console_wnd, FALSE), SC_CLOSE, MF_BYCOMMAND);
//        ::DrawMenuBar(console_wnd);
//    }
//}

int32_t main(int32_t argc, char** argv)
{
    //DisableConsoleClose();

    SetConsoleOutputCP(65001);

    if (!sDataServerApp.Start())
    {
        return -1;
    }

    char cmd[512] = { 0 };
    while (!sDataServerApp.IsStopping())
    {
        if (_kbhit())
        {
            gets_s(cmd);

            if (!_stricmp(u8"@stop", cmd) ||
                !_stricmp(u8"2", cmd))
            {
                sDataServerApp.Stop(SvrAppStopType::NORMAL);

                break;
            }
            else
            {

            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    _chmod(0);

    sDataServerApp.WaitFor();

    return 0;
}