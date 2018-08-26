#pragma once
#include <assert.h>

namespace heap
{
    const LPVOID push(const LPVOID buff, const size_t size);
    const char* push(std::string);
    void release(const LPVOID buff);
    void release();
}

namespace io
{
    inline void ThrowError(const char* errormsg) {
        fprintf(stderr, "fatal error '%s'\n", errormsg);
        assert(FALSE);
        exit(1);
    }

    void DbgOutput(const char* szFormat, ...);
    void MsgBox(const std::string& txt, const std::string& caption = "");
    void DbgPopLastError();

    const std::string LTrim(const std::string& str);
    const std::string RTrim(const std::string& str, bool bRemovePS = false);
    std::string Basename(const std::string& path);
    std::string Dirname(const std::string& path);
    std::string PathCombine(const std::string& path1, const std::string& path2);
    std::string PathUnix2Win(const std::string& path);
    std::string PathWin2Unix(const std::string& path);
    std::string PathAbsolute(const std::string& path);
    void DirectoryCreate(const std::string& path);
    bool DirectoryExists(const std::string& path);
    bool DirectoryDelete(const std::string& path, UINT maxRetries = 10, UINT millisecondsDelay = 30);
}
