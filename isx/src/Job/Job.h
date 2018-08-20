#pragma once
// std
#include <string>
#include <memory>
// system headers
#include "windows.h"

class Job
{
    public:
        
        typedef std::shared_ptr<Job> t_Pointer;
        Job(std::string title);
        std::string getName();
        
        class Arguments
        {
            public:
                typedef std::shared_ptr<Arguments> t_Pointer;

                typedef enum {
                    Idle = 0,
                    Running,
                    TerminatedWithSuccess,
                    TerminatedWithError
                } t_Status;

                std::string Title = "";
                std::string SubTitle = "";
                DWORD Progress = 0;
                t_Pointer Child;

                Arguments() {};
                Arguments(std::string title);
                bool isTerminated();
                bool isError();
            
                void setStatus(const t_Status &status, const std::string &error = SUCCESS);
                std::string getResult();

            private:
                t_Status Status = Idle;
                std::string ErrorString;
        };

        typedef void(*t_UpdateCb)(Arguments::t_Pointer pArg, LPVOID lpParam);

        virtual void setRunThread(HANDLE hThread);
        virtual void start(t_UpdateCb onUpdate, LPVOID lpParam = NULL);
        virtual void kill(const std::string& reason);

    protected:
        HANDLE mhThread;
        Arguments::t_Pointer mPArg;
        void sendUpdate();

    private:
        LPVOID mlpParam = NULL;
        t_UpdateCb mOnUpdate = NULL;
};
