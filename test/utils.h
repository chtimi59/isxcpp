#pragma once
#include <windows.h>
#include <stdio.h>
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
void DbgPopLastError();
void DbgOutput(const char* szFormat, ...);
bool DirectoryExists(const std::string& path);
bool DirectoryDelete(const std::string& path);

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