#include "common.h"
#include "JobState.h"

JobState::JobState(std::string title) {
    Title = title;
};

bool JobState::isTerminated() {
    return Status != Running;
}

bool JobState::isError() {
    return Status == TerminatedWithError;
}

void JobState::setStatus(const t_Status &status, const std::string &error) {
    Status = status;
    ErrorString = error;
}

std::string JobState::getResult() {
    if (Status != TerminatedWithError) return SUCCESS;
    if (ErrorString.empty()) return "Unknown error occurs";
    return ErrorString;
}
