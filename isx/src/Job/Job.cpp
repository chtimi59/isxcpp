#include "common.h"
#include "Job.h"
#include "Dialogs\UIEvent.h"

Job::Arguments::Arguments(std::string title) {
    Title = title;
};

bool Job::Arguments::isTerminated() {
    return Status != Running;
}

bool Job::Arguments::isError() {
    return Status == TerminatedWithError;
}

void Job::Arguments::setStatus(const t_Status &status, const std::string &error) {
    Status = status;
    ErrorString = error;
}

std::string Job::Arguments::getResult() {
    if (Status != TerminatedWithError) return SUCCESS;
    if (ErrorString.empty()) return "Unknown error occurs";
    return ErrorString;
}

Job::Job(std::string title) {
    mPArg = std::make_shared<Arguments>(title);
}

std::string Job::getName() {
    return mPArg->Title;
}

void Job::setRunThread(HANDLE hThread) {
    mhThread = hThread;
}

void Job::start(t_UpdateCb onUpdate, LPVOID lpParam) {
    io::DbgOutput("Job Start [%s]", this->getName().c_str());
    mOnUpdate = onUpdate;
    mlpParam = lpParam;
    mPArg->setStatus(Arguments::Running);
}

void Job::sendUpdate() {
    if (!mOnUpdate) throw std::invalid_argument("mOnUpdate is NULL!");
    mOnUpdate(mPArg, mlpParam);
}

void Job::kill(const std::string& reason) {
    // send UI update
    auto ui = UIEvent::GetCurrent();
    ui.result = reason;
    UIEvent::Send(ui);
    // by default kill thread
    if (mhThread) TerminateThread(mhThread, 1);
}
