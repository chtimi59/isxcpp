#include "Task.h"

void Task::setProgress(DWORD val) {
    mPArg->Progress = val;
}

void Task::setTitle(const std::string& val) {
	mPArg->Title = val;
}

void Task::setSubTitle(const std::string& val) {
    mPArg->SubTitle = val;
}

void Task::start(t_UpdateCb onUpdate, LPVOID lpParam)
{
    Job::start(onUpdate, lpParam);
    auto ret = main();
    if (ret.empty())
    {
        mPArg->setStatus(Arguments::TerminatedWithSuccess);
    }
    else
    {
        mPArg->setStatus(Arguments::TerminatedWithError, ret);
    }
    sendUpdate();
}

