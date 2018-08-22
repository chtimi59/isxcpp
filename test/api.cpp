#define INSTANCIATE_API
#include "api.h"
// System Header
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

bool loadAPI(const char* dllpath)
{
    TCHAR szPath[MAX_PATH];
    if (dllpath != NULL)
    {
        strcpy_s(szPath, MAX_PATH, dllpath);
    }
    else
    {
        strcpy_s(szPath, MAX_PATH, "isx.dll");
    }

    HMODULE hGetProcIDDLL = LoadLibrary(szPath);
    if (!hGetProcIDDLL) return false;
    Initialize = (tInitialize)GetProcAddress(hGetProcIDDLL, "Initialize");
    if (!Initialize) return false;
    ClearProducts = (tClearProducts)GetProcAddress(hGetProcIDDLL, "ClearProducts");
    if (!ClearProducts) return false;
    CreateProduct = (tCreateProduct)GetProcAddress(hGetProcIDDLL, "CreateProduct");
    if (!CreateProduct) return false;
    AddDeleteTask = (tAddDeleteTask)GetProcAddress(hGetProcIDDLL, "AddDeleteTask");
    if (!AddDeleteTask) return false;
    AddDownloadTask = (tAddDownloadTask)GetProcAddress(hGetProcIDDLL, "AddDownloadTask");
    if (!AddDownloadTask) return false;
    AddExecuteTask = (tAddExecuteTask)GetProcAddress(hGetProcIDDLL, "AddExecuteTask");
    if (!AddExecuteTask) return false;
    AddFakeTask = (tAddFakeTask)GetProcAddress(hGetProcIDDLL, "AddFakeTask");
    if (!AddFakeTask) return false;
    AddUnZipTask = (tAddUnZipTask)GetProcAddress(hGetProcIDDLL, "AddUnZipTask");
    if (!AddUnZipTask) return false;
    GetReadyMemo = (tGetReadyMemo)GetProcAddress(hGetProcIDDLL, "GetReadyMemo");
    if (!GetReadyMemo) return false;
    Run = (tRun)GetProcAddress(hGetProcIDDLL, "Run");
    if (!Run) return false;
    Wait = (tWait)GetProcAddress(hGetProcIDDLL, "Wait");
    if (!Wait) return false;
    return true;
}