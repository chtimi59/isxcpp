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

    int err = 0;
    char buf[100];

    const char *archive = "C:\\dev\\isxcpp\\2.zip";
    struct zip *za;
    if ((za = zip_open(archive, 0, &err)) == NULL) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        fprintf(stderr, "%s/n", buf);
        return 1;
    }

    int retcode = 0;
    struct zip_stat sb;
    for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            int len = strlen(sb.name);
            printf("%s\n", sb.name);

            /*struct zip_file *zf;
            zf = zip_fopen_index(za, i, 0);
            if (!zf) {
                retcode = 1;
                break;
            }

            FILE * f = fopen(sb.name, "wb");
            if (!f) {
                zip_fclose(zf);
                retcode = 1;
                break;
            }

            zip_uint64_t sum = 0;
            while (sum != sb.size) {
                zip_uint64_t len = zip_fread(zf, buf, 100);
                if (len < 0) {
                    retcode = 1;
                    break;
                }
                fwrite(buf, len, 1, f);
                sum += len;
            }
            
            fclose(f);
            zip_fclose(zf);
            */
        }
    }

    zip_close(za);

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

