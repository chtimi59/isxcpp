#pragma once
#include <assert.h>
#include <string>
#include <windows.h>

namespace io
{
    inline void ThrowError(const char* errormsg) {
        fprintf(stderr, "fatal error '%s'\n", errormsg);
        assert(FALSE);
        exit(1);
    }

    void DbgOutput(const char* szFormat, ...);
    void DbgPopLastError();

    const std::string SPrintf(const std::string szFormat, ...);
    const std::string StrFormatByteSize(LONGLONG qdw);

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