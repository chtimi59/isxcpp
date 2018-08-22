#define INSTANCIATE_UTILS
#include "utils.h"
// System Header
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>
#include <string>

void initUtils()
{
    EXTERN TCHAR szTmp[MAX_PATH];

    GetModuleFileName(NULL, szTmp, MAX_PATH);
    PathRemoveFileSpec(szTmp); // rootdir/test/bin
    
    PathCombine(szTmpPath, szTmp, "tmp"); // rootdir/test/bin/tmp
    CreateDirectory(szTmpPath, NULL);

    PathRemoveFileSpec(szTmp); // rootdir/test/
    strcpy_s(szExePath, MAX_PATH, szTmp); // rootdir/test/
    

    PathRemoveFileSpec(szTmp); // rootdir/
    PathCombine(szTmp, szTmp, "isx"); // rootdir/isx
    PathCombine(szTmp, szTmp, "bin"); // rootdir/isx/bin
    PathCombine(szTmp, szTmp, "isx.dll"); // rootdir/isx/bin/isx.dll
    strcpy_s(szLibPath, MAX_PATH, szTmp);
}

void DbgOutput(const char* szFormat, ...) {
    char szBuff[MAX_PATH];
    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);
    strcat_s(szBuff, MAX_PATH, "\n");
    OutputDebugString("isx-test > ");
    OutputDebugString(szBuff);
}

void DbgPopLastError() {
    char buff[MAX_PATH];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buff, (sizeof(buff) / sizeof(wchar_t)), NULL);
    MessageBox(NULL, buff, "error", MB_OK);
}