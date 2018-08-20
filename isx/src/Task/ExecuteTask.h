#pragma once
#include "Task.h"

class ExecuteTask : public Task
{
    public:
        ExecuteTask(const char* workingDirectory, const char* command, const char* arguments) :
            Task("ExecuteTask"),
            workingDirectory(workingDirectory),
            command(command),
            arguments(arguments)
        {};

    private:
        const std::string workingDirectory;
        const std::string command;
        const std::string arguments;
        const std::string main();
};
