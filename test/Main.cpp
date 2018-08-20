#include "api.h"

#include <string>
#include <iostream>  
#include <iomanip> 

#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

#include "..\zlib\inc\zlib.h"

int main()
{

#if 1
    BYTE buff[32];
    crc32(123, buff, 32);

#else
    if (!loadAPI()) return EXIT_FAILURE;
    Initialize(FALSE, FALSE, "fr", NULL);
    bool bRedo = true;
    while(bRedo)
    {
        int p1, p2;
        const char* result = NULL;

        ClearProducts();
        p1 = CreateProduct("My Product1");
        p2 = CreateProduct("My Product2");
        ClearProducts();
        p1 = CreateProduct("My Product1");
       /* p2 = CreateProduct("My Product2");
        assert(p1 == 0 && p2 == 1); */
        printf(GetReadyMemo("...", "\n"));

        AddFakeTask(666, "Task fail");
        //AddDeleteTask(p1, "C:\\dev\\isxcpp\\sdf\\\\\\\\      ", false);
        AddFakeTask(p1, "Task 1.1");
        //AddFakeTask(p1, "Task 1.2");

        //AddExecuteTask(p2, "c:\\dev", "C:\\Program Files\\Microsoft VS Code\\Code.exe", "");
        /*AddExecuteTask(p1, "c:\\dev", "notepad.exe", "info.txt", true);
        AddExecuteTask(p1, "c:\\dev", "notepad.exe", "", false);
        AddExecuteTask(p1, "c:\\dev", "notepads.exe", "", true);*/
        //AddFakeTask(p2, "Task 2.2");

        result = Run(0, false);
        printf("%s\n", result);

        bRedo = strcmp(result, "Operation canceled !") == 0;
        if (bRedo) {
            printf("Redo!\n");
            Wait(2000);
        }
    };

    printf("Success!");
#endif
    return EXIT_SUCCESS;
}

