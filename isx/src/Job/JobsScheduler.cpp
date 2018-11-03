#include "common.h"
#include "JobsScheduler.h"

void JobsScheduler::start(t_UpdateCb onUpdate, LPVOID lpParam) {
    Job::start(onUpdate, lpParam);
    mNextJobIdx = 0;

    progressCurrent = 0;
    progressCount = 0;
    for (size_t i = 0; i<mJobs.size(); i++) {
        if (!(mJobs[i]->isEmptyTask())) progressCount++;
    }

    runNextJob(); //kick off
}

void JobsScheduler::kill(const std::string& reason) {
    /* -- int is atomic R/W no thread protection needed -- */
    volatile int idx = mNextJobIdx;

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
    mState->setStatus(JobState::Idle);
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

void JobsScheduler::onJobUpdate(JobState::t_Pointer pJobState, LPVOID lpParam) {
    auto ctx = (JobsScheduler*)(lpParam);
    if (!ctx->isRunning()) return;

    ctx->mState->Child = pJobState;

    /* Progress calculation don't takes empty Job in account */
    if ((ctx->jobIdx() >= 0) && (!ctx->mJobs[ctx->jobIdx()]->isEmptyTask())) {
        float count = (float)ctx->progressCount;
        float base = (float)(ctx->progressCurrent);
        float subTask = (float)pJobState->Progress / 100.0f;
        ctx->mState->Progress = (DWORD)((100.0f * (base + subTask)) / count);
    }

    if (pJobState->isError()) {
        ctx->mState->setStatus(JobState::TerminatedWithError, pJobState->getResult());
        ctx->mNextJobIdx = 0;
        // error !
        ctx->sendUpdate();
        return;
    }
    if (pJobState->isTerminated()) {
        // keep going
        ctx->runNextJob();
    } else {
        // update UI
        ctx->sendUpdate();
    }
}

void JobsScheduler::runNextJob() {
    /* Progress calculation don't takes empty Job in account */
    if ((jobIdx() >= 0) && (!mJobs[jobIdx()]->isEmptyTask())) progressCurrent++;
    mState->Progress = (DWORD)(100.0f * (float)progressCurrent / (float)(progressCount));

    if ((size_t)(mNextJobIdx) < mJobs.size())
    {
        // keep going
        int idx = mNextJobIdx;
        mState->Index = mNextJobIdx;
        mNextJobIdx++;
        // start Job
        mJobs[idx]->setRunThread(mhRunThread);
        mJobs[idx]->start(onJobUpdate, this);
    }
    else
    {
        // success !
        mState->setStatus(JobState::TerminatedWithSuccess);
        mState->Index = mNextJobIdx;
        mNextJobIdx = 0;
    }
    sendUpdate();
}
