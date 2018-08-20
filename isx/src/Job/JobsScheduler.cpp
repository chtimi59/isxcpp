#include "common.h"
#include "JobsScheduler.h"

void JobsScheduler::start(t_UpdateCb onUpdate, LPVOID lpParam) {
    Job::start(onUpdate, lpParam);
    mJobIdx = 0;
    runNextJob(); //kick off
}

void JobsScheduler::add(Job::t_Pointer pJob) {
    mJobs.push_back(pJob);
}

size_t JobsScheduler::size() {
    return mJobs.size();
}

void JobsScheduler::clear() {
    mPArg->setStatus(Job::Arguments::Idle);
    mJobs.clear();
}

std::vector<Job::t_Pointer>::iterator JobsScheduler::begin() {
    return mJobs.begin();
}

std::vector<Job::t_Pointer>::iterator JobsScheduler::end() {
    return mJobs.end();
}

Job::t_Pointer JobsScheduler::get(int index) {
    if (index < 0) return NULL;
    if (index >= (int)(mJobs.size())) return NULL;
    return mJobs[index];
}

void JobsScheduler::onJobUpdate(Arguments::t_Pointer pArg, LPVOID lpParam) {
    auto ctx = (JobsScheduler*)(lpParam);
    if (!ctx->isRunning()) return;

    ctx->mPArg->Child = pArg;

    float count = (float)ctx->mJobs.size();
    float base = (float)(ctx->mJobIdx - 1);
    float subTask = (float)pArg->Progress / 100.0f;
    ctx->mPArg->Progress = (DWORD)((100.0f * (base + subTask)) / count);

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
