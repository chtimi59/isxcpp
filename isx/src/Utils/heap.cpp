#include "common.h"

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
            ChunkCode(TMethodPointer method)
            {
                SYSTEM_INFO system_info;
                GetSystemInfo(&system_info);
                auto const page_size = system_info.dwPageSize;

                // prepare the memory in which the machine code will be put (it's not executable yet):
                _mem = VirtualAlloc(nullptr, page_size, MEM_COMMIT, PAGE_READWRITE);

                // copy the machine code into that memory:
                unsigned char DATA[] = {
                    0x58, /* pop eax */
                    0x50, /* push eax */

                    0xb8, /* mov eax */
                    0xCC, 0xCC, 0xCC, 0xCC, // {pParam1}

                    0x68, /* push  */
                    0xCC, 0xCC, 0xCC, 0xCC, // {pParam2}

                    0xc3, /* ret */
                };
                memcpy(_mem, DATA, sizeof(DATA));
                void ** pParam1 = (void**)(void*)(((int)_mem) + 3);
                void ** pParam2 = (void**)(void*)(((int)_mem) + 8);

                // Set Parameters Values
                *pParam1 = method.data;
                *pParam2 = method.code;

                // mark the memory as executable:
                DWORD dummy;
                VirtualProtect(_mem, sizeof(DATA), PAGE_EXECUTE_READ, &dummy);
            }
            ~ChunkCode()
            {
                VirtualFree(_mem, 0, MEM_RELEASE);
            }
    };
    static std::vector<std::shared_ptr<ChunkCode>> mMemoryCode;
    
    const LPVOID push(const TMethodPointer method)
    {
        auto chunk = std::make_shared<ChunkCode>(method);
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
