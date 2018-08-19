#include "DeleteTask.h"
// project headers
#include "Utils/io.h"
#include "Resources/res.h"

const std::string DeleteTask::main()
{
	setTitle(ressources::getString(IDS_TASKDELETE));
	setProgress(0);
	
	if (!io::DirectoryDelete(path)) {
		auto msg = ressources::getString(IDS_TASKDELETEERROR, path.c_str());
		auto title = ressources::getString(IDS_ERROR);
		if (exitIfFail) return msg;
		MessageBox(NULL, msg.c_str(), title.c_str(), MB_OK | MB_ICONWARNING);
		return SUCCESS;
	};

	setProgress(100);
    return SUCCESS;
}

