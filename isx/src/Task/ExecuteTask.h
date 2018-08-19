#pragma once
// project headers
#include "Task.h"

class ExecuteTask : public Task
{
    public:
		ExecuteTask(const char* workingDirectory, const char* command, const char* arguments) : Task("ExecuteTask") {};
    private:
        virtual const std::string main();
};
