#include "common.h"
#include "DownloadTask.h"
#include <curl/curl.h>

void DownloadTask::kill(const std::string& reason) {
    sendKill(reason);
}

const std::string DownloadTask::main()
{
    setTitle(res::getString(IDS_TASKDWNL));
    setProgress(0);
    sendUpdate();

    CURL *curl = curl_easy_init();
    if (!curl) {
        io::DbgOutput("curl_easy_init() failed");
        return res::getString(IDS_TASKDWNLERROR, url.c_str());
    }
    lpvoid = curl;

    io::DirectoryCreate(io::Dirname(dest));

    FILE* fp;
    fopen_s(&fp, dest.c_str(), "wb");
    if (!fp) {
        io::DbgOutput("fopen_s() failed '%s'", dest.c_str());
        curl_easy_cleanup(curl);
        return res::getString(IDS_TASKDWNLERROR, url.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
    lpvoid = NULL;

    if (res != CURLE_OK)
    {
        io::DbgOutput(curl_easy_strerror(res));
        _unlink(dest.c_str());    
        if (res == CURLE_ABORTED_BY_CALLBACK) {
            Job::kill(getKillReason());
        } else {
            return res::getString(IDS_TASKDWNLERROR, url.c_str());
        }
    }

    return SUCCESS;
}

int DownloadTask::xferinfo(void *p,
    curl_off_t dltotal, curl_off_t dlnow,
    curl_off_t ultotal, curl_off_t ulnow)
{
    #if 1
    // Reduce Fps
    static DWORD lasttime = GetTickCount();
    DWORD diff = GetTickCount() - lasttime;
    if (diff < 100) return 0;
    lasttime = GetTickCount();
    #endif

    auto ctx = (DownloadTask*)p;
    if (dltotal != 0)
    {
        ctx->setProgress((DWORD)((100 * dlnow) / dltotal));

        double speed;
        CURL* curl = (CURL*)ctx->lpvoid;
        if (!curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed))
        {
            auto s1 = io::SPrintf("%.1f MB/sec", speed / 1024 / 1024);
            auto s2 = io::StrFormatByteSize((DWORD)dlnow);
            auto s3 = io::StrFormatByteSize((DWORD)dltotal);
            auto ret = io::SPrintf("%s - %s/%s", s1.c_str(), s2.c_str(), s3.c_str());
            ctx->setSubTitle(ret);
        }

        ctx->sendUpdate();

    }

    bool isKill = (WaitForSingleObject(ctx->killEvent, 0) == WAIT_OBJECT_0);
    return isKill ? 1 : 0;
}

size_t DownloadTask::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
