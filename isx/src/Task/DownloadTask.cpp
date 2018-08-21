#include "common.h"
#include "DownloadTask.h"
#include <curl/curl.h>

struct myprogress {
    CURL *curl;
    DownloadTask* ctx;
};

/* this is how the CURLOPT_XFERINFOFUNCTION callback works */
static int xferinfo(void *p,
    curl_off_t dltotal, curl_off_t dlnow,
    curl_off_t ultotal, curl_off_t ulnow)
{
    struct myprogress *myp = (struct myprogress *)p;
    CURL *curl = myp->curl;
    DownloadTask* ctx = myp->ctx;
    if (dltotal != 0) {
        DWORD progress = (DWORD)((100 * dlnow) / dltotal);
        fprintf(stderr, "%u%%\r\n", progress);
        ctx->setProgress(progress);
        ctx->sendUpdate();
        //if (progress > 50) return 1;
    }
    return 0;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

const std::string DownloadTask::main()
{
    setTitle(res::getString(IDS_TASKDWNL));
    setProgress(0);
    sendUpdate();

    CURL *curl = curl_easy_init();
    if (!curl) return res::getString(IDS_TASKDWNLERROR, url.c_str());

    io::DirectoryCreate(io::Dirname(dest));

    FILE* fp;
    fopen_s(&fp, dest.c_str(), "wb");
    if (!fp) {
        curl_easy_cleanup(curl);
        return res::getString(IDS_TASKDWNLERROR, url.c_str());
    }

    struct myprogress prog;
    prog.curl = curl;
    prog.ctx - this;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);

    if (res != CURLE_OK) {
        _unlink(dest.c_str());
        return res::getString(IDS_TASKDWNLERROR, url.c_str());
    }

    return SUCCESS;
}

