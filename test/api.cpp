#define INSTANCIATE_API
#include "api.h"
#include "utils.h"
// System Header
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

static HMODULE hModule = NULL;
FARPROC WINAPI GetProc(char* name) {
    auto fct = GetProcAddress(hModule, name);
    if (fct) return fct;
    DbgOutput("'%s' unfound", name);
    return NULL;
}

bool loadAPI()
{
    FILE *f = NULL;
    char szDLLPath[MAX_PATH]  ;
    do
    {
        sprintf_s(szDLLPath, MAX_PATH, "%s\\isx.dll", szExePath);
        DbgOutput("try (%s)", szDLLPath);
        fopen_s(&f, szDLLPath, "r");
        if (f) break;

        sprintf_s(szDLLPath, MAX_PATH, "%s\\isx.dll", szLibPath);
        DbgOutput("try (%s)", szDLLPath);
        fopen_s(&f, szDLLPath, "r");
        if (f) break;

        sprintf_s(szDLLPath, MAX_PATH, "isx.dll");
        DbgOutput("try (%s)", szDLLPath);
        break;

    } while (0);
    if (f) fclose(f);
    
    if (!(hModule = LoadLibrary(szDLLPath))) {
        DbgOutput("LoadLibrary(%s) - failed", szDLLPath);
        return false;
    }    
    if (!(Initialize = (tInitialize)GetProc("Initialize"))) return false;
    if (!(ClearProducts = (tClearProducts)GetProc("ClearProducts"))) return false;
    if (!(CreateProduct = (tCreateProduct)GetProc("CreateProduct"))) return false;
    if (!(AddDeleteTask = (tAddDeleteTask)GetProc("AddDeleteTask"))) return false;
    if (!(AddDownloadTask = (tAddDownloadTask)GetProc("AddDownloadTask"))) return false;
    if (!(AddExecuteTask = (tAddExecuteTask)GetProc("AddExecuteTask"))) return false;
    if (!(AddFakeTask = (tAddFakeTask)GetProc("AddFakeTask"))) return false;
    if (!(AddUnZipTask = (tAddUnZipTask)GetProc("AddUnZipTask"))) return false;
    if (!(GetReadyMemo = (tGetReadyMemo)GetProc("GetReadyMemo"))) return false;
    if (!(Run = (tRun)GetProc("Run"))) return false;
    if (!(Wait = (tWait)GetProc("Wait"))) return false;
    return true;
}