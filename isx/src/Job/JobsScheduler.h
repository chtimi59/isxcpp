#pragma once
// project headers
#include "Job.h"
//std
#include <vector>

class JobsScheduler : public Job
{
    public:
        JobsScheduler(std::string title) : Job(title) {}

        void add(Job::t_Pointer pJob);
        void start(t_UpdateCb onUpdate, LPVOID lpParam = NULL);
        void kill(const std::string& reason);

        size_t size();
        void clear();
        std::vector<Job::t_Pointer>::iterator JobsScheduler::begin();
        std::vector<Job::t_Pointer>::iterator JobsScheduler::end();
        Job::t_Pointer get(int index);

    private:
        int mNextJobIdx = 0;
        std::vector<Job::t_Pointer> mJobs;

    private:
        int jobIdx();
        bool isRunning();
        void runNextJob();

        static void onJobUpdate(JobState::t_Pointer pJobState, LPVOID lpParam);
};

// Alias
typedef JobsScheduler Product;