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
        size_t size();
        void clear();
        std::vector<Job::t_Pointer>::iterator JobsScheduler::begin();
        std::vector<Job::t_Pointer>::iterator JobsScheduler::end();
        Job::t_Pointer get(int index);
        void start(t_UpdateCb onUpdate, LPVOID lpParam = NULL);

    private:
        static const int RESET = -1;
        int mJobIdx = RESET;
        bool isRunning() { return mJobIdx != RESET; }

        std::vector<Job::t_Pointer> mJobs;
        static void onJobUpdate(Arguments::t_Pointer pArg, LPVOID lpParam);
        void runNextJob();
};
