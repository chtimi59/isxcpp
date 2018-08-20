#include "common.h"
#include "FakeTask.h"

const std::string FakeTask::main()
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

