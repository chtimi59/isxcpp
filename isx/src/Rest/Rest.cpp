#include "common.h"
#include "Rest.h"
#include <curl/curl.h>

namespace Rest
{
    struct MemoryStruct {
        char *memory;
        size_t size;
    };

    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        size_t realsize = size * nmemb;
        struct MemoryStruct *mem = (struct MemoryStruct *)userp;
        mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
        if (mem->memory == NULL) {
            io::DbgOutput("not enough memory (realloc returned NULL)");
            return 0;
        }
        memcpy(&(mem->memory[mem->size]), contents, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
        return realsize;
    }

    const std::string Rest::Get(const std::string& url)
    {
        struct MemoryStruct chunk;
        std::string ret = "";

        CURL *curl = curl_easy_init();
        if (!curl) {
            io::DbgOutput("curl_easy_init() failed");
            return ret;
        }

        /* CURLOPT_HTTPHEADER */
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        /* CURLOPT_USERAGENT */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "inno-setup/1.0");
        /* CURLOPT_URL */
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        /* CURLOPT_WRITEFUNCTION */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        /* CURLOPT_WRITEDATA */
        chunk.memory = (char*)malloc(1);
        chunk.size = 0;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            io::DbgOutput(curl_easy_strerror(res));
        }
        else {
            ret = chunk.memory;
        }
        free(chunk.memory);
        curl_easy_cleanup(curl);

        return ret;
    }
}
