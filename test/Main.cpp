#include "api.h"

#include <string>
#include <iostream>  
#include <iomanip> 

#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

#include "..\libzip\inc\zip.h"

int main()
{

#if 1
    //Open the ZIP archive
    int err = 0;
    zip *z = zip_open("foo.zip", 0, &err);

    //Search for the file of given name
    const char *name = "file.txt";
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, name, 0, &st);

    //Alloc memory for its uncompressed contents
    char *contents = new char[st.size];

    //Read the compressed file
    zip_file *f = zip_fopen(z, name, 0);
    zip_fread(f, contents, st.size);
    zip_fclose(f);

    //And close the archive
    zip_close(z);

    //Do something with the contents
    //delete allocated memory
    delete[] contents;


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

