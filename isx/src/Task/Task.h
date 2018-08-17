#pragma once
// project headers
#include "Job/Job.h"

class Task : public Job
{
public:
    Task(std::string title) : Job(title) {};
    void start(t_UpdateCb onUpdate, LPVOID lpParam = NULL);
protected:
    virtual const std::string main() = 0;
    void setProgress(DWORD val);
    void setSubTitle(const std::string& val);
};
