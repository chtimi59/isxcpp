#include "helpers.h"
//std
#include <string>
#include <memory>
#include <map>
// system headers
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

namespace helpers
{
    void DbgOutput(const char* szFormat, ...) {
        #ifdef _DEBUG
        char szBuff[1024];
        va_list arg;
        va_start(arg, szFormat);
        _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
        va_end(arg);
        OutputDebugString(szBuff);
        #endif
    }

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
    
    const LPVOID HeapPush(const LPVOID buff, const size_t size)
    {
        mHeap[(unsigned int)buff] = std::make_unique<Chunk>(buff, size);
        return mHeap[(unsigned int)buff]->_mem;
    }

    const char* HeapPush(std::string str)
    {
        const char* buff = str.c_str();
        const size_t size = str.size() + 1;
        return (const char*)HeapPush((const LPVOID)buff, size);
    }

    void HeapRelease(const LPVOID buff)
    {
        mHeap.erase((unsigned int)buff);
    }

    void HeapRelease()
    {
        mHeap.clear();
    }
}
