#include "common.h"
#include "Job.h"
#include "Dialogs\UIEvent.h"

Job::Job(std::string title) {
    mState = std::make_shared<JobState>(title);
}

std::string Job::getName() {
    return mState->Title;
}

void Job::setProgress(DWORD val) {
    mState->Progress = val;
}

void Job::setTitle(const std::string& val) {
    mState->Title = val;
}

void Job::setSubTitle(const std::string& val) {
    mState->SubTitle = val;
}

void Job::sendUpdate() {
    if (!mOnUpdate) io::ThrowError("mOnUpdate is NULL!");
    mOnUpdate(mState, mlpParam);
}

void Job::start(t_UpdateCb onUpdate, LPVOID lpParam) {
    io::DbgOutput("Job Start [%s]", this->getName().c_str());
    mOnUpdate = onUpdate;
    mlpParam = lpParam;
    mState->setStatus(JobState::Running);
}

void Job::setRunThread(HANDLE hThread) {
    mhRunThread = hThread;
}

void Job::kill(const std::string& reason) {
    // send UI update
    auto ui = UIEvent::GetCurrent();
    ui.result = reason;
    UIEvent::Send(ui);
    // by default kill thread
    if (mhRunThread) TerminateThread(mhRunThread, 1);
}
