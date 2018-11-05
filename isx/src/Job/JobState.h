#pragma once
// std
#include <string>
#include <memory>
// system headers
#include "windows.h"

class JobState
{
    public:
        typedef std::shared_ptr<JobState> t_Pointer;

    public:
        typedef enum {
            Idle = 0,
            Running,
            TerminatedWithSuccess,
            TerminatedWithError
        } t_Status;

    public:
        JobState() {};
        JobState(std::string title);

    private:
        t_Status Status = Idle;
        std::string ErrorString;
    public:
        bool isTerminated();
        bool isError();
        void setStatus(const t_Status &status, const std::string &error = SUCCESS);
        std::string getResult();

    public:
        /* if True, it means this Job won't affects
            1- JobScheduler's progress count
            2- or even potential UI
        */
        bool IsEmptyTask = false;

        /* 
            Job Index if there are part of a JobScheduler
        */
        int Index = 0;

        /* 
            Various Info (used by UI)
        */
        std::string Title = "";
        std::string SubTitle = "";
        DWORD Progress = 0;

        /*
            Job potentials children (Job also)
        */
        t_Pointer Child;


};