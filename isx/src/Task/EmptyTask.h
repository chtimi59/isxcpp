#pragma once
#include "Task.h"

class EmptyTask : public Task
{
    public:
        EmptyTask():
            Task("EmptyTask")
        {
            /* Means this task wont affect JobScheduler progress count or even potential UI */
            this->mState->IsEmptyTask = true;
        };
    private:
        const std::string main();
};
