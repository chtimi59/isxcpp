#pragma once
#include "Job/Job.h"

class Task : public Job
{
    public:
        Task(std::string title);
        ~Task();
        void start(t_UpdateCb onUpdate, LPVOID lpParam = NULL);

    protected:
        virtual const std::string main() = 0;

    protected:
        void sendKill(const std::string& reason);
        std::string getKillReason();
        bool isKilled = false;
        HANDLE killEvent = NULL;

    private:
        CRITICAL_SECTION killReasonLock;
        std::string killReason;
};
