#pragma once
#include "Task.h"

class FakeTask : public Task
{
    public:
        FakeTask(std::string title) :
            Task(title)
        {};

    private:
        const std::string main();
};
