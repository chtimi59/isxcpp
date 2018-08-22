#pragma once
#include "JobState.h"
// std
#include <string>
#include <memory>
// system headers
#include "windows.h"

class Job
{
    public:
        typedef std::shared_ptr<Job> t_Pointer;
        typedef void(*t_UpdateCb)(JobState::t_Pointer pJobState, LPVOID lpParam);

    public:
        Job(std::string title);
        std::string getName();
        virtual void start(t_UpdateCb onUpdate, LPVOID lpParam = NULL);
        virtual void kill(const std::string& reason);

    protected:
        JobState::t_Pointer mState;
        void setProgress(DWORD val);
        void setTitle(const std::string& val);
        void setSubTitle(const std::string& val);
        void sendUpdate();

    public:
        void setRunThread(HANDLE hThread);
    protected:
        HANDLE mhRunThread;
    private:
        LPVOID mlpParam = NULL;
        t_UpdateCb mOnUpdate = NULL;
};
