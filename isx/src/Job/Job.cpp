#include "Job.h"

const std::string Job::SUCCESS = "";

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
    if (Status != TerminatedWithError) return Job::SUCCESS;
    if (ErrorString.empty()) return "Unknown error occurs";
    return ErrorString;
}

Job::Job(std::string title) {
    mPArg = std::make_shared<Arguments>(title);
}

std::string Job::getName() {
    return mPArg->Title;
}

void Job::start(t_UpdateCb onUpdate, LPVOID lpParam) {
    mOnUpdate = onUpdate;
    mlpParam = lpParam;
    mPArg->setStatus(Arguments::Running);
}

void Job::sendUpdate() {
    if (mOnUpdate == NULL) throw std::invalid_argument("mOnUpdate is NULL!");
    mOnUpdate(mPArg, mlpParam);
}
