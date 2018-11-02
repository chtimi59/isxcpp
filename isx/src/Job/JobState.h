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

    private:
        t_Status Status = Idle;
        std::string ErrorString;

    public:
        JobState() {};
        JobState(std::string title);
        int Index = 0;
        std::string Title = "";
        std::string SubTitle = "";
        DWORD Progress = 0;
        t_Pointer Child;

        bool isTerminated();
        bool isError();
        void setStatus(const t_Status &status, const std::string &error = SUCCESS);
        std::string getResult();
};