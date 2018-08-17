#pragma once
#include "JobsScheduler.h"

void JobsScheduler::start(t_UpdateCb onUpdate, LPVOID lpParam) {
    if (isRunning()) throw std::invalid_argument("Scheduler has been already Started");
    Job::start(onUpdate, lpParam);
    mJobIdx = 0;
    runNextJob(); //kick off
}

void JobsScheduler::add(Job::t_Pointer pJob) {
    if (isRunning()) throw std::invalid_argument("Couldn't add job, as Scheduler has been Started");
    mJobs.push_back(pJob);
}

size_t JobsScheduler::size() {
    return mJobs.size();
}

void JobsScheduler::clear() {
    if (isRunning()) throw std::invalid_argument("Couldn't add job, as Scheduler has been Started");
    mJobs.clear();
}

std::vector<Job::t_Pointer>::iterator JobsScheduler::begin() {
    if (isRunning()) throw std::invalid_argument("Couldn't add job, as Scheduler has been Started");
    return mJobs.begin();
}

std::vector<Job::t_Pointer>::iterator JobsScheduler::end() {
    if (isRunning()) throw std::invalid_argument("Couldn't add job, as Scheduler has been Started");
    return mJobs.end();
}

Job::t_Pointer JobsScheduler::get(int index) {
    if (isRunning()) throw std::invalid_argument("Couldn't add job, as Scheduler has been Started");
    if (index < 0) return NULL;
    if (index >= mJobs.size()) return NULL;
    return mJobs[index];
}

void JobsScheduler::onJobUpdate(Arguments::t_Pointer pArg, LPVOID lpParam) {
    auto ctx = (JobsScheduler*)(lpParam);
    if (!ctx->isRunning()) return;

    ctx->mPArg->Child = pArg;
    if (pArg->isError()) {
        ctx->mPArg->setStatus(Arguments::TerminatedWithError, pArg->getResult());
        ctx->mJobIdx = RESET;
        // error !
        ctx->sendUpdate();
        return;
    }
    if (pArg->isTerminated()) {
        // keep going
        ctx->runNextJob();
    } else {
        // update UI
        ctx->sendUpdate();
    }
}

void JobsScheduler::runNextJob() {
    if (!isRunning()) return;
    mPArg->Progress = (DWORD)(100.0f * (float)mJobIdx / (float)mJobs.size());
    if ((size_t)(mJobIdx) < mJobs.size())
    {
        // keep going
        mJobs[mJobIdx++]->start(onJobUpdate, this);
    }
    else
    {
        // success !
        mPArg->setStatus(Arguments::TerminatedWithSuccess);
        mJobIdx = RESET;
    }
    sendUpdate();
}
