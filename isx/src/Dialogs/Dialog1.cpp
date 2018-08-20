// --- uncomment for UI debug ---
//#define DEBUG_INNO_LAYOUT

#include "common.h"
#include "Dialog1.h"
// project headers
#include "resources.h" // Dialog RC
#include "Events.h"    // Thread synchronization
#include "Job/Job.h"   // This dialog is here to reflect a Job progression
// std
#ifdef DEBUG_INNO_LAYOUT
#include <iomanip>     // std::hex
#include <sstream>     // std::stringstream
#endif
// system headers
#include <commctrl.h>

inline DWORD Progress(DWORD value) {
    return ISINSTALL ? value : (100 - value);
}

Dialog1::Dialog1(HWND hWnd, bool matchPrepareToInstallPage, Job::t_Pointer pJob)
{
    hWnds.parent = hWnd;
    mbMatchPrepareToInstallPage = matchPrepareToInstallPage;
    mpJob = pJob;
}

std::string Dialog1::show()
{
    /* Create Dialog */
    const HWND hwnd = CreateDialogParam(HINST, MAKEINTRESOURCE(IDD_DIALOG1), hWnds.parent, DlgProc, (LPARAM)this);
    if (hwnd == NULL) throw std::invalid_argument("couldn't create dialog");
    /* Create OperationsThread */
    const HANDLE hThread = CreateThread(NULL, 0, OperationsThread, this, 0, NULL);
    /* Define sensitive events: update UI | user cancel | OperationsThread terimated */
    const HANDLE evtHandles[] = {
        Events::UIEvent(),
        Events::UserCancelEvent(),
        hThread
    };
    const DWORD count = sizeof(evtHandles) / sizeof(HANDLE);

    std::string result = SUCCESS;
    bool bFirstUIUpdate = true;
    hWnds.dialog = hwnd;
    ShowWindow(hWnds.dialog, SW_SHOW);
    Events::UIParams uiDisplayed;
    while (TRUE)
    {
        /* Dialog Message loop | defined events */
        DWORD dwStatus = MsgWaitForMultipleObjects(count, evtHandles, FALSE, INFINITE, QS_ALLINPUT);
        switch (dwStatus)
        {
            /* Update UI */
            case WAIT_OBJECT_0:
            {
                auto ui = Events::GetCurrentUI();
                if (uiDisplayed.label1 != ui.label1 || bFirstUIUpdate)
                    SendMessage(hWnds.label1, WM_SETTEXT, NULL, ui.label1.getLParam());
                if (uiDisplayed.label2 != ui.label2 || bFirstUIUpdate)
                    SendMessage(hWnds.label2, WM_SETTEXT, NULL, ui.label2.getLParam());
                if (uiDisplayed.label3 != ui.label3 || bFirstUIUpdate)
                    SendMessage(hWnds.label3, WM_SETTEXT, NULL, ui.label3.getLParam());
                if (uiDisplayed.progress1 != ui.progress1 || bFirstUIUpdate)
                    SendMessage(hWnds.progress1, PBM_SETPOS, Progress(ui.progress1), 0);
                if (uiDisplayed.progress2 != ui.progress2 || bFirstUIUpdate)
                    SendMessage(hWnds.progress2, PBM_SETPOS, Progress(ui.progress2), 0);
                result = ui.result;
                uiDisplayed = ui;
                bFirstUIUpdate = false;
                break;
            }

            /* User Cancel */
            case WAIT_OBJECT_0 + 1:
            {
                /* kill the whole thread */
                TerminateThread(hThread, 1);
                result = res::getString(IDS_CANCELMSG);
                break;
            }

            /* Thread terminated */
            case WAIT_OBJECT_0 + 2:
            {
                DestroyWindow(hwnd);
                break;
            }

            /* Last, Dialog Messages... */
            case WAIT_OBJECT_0 + count:
            {
                MSG msg;
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    if (msg.message == WM_QUIT) {
                        TerminateThread(hThread, 1); // can it sill be active ?
                        return result;
                    }
                    if (!IsDialogMessage(hwnd, &msg)) {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }
                break;
            }
        } /* events switch case */
    } /* infinite loop */
}

DWORD WINAPI Dialog1::OperationsThread(LPVOID lpParam)
{
    auto pJob = ((Dialog1*)lpParam)->mpJob;
    pJob->start(UpdateProc);
    Sleep(500); // UX concern, make a short pause on 100% completed status
    return 0;
}

void Dialog1::UpdateProc(Job::Arguments::t_Pointer pArg, LPVOID lpParam)
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
            pDlg->hWnds.cancel = GetDlgItem(hWnd, IDCANCEL);
            
            SendMessage(pDlg->hWnds.label1, WM_SETTEXT, NULL, (LPARAM)"");
            SendMessage(pDlg->hWnds.label2, WM_SETTEXT, NULL, (LPARAM)"");
            SendMessage(pDlg->hWnds.label3, WM_SETTEXT, NULL, (LPARAM)"");
            SendMessage(pDlg->hWnds.progress1, PBM_SETPOS, 0, 0);
            SendMessage(pDlg->hWnds.progress2, PBM_SETPOS, 0, 0);

            SendMessage(pDlg->hWnds.cancel, WM_SETTEXT, NULL,
                (LPARAM)(res::getString(IDS_CANCELBTN).c_str()));
            SendMessage(hWnd, WM_SETTEXT, NULL,
                (LPARAM)(res::getString(IDS_DIALOGTITLE).c_str()));

            /* Innosetup layout */
            if (pDlg->mbMatchPrepareToInstallPage && pDlg->hWnds.parent)
                pDlg->matchInnoLayout();

            return TRUE;
        }
        case WM_COMMAND:
        {
            /* User click on Cancel */
            if (wParam == IDCANCEL) {
                Events::SendCancelEvent();
                return TRUE;
            }
            break;
        }
        case WM_CLOSE:
        {
            /* User close window (such as ALT-F4 or click X on menu bar) */
            Events::SendCancelEvent();
            return TRUE;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return TRUE;
        }
        
