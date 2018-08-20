#pragma once
#include "Task.h"

class ExecuteTask : public Task
{
    public:
        ExecuteTask(const char* workingDirectory, const char* command, const char* arguments, bool cancelable);
        ~ExecuteTask();
        void kill(const std::string& reason);

    private:
        const bool cancelable;
        const std::string workingDirectory;
        const std::string command;
        const std::string arguments;
        const std::string main();

    private:
        CRITICAL_SECTION killReasonLock;
        HANDLE killEvent = NULL;
        std::string killReason;
};
