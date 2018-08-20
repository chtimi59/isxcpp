#pragma once
#include "Job/JobsScheduler.h"

class Dialog1
{
    private:
        static std::string RESS_STR_CANCEL;
        static std::string RESS_STR_TITLE;

    public:
        Dialog1(HWND hWnd, bool matchPrepareToInstallPage, Job::t_Pointer pJob);
        std::string show();

    private:
        bool mbMatchPrepareToInstallPage;
        Job::t_Pointer mpJob;

    private:
        static void UpdateProc(Job::Arguments::t_Pointer pArg, LPVOID lpParam);
        static DWORD WINAPI OperationsThread(LPVOID lpParam);

    private:
        void matchInnoLayout();

        struct {
            HWND parent;
            HWND dialog;
            HWND label1;
            HWND label2;
            HWND label3;
            HWND progress1;
            HWND progress2;
            HWND cancel;
        } hWnds = {0};
        
        static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
