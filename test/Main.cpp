#include <stdio.h>
#include <windows.h>

typedef void(__stdcall *tInitialize)(bool, const char*, const char*, const char*);
typedef void(__stdcall *tClearProducts)();
typedef int(__stdcall *tCreateProduct)(const char*);
typedef void(__stdcall *tAddFakeTask)(int, const char*);
typedef const char * (__stdcall *tGetReadyMemo)(const char*, const char*);
typedef const char * (__stdcall *tRun)(int, bool);
typedef void (__stdcall *tWait)(int);

void poplasterror() {
    char buff[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buff, (sizeof(buff) / sizeof(wchar_t)), NULL);
    MessageBox(NULL, buff, "error", MB_OK);
}
int main()
{
    printf("Hi!\n");
    HMODULE hGetProcIDDLL = LoadLibrary("isx.dll");
    if (!hGetProcIDDLL) {
        printf("could not load the dynamic library");
        poplasterror();
        return EXIT_FAILURE;
    }
    tInitialize Initialize = (tInitialize)GetProcAddress(hGetProcIDDLL, "Initialize");
    tClearProducts ClearProducts = (tClearProducts)GetProcAddress(hGetProcIDDLL, "ClearProducts");
    tCreateProduct CreateProduct = (tCreateProduct)GetProcAddress(hGetProcIDDLL, "CreateProduct");
    tAddFakeTask AddFakeTask = (tAddFakeTask)GetProcAddress(hGetProcIDDLL, "AddFakeTask");
    tGetReadyMemo GetReadyMemo = (tGetReadyMemo)GetProcAddress(hGetProcIDDLL, "GetReadyMemo");
    tRun Run = (tRun)GetProcAddress(hGetProcIDDLL, "Run");
    tWait Wait = (tWait)GetProcAddress(hGetProcIDDLL, "Wait");

    if (!Initialize) { poplasterror(); return EXIT_FAILURE; }
    if (!ClearProducts) { poplasterror(); return EXIT_FAILURE; }
    if (!CreateProduct) { poplasterror(); return EXIT_FAILURE; }
    if (!AddFakeTask) { poplasterror(); return EXIT_FAILURE; }
    if (!GetReadyMemo) { poplasterror(); return EXIT_FAILURE; }
    if (!Run) { poplasterror(); return EXIT_FAILURE; }
    if (!Wait) { poplasterror(); return EXIT_FAILURE; }

    /* --- */

    Initialize(true, "C:\\dev\\isx\\test\\tmpstub", "\\", "fr");
    int i = CreateProduct("My Product1");
    int j = CreateProduct("My Product2");
    ClearProducts();
    i = CreateProduct("My Product1");
    j = CreateProduct("My Product2");
    printf(GetReadyMemo("...","\n"));
    AddFakeTask(666, "Task fail");
    AddFakeTask(i, "Task 1.1");
    AddFakeTask(i, "Task 1.2");
    AddFakeTask(j, "Task 2.1");
    AddFakeTask(j, "Task 2.2");
    //Wait(1000);
    printf(Run(0, false));
    printf("\n");

    printf("Success!");
    return EXIT_SUCCESS;
}