#ifdef DEBUG_INNO_LAYOUT
        case WM_ERASEBKGND:
        {
            // Magenta background
            HBRUSH brush;
            RECT rect;
            brush = CreateSolidBrush(RGB(255, 0, 255));
            SelectObject((HDC)wParam, brush);
            GetClientRect(hWnd, &rect);
            Rectangle((HDC)wParam, rect.left, rect.top, rect.right, rect.bottom);
            return TRUE;
        }
#endif 
    }
    return FALSE;
}

void Dialog1::matchInnoLayout()
{
    /* Use raw window style */
    SetWindowLongPtr(hWnds.dialog, GWL_STYLE, WS_CHILD);

#ifdef DEBUG_INNO_LAYOUT
    SetWindowLong(hWnds.dialog, GWL_EXSTYLE, 0);
    {
        std::stringstream stream;
        stream << "Please attach visual studio debugger\non this dialog now, and then click OK.\n";
        stream << "\n";
        stream << "On visual studio IDE, you may need to use 'tools/spy++'\nwith the following windows Handle:\n";
        stream << "\n";
        stream << "     " << std::hex << hWnds.parent;
        stream << "\n";
        std::string str(stream.str());
        MessageBox(NULL, str.c_str(), "ATTACH DEBUGGER ON ME", 0);
    }        
#else    
    SetWindowLong(hWnds.dialog, GWL_EXSTYLE, WS_EX_LAYERED);
    SetLayeredWindowAttributes(hWnds.dialog, GetSysColor(COLOR_3DFACE), 0, LWA_COLORKEY);
#endif
    
    /* Find out some controls position in InnoSetup */
    RECT outline = { 0 };
    RECT header = { 0 };
    RECT content = { 0 };
    RECT cancel  = { 0 };
    float refHeight = 1.0f;
    {
        HWND hwnd = hWnds.parent;
        GetWindowRect(hwnd, &outline);
        HWND nextBtn = FindWindowEx(hwnd, NULL, "TNewButton", NULL);
        HWND cancelBtn = FindWindowEx(hwnd, nextBtn, "TNewButton", NULL);
        GetWindowRect(cancelBtn, &cancel);
        HWND notebook = FindWindowEx(hwnd, NULL, "TNewNotebook", NULL);
        RECT tmp = { 0 };
        HWND notebookpage = FindWindowEx(notebook, NULL, "TNewNotebookPage", NULL);
        GetWindowRect(notebookpage, &tmp);
        refHeight = (float)(tmp.bottom - tmp.top);
        HWND panel = FindWindowEx(notebookpage, NULL, "TPanel", NULL);
        GetWindowRect(panel, &header);
        HWND page = FindWindowEx(notebookpage, NULL, "TNewNotebook", NULL);
        GetWindowRect(page, &content);
    }

    /* Dialog position */
    const int MARGIN = 4;
    int x0 = header.left + MARGIN;
    int y0 = header.bottom + MARGIN;
    int width = header.right - header.left - 2 * MARGIN;
    int height = cancel.bottom - header.bottom;

    /* Controls */
    {
        int textHeight = (int)((refHeight * 14.0f) / 313.0f);
        int ProgressHeight = (int)((refHeight * 21.0f) / 313.0f);
        int inter = (int)((refHeight * 14.0f) / 313.0f);
        
        int x = content.left - x0;
        int y = content.top - y0;
        int w = content.right - content.left;
        
        SetWindowPos(hWnds.label1, HWND_TOP, x, y, w, textHeight, 0);
        y += textHeight;
        SetWindowPos(hWnds.label2, HWND_TOP, x, y, w, textHeight, 0);
        y += textHeight;
        SetWindowPos(hWnds.label3, HWND_TOP, x, y, w, textHeight, 0);
        y += textHeight;
        y += inter;
        SetWindowPos(hWnds.progress1, HWND_TOP, x, y, w, ProgressHeight, 0);
        y += ProgressHeight;
        y += inter;
        SetWindowPos(hWnds.progress2, HWND_TOP, x, y, w, ProgressHeight, 0);
    }

    /* Cancel match Cancel */
    {
        int cancelHeight = cancel.bottom - cancel.top;
        int x = cancel.left - x0;
        int y = cancel.top - y0;
        int w = cancel.right - cancel.left;
        SetWindowPos(hWnds.cancel, HWND_TOP, x, y, w, cancelHeight, 0);
    }

    /* Dialog match outline */
    {
        SetWindowPos(hWnds.dialog, HWND_TOP, x0, y0, width, height, 0);
    }
}
