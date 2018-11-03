#include "common.h"
#include "EmptyTask.h"

const std::string EmptyTask::main()
{
    setProgress(100);
    sendUpdate();
    return SUCCESS;
}
