#include "common.h"
#include "asmX86Intel.hxx"

//std
#include <memory>
#include <map>

namespace heap
{
    // ----------------------
    // MEMORY DATA
    // ----------------------
    class ChunkData
    {
    public:
        LPVOID _mem = NULL;
        ChunkData(const LPVOID buff, const size_t size)
        {
            if ((NULL == buff) || (0 == size)) return;
            _mem = malloc(size);
            memcpy(_mem, buff, size);
        }
        ~ChunkData()
        {
            if (NULL != _mem) free(_mem);
        }
    };
    static std::map<unsigned int, std::unique_ptr<ChunkData>> mMemoryData;

    const LPVOID push(const LPVOID buff, const size_t size)
    {
        mMemoryData[(unsigned int)buff] = std::make_unique<ChunkData>(buff, size);
        return mMemoryData[(unsigned int)buff]->_mem;
    }

    const char* push(std::string str)
    {
        const char* buff = str.c_str();
        const size_t size = str.size() + 1;
        return (const char*)push((const LPVOID)buff, size);
    }


    // ----------------------
    // MEMORY CODE
    // ----------------------
    class ChunkCode
    {
    public:
        LPVOID _mem = NULL;
        ChunkCode(TMethodPointer method, int parmCount)
        {
            Bytes code = Win32ToDelphiCallCode(method, parmCount);
            SYSTEM_INFO system_info;
            GetSystemInfo(&system_info);
            auto const page_size = system_info.dwPageSize;
            assert(code.size() < page_size);
            _mem = VirtualAlloc(nullptr, page_size, MEM_COMMIT, PAGE_READWRITE);
            code.ApplyToBuff(_mem);
            // mark the memory as executable:
            DWORD dummy;
            VirtualProtect(_mem, code.size(), PAGE_EXECUTE_READ, &dummy);
        }
        ~ChunkCode()
        {
            VirtualFree(_mem, 0, MEM_RELEASE);
        }
    };
    static std::vector<std::shared_ptr<ChunkCode>> mMemoryCode;

    const LPVOID push(const TMethodPointer method, int paramCount)
    {
        auto chunk = std::make_shared<ChunkCode>(method, paramCount);
        mMemoryCode.push_back(chunk);
        return chunk->_mem;
    }



    // ----------------------
    // JSON Storage
    // ----------------------

    // { JSON - storage } 
    static std::vector<nlohmann::basic_json<>> mHeapJson;

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

}
