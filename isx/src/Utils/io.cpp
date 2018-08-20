#include "common.h"

// std
#include <memory>
#include <map>
#include <cctype>
#include <algorithm>
// system headers
#include <stdarg.h>

namespace io
{
	bool _directory_delete(const std::string& dirName_in);

    void MsgBox(const std::string& txt, const std::string& caption) {
        if (!ISQUIET) {
            const std::string title = caption.empty() ? res::getString(IDS_DIALOGTITLE) : caption;
            MessageBox(NULL, txt.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
        }
    }

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

    bool DirectoryExists(const std::string& dirName_in)
    {
        DWORD ftyp = GetFileAttributes(dirName_in.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES)
            return false;  //something is wrong with your path!
        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
            return true;   // this is a directory!
        return false;    // this is not a directory!
    }

    /**
    * History: Nowaday a lot of external process (like git, defender, watchers...)
    * will lock some files for a very short amount of time. Unfortunalty that's enought to
    * to time to time throw an Exception
    */
    bool DirectoryDelete(const std::string& directoryPath, UINT maxRetries, UINT millisecondsDelay)
    {
        for (UINT i = 0; i < maxRetries; ++i) {
            _directory_delete(directoryPath);
            if (!DirectoryExists(directoryPath)) return TRUE;
            Sleep(millisecondsDelay);
        }
        return FALSE;
    }

    // --- private helpers ---
    
    const std::string _trimPath(const std::string& dirName_in)
    {
        std::string str = dirName_in;
        if (str.empty()) return str;
        // Returns an iterator to the first element in the range[first, last)
        // for which pred returns true.
        // If no such element is found, the function returns last.
        //
        // note rbegin() and rend() return reverse operator
        //
        auto rstart = std::find_if(str.rbegin(), str.rend(),
            /* lambda fct */ [](int ch)
        {
            if (std::isspace(ch)) return false;
            if ('\\' == ch) return false;
            return true;
        }
        );
        // change reverse iterator to regular
        auto start = rstart.base();
        auto end = str.end();
        str.erase(start, end);
        // nothing to return
        if (str.empty()) return "";
        // bring back '\'
        return str + "\\";
    }

    bool _directory_delete(const std::string& dirName_in)
    {
        std::string path = _trimPath(dirName_in);
        if (path.empty()) return TRUE;
        if (!DirectoryExists(path)) return TRUE;

        HANDLE hFind;
        WIN32_FIND_DATA FindFileData;
        hFind = FindFirstFile((path + "*").c_str(), &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE) return FALSE;
        while (TRUE)
        {
            if (!FindNextFile(hFind, &FindFileData)) {
                if (GetLastError() == ERROR_NO_MORE_FILES) break;
                FindClose(hFind);
                return FALSE;
            }

            std::string item(FindFileData.cFileName);
            if (item == "." || item == "..") continue;

            std::string filename = path + item;

            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                /* item is a directory (recursive call) */
                if (!_directory_delete(filename)) {
                    FindClose(hFind);
                    return FALSE;
                }
            }
            else
            {
                /* item is a file */
                if (!DeleteFile(filename.c_str())) {
                    FindClose(hFind);
                    return FALSE;
                }
            }
        }
        FindClose(hFind);
        if (!RemoveDirectory(path.c_str())) {
            return FALSE;
        }
        return TRUE;
    }
}
