#pragma once
#include "Task.h"

class DeleteTask : public Task
{
    public:
        DeleteTask(std::string path, bool exitIfFail) :
            Task("DeleteTask"),
            path(path),
            exitIfFail(exitIfFail)
        {};
    private:
        const std::string path;
        const bool exitIfFail;
        const std::string main();
};
