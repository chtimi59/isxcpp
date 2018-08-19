#pragma once
// project headers
#include "Task.h"

class DeleteTask : public Task
{
    public:
		DeleteTask(std::string path) : Task("DeleteTask") {};
    private:
        virtual const std::string main();
};
