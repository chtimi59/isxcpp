#define INSTANCIATE_API
#include "api.h"
// System Header
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

void poplasterror() {
    char buff[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buff, (sizeof(buff) / sizeof(wchar_t)), NULL);
    MessageBox(NULL, buff, "error", MB_OK);
}

bool loadAPI()
{
    TCHAR szLibPath[MAX_PATH + 1];
    GetModuleFileName(NULL, szLibPath, MAX_PATH + 1);
    PathRemoveFileSpec(szLibPath); // rootdir/test/bin
    PathCombine(szTmpPath, szLibPath, "tmp"); // rootdir/test/bin/tmp
    PathRemoveFileSpec(szLibPath); // rootdir/test/
    PathRemoveFileSpec(szLibPath); // rootdir/
    PathCombine(szLibPath, szLibPath, "isx"); // rootdir/isx
    PathCombine(szLibPath, szLibPath, "bin"); // rootdir/isx/bin
    PathCombine(szLibPath, szLibPath, "isx.dll");
    CreateDirectory(szTmpPath, NULL);

    HMODULE hGetProcIDDLL = LoadLibrary(szLibPath);
    if (!hGetProcIDDLL) {
        printf("could not load the dynamic library");
        poplasterror();
        return false;
    }

    Initialize = (tInitialize)GetProcAddress(hGetProcIDDLL, "Initialize");
    if (!Initialize) { poplasterror(); return false; }
    ClearProducts = (tClearProducts)GetProcAddress(hGetProcIDDLL, "ClearProducts");
    if (!ClearProducts) { poplasterror(); return false; }
    CreateProduct = (tCreateProduct)GetProcAddress(hGetProcIDDLL, "CreateProduct");
    if (!CreateProduct) { poplasterror(); return false; }
    AddDeleteTask = (tAddDeleteTask)GetProcAddress(hGetProcIDDLL, "AddDeleteTask");
    if (!AddDeleteTask) { poplasterror(); return false; }
    AddDownloadTask = (tAddDownloadTask)GetProcAddress(hGetProcIDDLL, "AddDownloadTask");
    if (!AddDownloadTask) { poplasterror(); return false; }
    AddExecuteTask = (tAddExecuteTask)GetProcAddress(hGetProcIDDLL, "AddExecuteTask");
    if (!AddExecuteTask) { poplasterror(); return false; }
    AddFakeTask = (tAddFakeTask)GetProcAddress(hGetProcIDDLL, "AddFakeTask");
    if (!AddFakeTask) { poplasterror(); return false; }
    AddUnZipTask = (tAddUnZipTask)GetProcAddress(hGetProcIDDLL, "AddUnZipTask");
    if (!AddUnZipTask) { poplasterror(); return false; }
    GetReadyMemo = (tGetReadyMemo)GetProcAddress(hGetProcIDDLL, "GetReadyMemo");
    if (!GetReadyMemo) { poplasterror(); return false; }
    Run = (tRun)GetProcAddress(hGetProcIDDLL, "Run");
    if (!Run) { poplasterror(); return false; }
    Wait = (tWait)GetProcAddress(hGetProcIDDLL, "Wait");
    if (!Wait) { poplasterror(); return false; }

    return true;
}