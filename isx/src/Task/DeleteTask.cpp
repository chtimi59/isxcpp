#include "common.h"
#include "DeleteTask.h"

const std::string DeleteTask::main()
{
    setTitle(res::getString(IDS_TASKDELETE));
    setProgress(0);
    
    bool done = io::DirectoryDelete(path);

    if (!done) {
        auto msg = res::getString(IDS_TASKDELETEERROR, path.c_str());
        auto title = res::getString(IDS_ERROR);
        if (exitIfFail) return msg;
        io::MsgBox(msg, title);
        return SUCCESS; // just warning
    };

    setProgress(100);
    return SUCCESS;
}

