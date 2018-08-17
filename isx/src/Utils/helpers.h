#pragma once
// system headers
#include "Windows.h"
//std
#include <string>

namespace helpers
{
    void DbgOutput(const char* szFormat, ...);
    const LPVOID HeapPush(const LPVOID buff, const size_t size);
    const char* HeapPush(std::string);
    void HeapRelease(const LPVOID buff);
    void HeapRelease();
}
