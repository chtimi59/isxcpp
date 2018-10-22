#include "common.h"

//std
#include <memory>
#include <map>

namespace heap
{
    class Chunk
    {
        public:
            LPVOID _mem = NULL;
            Chunk(const LPVOID buff, const size_t size)
            {
                if ((NULL == buff) || (0 == size)) return;
                _mem = malloc(size);
                memcpy(_mem, buff, size);
            }
            ~Chunk()
            {
                if (NULL != _mem) free(_mem);
            }
    };

    static std::map<unsigned int, std::unique_ptr<Chunk>> mHeap;
    static std::vector<nlohmann::basic_json<>> mHeapJson;
    
    const LPVOID push(const LPVOID buff, const size_t size)
    {
        mHeap[(unsigned int)buff] = std::make_unique<Chunk>(buff, size);
        return mHeap[(unsigned int)buff]->_mem;
    }

    const char* push(std::string str)
    {
        const char* buff = str.c_str();
        const size_t size = str.size() + 1;
        return (const char*)push((const LPVOID)buff, size);
    }

    const int push(nlohmann::basic_json<> json)
    {
        auto idx = mHeapJson.size();
        mHeapJson.push_back(json);
        return idx;
    }

    nlohmann::basic_json<>* json(const int idx)
    {
        try {
            return &mHeapJson[idx];
        }
        catch (...) {}
        return NULL;
    }

    void release(const LPVOID buff)
    {
        mHeap.erase((unsigned int)buff);
    }

    void release()
    {
        mHeap.clear();
    }
}
