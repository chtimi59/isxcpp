#pragma once
#include <windows.h>
#include <stdio.h>
#include <assert.h>
// std
#include <string>

#ifdef INSTANCIATE_UTILS
    TCHAR szExePath[MAX_PATH] = { 0 };
    TCHAR szLibPath[MAX_PATH] = { 0 };
    TCHAR szTmpPath[MAX_PATH] = { 0 };
    TCHAR szCurPath[MAX_PATH] = { 0 };
#else
    extern TCHAR szExePath[MAX_PATH];
    extern TCHAR szLibPath[MAX_PATH];
    extern TCHAR szTmpPath[MAX_PATH];
    extern TCHAR szCurPath[MAX_PATH];
#endif

void initUtils();

void DbgOutput(const char* szFormat, ...);

inline bool isSucceed(const char* result) {
    return strcmp(result, "") == 0;
}

inline bool isCanceled(const char* result) {
    return strcmp(result, "Operation canceled !") == 0;
}

inline void printResult(const char* result) {
    DbgOutput("");
    if (isSucceed(result)) {
        DbgOutput("Success !");
    }
    else
    {
        DbgOutput("Failed\n---\n%s\n---", result);
    }
    DbgOutput("");
}

namespace io
{
    inline void ThrowError(const char* errormsg) {
        fprintf(stderr, "fatal error '%s'\n", errormsg);
        assert(FALSE);
        exit(1);
    }

    void DbgOutput(const char* szFormat, ...);
    void DbgPopLastError();

    const std::string LTrim(const std::string& str);
    const std::string RTrim(const std::string& str, bool bRemovePS = false);
    std::string Basename(const std::string& path);
    std::string Dirname(const std::string& path);
    std::string PathCombine(const std::string& path1, const std::string& path2);
    std::string PathUnix2Win(const std::string& path);
    std::string PathWin2Unix(const std::string& path);
    std::string PathAbsolute(const std::string& path);
    void DirectoryCreate(const std::string& path);
    bool DirectoryExists(const std::string& path);
    bool DirectoryDelete(const std::string& path, UINT maxRetries = 10, UINT millisecondsDelay = 30);
}
