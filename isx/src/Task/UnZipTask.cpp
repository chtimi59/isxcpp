#include "common.h"
#include "UnZipTask.h"

const std::string UnZipTask::main()
{
    for (int i = 0; i <= 10; i++)
    {
        //if (i == 50) return "oupsy";
        
        setProgress(i*10);
        setSubTitle("some subtitle");
        sendUpdate();
        Sleep(100);
    }
    return SUCCESS;
}

