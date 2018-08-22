#include "common.h"
#include "Task.h"


Task::Task(std::string title) : Job(title)
{
    killEvent = CreateEvent(NULL, FALSE, FALSE, "TaskKill");
    if (killEvent == NULL) throw std::invalid_argument("couldn't create killEvent");
    if (!InitializeCriticalSectionAndSpinCount(&killReasonLock, 0))
        throw std::invalid_argument("couldn't create killReasonLock");
};

Task::~Task()
{
    if (killEvent) CloseHandle(killEvent);
    DeleteCriticalSection(&killReasonLock);
};

void Task::sendKill(const std::string& reason)
{
    EnterCriticalSection(&killReasonLock);
    killReason = reason;
    io::DbgOutput("sendKill kill signal ('%s') ", killReason.c_str());
    isKilled = true;
    LeaveCriticalSection(&killReasonLock);
    if (killEvent) SetEvent(killEvent);
}

std::string Task::getKillReason()
{
    std::string out;
    EnterCriticalSection(&killReasonLock);
    out = killReason;
    LeaveCriticalSection(&killReasonLock);
    return out;
}

void Task::start(t_UpdateCb onUpdate, LPVOID lpParam)
{
    Job::start(onUpdate, lpParam);
    auto ret = main();
    if (ret.empty())
    {
        mState->setStatus(JobState::TerminatedWithSuccess);
    }
    else
    {
        mState->setStatus(JobState::TerminatedWithError, ret);
    }
    sendUpdate();
}
