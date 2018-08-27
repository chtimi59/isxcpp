#pragma once
#include <assert.h>
#include <io.h>

namespace heap
{
    const LPVOID push(const LPVOID buff, const size_t size);
    const char* push(std::string);
    void release(const LPVOID buff);
    void release();
}

namespace io
{
    void MsgBox(const std::string& txt, const std::string& caption = "");
}
