#pragma once
// project headers
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
		std::string path;
		bool exitIfFail;
        virtual const std::string main();
};
