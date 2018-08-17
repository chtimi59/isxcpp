#pragma once
// project headers
#include "Job/JobsScheduler.h"
// system headers
#include "Windows.h"
//std
#include <string>

class Dialog1
{
    private:
        static std::string RESS_STR_CANCEL;

    public:
        Dialog1(HWND hWnd, bool matchPrepareToInstallPage, Job::t_Pointer pJob);
        std::string Show();

    private:
        bool mbMatchPrepareToInstallPage;
        Job::t_Pointer mpJob;

    private:
        static void onUpdate(Job::Arguments::t_Pointer pArg, LPVOID lpParam);
        static DWORD WINAPI operationsThread(LPVOID lpParam);

    private:
        struct {
            HWND parent;
            HWND dialog;
            HWND label1;
            HWND label2;
            HWND label3;
            HWND progress1;
            HWND progress2;
        } hWnds = {0};
        static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
        void MatchPrepareToInstallPage();
};
