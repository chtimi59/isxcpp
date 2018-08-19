#pragma once
// project headers
#include "Task.h"

class UnZipTask : public Task
{
    public:
		UnZipTask(const char* path, const char* dst, bool clear) : Task("UnZipTask") {};
    private:
        virtual const std::string main();
};
