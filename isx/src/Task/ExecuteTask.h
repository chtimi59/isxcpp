#pragma once
#include "Task.h"

class ExecuteTask : public Task
{
    public:
        ExecuteTask(const char* workingDirectory, const char* command, const char* arguments, bool cancelable) :
            Task("ExecuteTask"),
            workingDirectory(workingDirectory),
            command(command),
            arguments(arguments),
            cancelable(cancelable)
        {}

        void kill(const std::string& reason);

    private:
        const bool cancelable;
        const std::string workingDirectory;
        const std::string command;
        const std::string arguments;
        const std::string main();

};
