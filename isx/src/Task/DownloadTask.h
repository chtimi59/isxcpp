#pragma once
// project headers
#include "Task.h"

class DownloadTask : public Task
{
    public:
		DownloadTask(const char* url, const char* dest) : Task("DownloadTask") {};
    private:
        virtual const std::string main();
};
