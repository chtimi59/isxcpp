#include "common.h"
#include "JobsScheduler.h"

void JobsScheduler::start(t_UpdateCb onUpdate, LPVOID lpParam) {
    Job::start(onUpdate, lpParam);
    mNextJobIdx = 0;
    runNextJob(); //kick off
}

void JobsScheduler::kill(const std::string& reason) {
    /* -- int is atomic R/W no thread protection needed -- */
    volatile int idx = mNextJobIdx;
    /* -- */

    idx--;
    if (idx < 0) {
        // if couldn't rely on childs jobs
        // then use default behavior instead
        Job::kill(reason);
    }
    else
    {
        mJobs[idx]->kill(reason);
    }
}

void JobsScheduler::add(Job::t_Pointer pJob) {
    mJobs.push_back(pJob);
}

size_t JobsScheduler::size() {
    return mJobs.size();
}

bool JobsScheduler::isRunning() {
    return jobIdx() >= 0;
}

int JobsScheduler::jobIdx() {
    return mNextJobIdx - 1;
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
    float base = (float)(ctx->jobIdx());
    float subTask = (float)pArg->Progress / 100.0f;
    ctx->mPArg->Progress = (DWORD)((100.0f * (base + subTask)) / count);

    if (pArg->isError()) {
        ctx->mPArg->setStatus(Arguments::TerminatedWithError, pArg->getResult());
        ctx->mNextJobIdx = 0;
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
    mPArg->Progress = (DWORD)(100.0f * (float)mNextJobIdx / (float)mJobs.size());
    if ((size_t)(mNextJobIdx) < mJobs.size())
    {
        // keep going
        int idx = mNextJobIdx;
        mNextJobIdx++;
        // start Job
        mJobs[idx]->setRunThread(mhThread);
        mJobs[idx]->start(onJobUpdate, this);
    }
    else
    {
        // success !
        mPArg->setStatus(Arguments::TerminatedWithSuccess);
        mNextJobIdx = 0;
    }
    sendUpdate();
}
