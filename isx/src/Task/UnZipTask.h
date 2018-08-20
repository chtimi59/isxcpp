#pragma once
#include "Task.h"

class UnZipTask : public Task
{
    public:
		UnZipTask(const char* path, const char* dst, bool clear) :
			Task("UnZipTask"),
			path(path),
			dst(dst),
			clear(clear)
		{};

    private:
		const std::string path;
		const std::string dst;
		const bool clear;
        const std::string main();
};
