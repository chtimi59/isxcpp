#pragma once
#include <windows.h>

#ifdef INSTANCIATE_API
#define EXTERN
#define DEFAULT =NULL
#else
#define EXTERN extern
#define DEFAULT
#endif

typedef void(__stdcall *tInitialize)(bool isInstall, const char* tmpPath, const char* isxPath, const char* lang);
EXTERN tInitialize Initialize DEFAULT;
typedef void(__stdcall *tClearProducts)();
EXTERN tClearProducts ClearProducts DEFAULT;
typedef int(__stdcall *tCreateProduct)(const char* name);
EXTERN tCreateProduct CreateProduct DEFAULT;
typedef void(__stdcall *tAddDeleteTask)(int productIndex, const char* path);
EXTERN tAddDeleteTask AddDeleteTask DEFAULT;
typedef void(__stdcall *tAddDownloadTask)(int productIndex,	const char* url, const char* dest);
EXTERN tAddDownloadTask AddDownloadTask DEFAULT;
typedef void(__stdcall *tAddExecuteTask)(int productIndex, const char* workingDirectory, const char* command, const char* arguments);
EXTERN tAddExecuteTask AddExecuteTask DEFAULT;
typedef void(__stdcall *tAddFakeTask)(int productIndex,	const char* name);
EXTERN tAddFakeTask AddFakeTask DEFAULT;
typedef void(__stdcall *tAddUnZipTask)(int productIndex, const char* path, const char* dst,	bool clear);
EXTERN tAddUnZipTask AddUnZipTask DEFAULT;
typedef const char * (__stdcall *tGetReadyMemo)(const char* space, const char* newLine);
EXTERN tGetReadyMemo GetReadyMemo DEFAULT;
typedef const char * (__stdcall *tRun)(int hWnd, bool matchPrepareToInstallPage);
EXTERN tRun Run DEFAULT;
typedef void(__stdcall *tWait)(int ms);
EXTERN tWait Wait DEFAULT;

EXTERN TCHAR szRootPath[MAX_PATH + 1];
EXTERN TCHAR szTmpPath[MAX_PATH + 1];
bool loadAPI();