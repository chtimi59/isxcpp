#define INSTANCIATE_UTILS
#include "utils.h"
// System Header
#include <stdio.h>
#include <windows.h>
#include <string>

void initUtils()
{
    std::string tmp;
    TCHAR szModuleName[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, szCurPath); // rootdir/test/
    GetModuleFileName(NULL, szModuleName, MAX_PATH); // rootdir/test/bin/text.exe

    tmp = io::Dirname(szModuleName); // rootdir/test/bin
    strcpy_s(szExePath, MAX_PATH, tmp.c_str()); // rootdir/test/bin
    
    tmp  = io::PathCombine(tmp, "tmp"); // rootdir/test/bin/tmp
    strcpy_s(szTmpPath, MAX_PATH, tmp.c_str()); // rootdir/test/bin
    io::DirectoryDelete(tmp);
    CreateDirectory(szTmpPath, NULL);

    tmp = io::Dirname(tmp); // rootdir/test/
    tmp = io::Dirname(tmp); // rootdir/
    tmp = io::PathCombine(tmp, "isx"); // rootdir/isx
    tmp = io::PathCombine(tmp, "bin"); // rootdir/isx/bin
    strcpy_s(szLibPath, MAX_PATH, tmp.c_str());
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
    printf_s(szBuff, MAX_PATH);
}

void DbgPopLastError() {
    char buff[MAX_PATH];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buff, (sizeof(buff) / sizeof(wchar_t)), NULL);
    MessageBox(NULL, buff, "error", MB_OK);
}

bool DirectoryExists(const std::string& path) {
    DWORD ftyp = GetFileAttributes(path.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;  //something is wrong with your path!
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;   // this is a directory!
    return false;    // this is not a directory!
}

bool DirectoryDelete(const std::string& path) {
    if (path.empty()) return TRUE;
    if (!DirectoryExists(path)) return TRUE;

    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;
    std::string search = path +  "\\*";
    hFind = FindFirstFile(search.c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) return FALSE;

    while (TRUE)
    {
        /* end */
        if (!FindNextFile(hFind, &FindFileData)) {
            if (GetLastError() == ERROR_NO_MORE_FILES) break;
            FindClose(hFind);
            return FALSE;
        }

        /* skip '.' and '..' */
        std::string item(FindFileData.cFileName);
        if (item == "." || item == "..") continue;

        std::string filename = path + "\\" + item;

        if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            /* item is a directory (recursive call) */
            if (!DirectoryDelete(filename)) {
                FindClose(hFind);
                return FALSE;
            }
        }
        else
        {
            /* item is a file, delete it */
            if (!DeleteFile(filename.c_str())) {
                FindClose(hFind);
                return FALSE;
            }
        }
    }
    FindClose(hFind);

    /* actual folder deletion */
    if (!RemoveDirectory(path.c_str())) return FALSE;
    return TRUE;
}