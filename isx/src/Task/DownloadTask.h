#pragma once
#include "Task.h"

class DownloadTask : public Task
{
    public:
        DownloadTask(const char* url, const char* dest) :
            Task("DownloadTask"),
            url(url),
            dest(dest)
        {}

        void kill(const std::string& reason);

    private:
        const std::string url;
        const std::string dest;
        const std::string main();

    private:
        static int xferinfo(void *, long long , long long, long long, long long);
        static size_t write_data(void *, size_t, size_t, FILE *);
        void* lpvoid = NULL;
};
