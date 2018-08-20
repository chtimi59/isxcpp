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
        void kill(const std::string& reason);

    private:
        int mNextJobIdx = 0; /* int is atomic R/W no thread protection needed */
        std::vector<Job::t_Pointer> mJobs;

    private:
        int jobIdx();
        bool isRunning();
        void runNextJob();

        static void onJobUpdate(Arguments::t_Pointer pArg, LPVOID lpParam);
};

// Alias
typedef JobsScheduler Product;