#include "Dialog1.h"
// project headers
#include "Resources/res.h"
#include "resources.h"
#include "Events.h"
#include "Job/JobsScheduler.h"
// std
#include <memory>
#include <string>
// system headers
#include <windows.h>
#include <commctrl.h>

extern HINSTANCE hInst;

std::string Dialog1::Dialog1::RESS_STR_CANCEL;

Dialog1::Dialog1(HWND hWnd, bool matchPrepareToInstallPage, Job::t_Pointer pJob)
{
    RESS_STR_CANCEL = ressources::getString(IDS_CANCEL);

    hWnds.parent = hWnd;
    mbMatchPrepareToInstallPage = matchPrepareToInstallPage;
    mpJob = pJob;
}

std::string Dialog1::Show()
{
    std::string out;

    HWND hwnd = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnds.parent, DlgProc, (LPARAM)this);
    if (hwnd == NULL) throw std::invalid_argument("couldn't create dialog");
    hWnds.dialog = hwnd;

    ShowWindow(hWnds.dialog, SW_SHOW);
    HANDLE hThread = CreateThread(NULL, 0, operationsThread, this, 0, NULL);

    HANDLE evtHandles[] = {
        Events::UIEvent(),
        Events::UserCancelEvent(),
        hThread
    };
    const DWORD count = sizeof(evtHandles) / sizeof(HANDLE);

    Events::UIParams uiDisplayed;
    while (TRUE)
    {
        
        DWORD dwStatus = MsgWaitForMultipleObjects(count, evtHandles, FALSE, INFINITE, QS_ALLINPUT);
        switch (dwStatus)
        {
            /* Update UI */
            case WAIT_OBJECT_0:
            {
                auto ui = Events::GetCurrentUI();
                if (uiDisplayed.label1 != ui.label1)
                    SendMessage(hWnds.label1, WM_SETTEXT, NULL, ui.label1.getLParam());
                if (uiDisplayed.label2 != ui.label2)
                    SendMessage(hWnds.label2, WM_SETTEXT, NULL, ui.label2.getLParam());
                if (uiDisplayed.label3 != ui.label3)
                    SendMessage(hWnds.label3, WM_SETTEXT, NULL, ui.label3.getLParam());
                if (uiDisplayed.progress1 != ui.progress1)
                    SendMessage(hWnds.progress1, PBM_SETPOS, ui.progress1, 0);
                if (uiDisplayed.progress2 != ui.progress2)
                    SendMessage(hWnds.progress2, PBM_SETPOS, ui.progress2, 0);
                out = ui.result;
                uiDisplayed = ui;
                break;
            }

            /* User Cancel */
            case WAIT_OBJECT_0 + 1:
            {
                /* kill the whole thread */
                TerminateThread(hThread, 1);
                if (out.empty()) out = RESS_STR_CANCEL;
                break;
            }

            /* Thread terminated */
            case WAIT_OBJECT_0 + 2:
            {
                Sleep(1500);
                DestroyWindow(hwnd);
                break;
            }

            /* Last, DialogMessages... */
            case WAIT_OBJECT_0 + count:
            {
                MSG msg;
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    if (msg.message == WM_QUIT) {
                        TerminateThread(hThread, 1);
                        return out;
                    }
                    if (!IsDialogMessage(hwnd, &msg)) {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }
                break;
            }
        }
    }
    return out;
}

DWORD WINAPI Dialog1::operationsThread(LPVOID lpParam)
{
    auto pJob = ((Dialog1*)lpParam)->mpJob;
    pJob->start(onUpdate);
    return 0;
}

void Dialog1::onUpdate(Job::Arguments::t_Pointer pArg, LPVOID lpParam)
{
    Events::UIParams ui;
    do
    {
        auto p = pArg;
        if (!p) break;

        ui.result = p->getResult();
        
        /* products scheduler */
        ui.progress1 = p->Progress;

        /* current product */
        p = p->Child;
        if (!p) break;
        ui.label1.set(p->Title);

        /* current task */
        p = p->Child;
        if (!p) break;
        ui.label2.set(p->Title);
        ui.label3.set(p->SubTitle);
        ui.progress2 = p->Progress;

    } while (0);
    Events::SendUIEvent(ui);
}

INT_PTR CALLBACK Dialog1::DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    static Dialog1* pDlg;
    switch (Msg)
    {
        case WM_INITDIALOG:
        {
            pDlg = (Dialog1 *)lParam;
            pDlg->hWnds.dialog = hWnd;
            pDlg->hWnds.label1 = GetDlgItem(hWnd, IDC_STATIC1);
            pDlg->hWnds.label2 = GetDlgItem(hWnd, IDC_STATIC2);
            pDlg->hWnds.label3 = GetDlgItem(hWnd, IDC_STATIC3);
            pDlg->hWnds.progress1 = GetDlgItem(hWnd, IDC_PROGRESS1);
            pDlg->hWnds.progress2 = GetDlgItem(hWnd, IDC_PROGRESS2);
            SendMessage(pDlg->hWnds.label1, WM_SETTEXT, NULL, (LPARAM)"");
            SendMessage(pDlg->hWnds.label2, WM_SETTEXT, NULL, (LPARAM)"");
            SendMessage(pDlg->hWnds.label3, WM_SETTEXT, NULL, (LPARAM)"");
            SendMessage(pDlg->hWnds.progress1, PBM_SETPOS, 0, 0);
            SendMessage(pDlg->hWnds.progress2, PBM_SETPOS, 0, 0);
            SendMessage(GetDlgItem(hWnd, IDCANCEL), WM_SETTEXT, NULL, (LPARAM)(RESS_STR_CANCEL.c_str()));
            pDlg->MatchPrepareToInstallPage();
            return TRUE;
        }
        case WM_COMMAND: {
            switch (wParam)
            {
            case IDCANCEL:
                Events::SendCancelEvent();
                return TRUE;
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return TRUE;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return TRUE;
    }
    return FALSE;
}

void Dialog1::MatchPrepareToInstallPage()
{
    RECT rect;
    if (hWnds.parent == NULL) return;
    if (!mbMatchPrepareToInstallPage) return;
    if (!GetWindowRect(hWnds.parent, &rect)) return;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    SetWindowPos(hWnds.dialog, hWnds.parent, rect.left, rect.top, width, height, 0);
}
