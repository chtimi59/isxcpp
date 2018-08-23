#pragma once
#include <windows.h>
#include <stdio.h>
// std
#include <string>

#ifdef INSTANCIATE_UTILS
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN TCHAR szExePath[MAX_PATH];
EXTERN TCHAR szLibPath[MAX_PATH];
EXTERN TCHAR szTmpPath[MAX_PATH];

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
        printf("Success !\n");
        DbgOutput("Success !");
    }
    else
    {
        printf("%s\n", result);
        DbgOutput("Failed\n---\n%s\n---", result);
    }
    DbgOutput("");
}