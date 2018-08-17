#pragma once
// project headers
#include "Task.h"

class FakeTask : public Task
{
    public:
        FakeTask(std::string title) : Task(title) {};
    private:
        virtual const std::string main();
};
