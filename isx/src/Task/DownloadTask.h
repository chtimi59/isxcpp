#pragma once
#include "Task.h"

class DownloadTask : public Task
{
    public:
		DownloadTask(const char* url, const char* dest) :
			Task("DownloadTask"),
			url(url),
			dest(dest)
		{};

    private:
		const std::string url;
		const std::string dest;
        const std::string main();
};
