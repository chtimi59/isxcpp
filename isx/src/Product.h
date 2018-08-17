#pragma once
// project headers
#include "Job/Job.h"
#include "Job/JobsScheduler.h"
//std
#include <vector>
#include <memory>

class Product : public JobsScheduler
{
public:
    Product(std::string title) : JobsScheduler(title) { };
};
