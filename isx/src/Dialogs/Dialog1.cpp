#include "Dialog1.h"
// project headers
#include "Resources/res.h"
#include "resources.h"
#include "Events.h"
#include "Job/JobsScheduler.h"
// std
#include <memory>
#include <string>
#include <iomanip> // std::hex
#include <sstream> // std::stringstream
// system headers
#include <windows.h>
#include <commctrl.h>
#include <ShellScalingApi.h>

//#define DEBUG_INNO_LAYOUT

extern HINSTANCE hInst;

std::string Dialog1::Dialog1::RESS_STR_CANCEL;
std::string Dialog1::Dialog1::RESS_STR_TITLE;

Dialog1::Dialog1(HWND hWnd, bool matchPrepareToInstallPage, Job::t_Pointer pJob)
{
    RESS_STR_CANCEL = ressources::getString(IDS_CANCEL);
	RESS_STR_TITLE = ressources::getString(IDS_DIALOGTITLE);

    hWnds.parent = hWnd;
    mbMatchPrepareToInstallPage = matchPrepareToInstallPage;
    mpJob = pJob;
}

std::string Dialog1::show()
{
    std::string out;

    HWND hwnd = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnds.parent, DlgProc, (LPARAM)this);
    if (hwnd == NULL) throw std::invalid_argument("couldn't create dialog");
    hWnds.dialog = hwnd;
    ShowWindow(hWnds.dialog, SW_SHOW);
    HANDLE hThread = CreateThread(NULL, 0, OperationsThread, this, 0, NULL);

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

DWORD WINAPI Dialog1::OperationsThread(LPVOID lpParam)
{
    auto pJob = ((Dialog1*)lpParam)->mpJob;
    pJob->start(UpdateProc);
	Sleep(500);
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

            SendMessage(pDlg->hWnds.cancel, WM_SETTEXT, NULL, (LPARAM)(RESS_STR_CANCEL.c_str()));			
			SendMessage(hWnd, WM_SETTEXT, NULL, (LPARAM)(RESS_STR_TITLE.c_str()));

			if (pDlg->mbMatchPrepareToInstallPage && pDlg->hWnds.parent) {
				pDlg->matchInnoLayout();
			}
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
		{
			PostQuitMessage(0);
			return TRUE;
		}
        case WM_CLOSE:
		{
			Events::SendCancelEvent();
			return TRUE;
		}
#ifdef DEBUG_INNO_LAYOUT
		// Magenta background
		case WM_ERASEBKGND:
		{
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
		/*int MARGIN = 1;
		outline.top -= MARGIN;
		outline.bottom += MARGIN;
		outline.left = MARGIN;
		outline.right -= MARGIN;*/

		SetWindowPos(hWnds.dialog, HWND_TOP, x0, y0, width, height, 0);
	}
		// Get mbMatchPrepareToInstallPage
	//	GetWindowRect(hWnds.parent, &rect);
		/*
		float dpi_scale = 1;
		POINT    point;
		UINT     dpiX = 0, dpiY = 0;
		point.x = rect.left;
		point.y = rect.top;
		auto hMonitor = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);
		auto hr = GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
		if (hr != S_OK)
		{
			MessageBox(NULL, (LPCWSTR)L"GetDpiForMonitor failed", (LPCWSTR)L"Notification", MB_OK);
			return FALSE;
		}

		std::ceil(((GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME)) * dpi_scale) + GetSystemMetrics(SM_CXPADDEDBORDER))

		rect.top += CaptionHeight;
		*/
	//}
	
}

BOOL CALLBACK Dialog1::EnumChildWindowsProc(HWND hWnd, LPARAM lParam)
{
	auto pDlg = (Dialog1 *)lParam;
	//0 == Main
	//1 == title ?
	//1 == title ?
	if (pDlg->mEnumChildWindowsIdx == 1) {
		RECT rect;
		GetWindowRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		SetWindowPos(pDlg->hWnds.dialog, pDlg->hWnds.parent, rect.left, rect.top, width, height, 0);
		return FALSE;
	}

	pDlg->mEnumChildWindowsIdx++;
	return TRUE;
}