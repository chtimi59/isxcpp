#include "DeleteTask.h"

const std::string DeleteTask::main()
{
    std::string ret;
    for (int i = 0; i <= 10; i++)
    {
        //if (i == 50) return "oupsy";
        
        setProgress(i*10);
        setSubTitle("some subtitle");
        sendUpdate();
        Sleep(100);
    }
    return ret;
}

