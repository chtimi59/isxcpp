#include "utils.h"

#include <string>
#include <iostream>
#include <iomanip>

#include <isx.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <cctype>
#include <algorithm>
#include <map>

void testjson();

int main()
{
    DbgOutput("Hi there!");
    initUtils();

    testjson();

#if 1
    {
    int code = 0;
    auto data = HttpGet("http://localhost:8081/json", &code);

    int hJson, hArray;
    JsonParse(data, &hJson);
    JsonObj(hJson, "object.array", false, &hArray);

    const char* str = "POST !!";
    JsonStringFromIdx(hArray, 1, true, &str);
    JsonObj(hJson, "object.array", true, &hArray);

    const char* out;
    JsonStringify(hJson, &out);
    data = HttpPost("http://localhost:8081/api", "application/json", out, &code);
    printf("Response %u\n%s", code, data);
}
#endif

#if 0
    bool bRedo = true;
    while(bRedo)
    {
        DbgOutput("");
        DbgOutput("[Install]");
        DbgOutput("");

        Initialize(TRUE, FALSE, "fr", NULL);

        int p1, p2;
        const char* result = NULL;

        ClearProducts();
        p1 = CreateProduct("My Product1");
        p2 = CreateProduct("My Product2");
        ClearProducts();
        p1 = CreateProduct("My Product1");
        p2 = CreateProduct("My Product2");
        assert(p1 == 0 && p2 == 1);
        DbgOutput(GetReadyMemo("...", "\n"));

        AddFakeTask(666, "Task fail");
        AddFakeTask(p1, "Task 1.1");

        TCHAR a[MAX_PATH];
        TCHAR b[MAX_PATH];

        sprintf_s(a, MAX_PATH, "http://download.microsoft.com/download/0/5/6/056dcda9-d667-4e27-8001-8a0c6971d6b1/vcredist_x64.exe");
        sprintf_s(b, MAX_PATH, "%s\\test\\vcredist_x64.exe", szTmpPath);
        AddDownloadTask(p1, a, b);

        /*sprintf_s(a, MAX_PATH, "%s\\test\\", szTmpPath);
        AddExecuteTask(p1, a, "vcredist_x64.exe", "/passive /norestart", TRUE);*/

        sprintf_s(a, MAX_PATH, "%s\\samples\\testzip.zip", szCurPath);
        sprintf_s(b, MAX_PATH, "%s\\unzip\\", szTmpPath);
        AddUnZipTask(p1, a, b, TRUE);

        sprintf_s(b, MAX_PATH, "%s\\unzip-delete\\", szTmpPath);
        AddUnZipTask(p1, a, b, TRUE);

        sprintf_s(a, MAX_PATH, "%s\\unzip-delete\\\\\\\\\\      ", szTmpPath);
        AddDeleteTask(p1, a, false);

        sprintf_s(a, MAX_PATH, "%s\\samples\\", szCurPath);
        AddExecuteTask(p2, a, "testapp.exe", "0 foo bar", true); // first parm is the exitcode of testapp.exe

        sprintf_s(a, MAX_PATH, "%s\\unzip\\folder2", szTmpPath);
        AddExecuteTask(p2, a, "notepad.exe", "compressable", true);

        result = Run(0, false);
        printResult(result);

        bRedo = isCanceled(result);
        if (bRedo)
        {
            DbgOutput("");
            DbgOutput("[Uninstall]");
            DbgOutput("");

            Initialize(FALSE, FALSE, "en", NULL);
            ClearProducts();
            p1 = CreateProduct("My Product1");
            AddFakeTask(p1, "Task 1.1");
            AddFakeTask(p1, "Task 1.2");
            result = Run(0, false);
            printResult(result);
            if (isCanceled(result)) bRedo = false;
            Wait(2000);
        }
    };
#endif

    DbgOutput("");
    DbgOutput("end!");
    return EXIT_SUCCESS;
}
