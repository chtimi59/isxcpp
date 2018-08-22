#include "common.h"

// std
#include <memory>
#include <map>
#include <cctype>
#include <algorithm>
// system headers
#include <stdarg.h>
#include <assert.h>

#define PATH_SEPARATOR '\\'
#ifdef PathCombine
#undef PathCombine
#endif

namespace io
{
    void MsgBox(const std::string& txt, const std::string& caption) {
        if (!ISQUIET) {
            const std::string title = caption.empty() ? res::getString(ISINSTALL ? IDS_INSTALL_DIALOGTITLE : IDS_UNINSTALL_DIALOGTITLE) : caption;
            MessageBox(NULL, txt.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
        }
    }

    void DbgOutput(const char* szFormat, ...) {
#ifdef _DEBUG
        char szBuff[MAX_PATH];
        va_list arg;
        va_start(arg, szFormat);
        _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
        va_end(arg);
        strcat_s(szBuff, MAX_PATH, "\n");
        OutputDebugString("isx > ");
        OutputDebugString(szBuff);
#endif
    }

    void DbgPopLastError() {
#ifdef _DEBUG
        char buff[MAX_PATH];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buff, (sizeof(buff) / sizeof(wchar_t)), NULL);
        MessageBox(NULL, buff, "error", MB_OK);
#endif
    }

    /**
    LTrim("") == ""
    LTrim("aa") == "aa"
    LTrim("  \t \r \n  aa") == "aa"
    */
    const std::string LTrim(const std::string& str) {
        std::string out = str;
        auto pred = [](int ch) { return !std::isspace(ch); };
        auto end = std::find_if(out.begin(), out.end(), pred);
        out.erase(out.begin(), end);
        return out;
    }

    /**
    RTrim("") == ""
    RTrim("aa") == "aa"
    RTrim("aa  \t \r \n  ") == "aa"
    */
    const std::string RTrim(const std::string& str, bool bRemovePS) {
        std::string out = str;
        auto pred = [](int ch) { return !std::isspace(ch); };
        auto predWithPS = [](int ch) { return !(std::isspace(ch) || PATH_SEPARATOR == ch); };
        auto rstart = bRemovePS ? std::find_if(out.rbegin(), out.rend(), predWithPS)
            : std::find_if(out.rbegin(), out.rend(), pred);
        out.erase(rstart.base(), out.end());
        return out;
    }

    /**
    Basename("") == ""
    Basename("aa") == "aa"
    Basename("c:\\folder1\\folder2\\file.c") == "file1.c"
    Basename("c:\\folder1\\folder2\\") == "folder2"
    Basename("c:\\folder1\\folder2\\\\\\") == "folder2"
    Basename("c:\\") == "\"
    */
    std::string Basename(const std::string& path) {
        std::string out = RTrim(path, TRUE);
        std::size_t pos = out.find_last_of(PATH_SEPARATOR);
        out = out.substr(pos + 1);
        if (out.find_last_of(':') != std::string::npos) {
            const char root[2] = { PATH_SEPARATOR, '\0' };
            return root;
        }
        return out;
    }

    /**
    Dirname("") == ""
    Dirname("aa") == "."
    Dirname("c:\\folder1\\folder2\\file.c") == "file1.c"
    Dirname("c:\\folder1\\folder2\\") == "folder2"
    Dirname("c:\\folder1") == "c:"
    */
    std::string Dirname(const std::string& path) {
        std::string out = RTrim(path, TRUE);
        if (out.empty()) return out;
        std::size_t pos = out.find_last_of(PATH_SEPARATOR);
        if (pos == std::string::npos) {
            const char root[2] = { '.', '\0' };
            return root;
        }
        out = out.substr(0, pos);
        return out;
    }

    /**
    PathCombine("c:\\folder\\", "file1") == "c:\\folder\\file1");
    */
    std::string PathCombine(const std::string& path1, const std::string& path2) {
        std::string left = RTrim(path1, TRUE);
        std::string right = LTrim(path2);
        std::string out = left + PATH_SEPARATOR + right;
        return out;
    }

    /**
    PathUnix2Win("c:/folder/file1") == "c:\\folder\\file1"
    */
    std::string PathUnix2Win(const std::string& path) {
        std::string out = path;
        std::replace(out.begin(), out.end(), '/', '\\');
        return out;
    }

    /**
    PathWin2Unix("c:\\folder\\file1") == "c:/folder/file1"
    */
    std::string PathWin2Unix(const std::string& path) {
        std::string out = path;
        std::replace(out.begin(), out.end(), '\\', '/');
        return out;
    }

    /**
    PathWin2Unix("c:\\folder\\file1") == "c:/folder/file1"
    PathAbsolute(".") != currentDirector
    */
    std::string PathAbsolute(const std::string& path) {
        char sz[MAX_PATH] = { 0 };
        GetFullPathName(path.c_str(), MAX_PATH, sz, NULL);
        return sz;
    }

    /**
    Ensure that all needed folders and sub folder are created
    */
    void DirectoryCreate(const std::string& path) {
        std::string startWith = "";
        std::string str = PathAbsolute(path);
        while (!str.empty()) {
            auto pos = str.find_first_of(PATH_SEPARATOR);
            startWith += str.substr(0, pos) + PATH_SEPARATOR;
            CreateDirectory(startWith.c_str(), NULL);
            str = (pos != std::string::npos) ? str.substr(pos + 1) : "";
        }
    }

    /**
    Check that folder exists
    */
    bool DirectoryExists(const std::string& path) {
        std::string str = PathAbsolute(path);
        DWORD ftyp = GetFileAttributes(str.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES)
            return false;  //something is wrong with your path!
        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
            return true;   // this is a directory!
        return false;    // this is not a directory!
    }

    bool _directory_delete(const std::string& dirName_in);

    /**
    Recursively Delete a folder and its content

    History:
    Nowaday a lot of external process (like git, defender, watchers...)
    will lock some files for a very short amount of time. Unfortunalty that's enought to
    to time to time throw an Exception
    */
    bool DirectoryDelete(const std::string& path, UINT maxRetries, UINT millisecondsDelay)
    {
        for (UINT i = 0; i < maxRetries; ++i) {
            _directory_delete(path);
            if (!DirectoryExists(path)) return TRUE;
            Sleep(millisecondsDelay);
        }
        return FALSE;
    }

    static bool _directory_delete(const std::string& path)
    {
        if (path.empty()) return TRUE;
        if (!DirectoryExists(path)) return TRUE;

        HANDLE hFind;
        WIN32_FIND_DATA FindFileData;
        std::string search = PathCombine(path, "*");
        hFind = FindFirstFile(search.c_str(), &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE) return FALSE;

        while (TRUE)
        {
            /* end */
            if (!FindNextFile(hFind, &FindFileData)) {
                if (GetLastError() == ERROR_NO_MORE_FILES) break;
                FindClose(hFind);
                return FALSE;
            }

            /* skip '.' and '..' */
            std::string item(FindFileData.cFileName);
            if (item == "." || item == "..") continue;

            std::string filename = PathCombine(path, item);

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
                /* item is a file, delete it */
                if (!DeleteFile(filename.c_str())) {
                    FindClose(hFind);
                    return FALSE;
                }
            }
        }
        FindClose(hFind);

        /* actual folder deletion */
        if (!RemoveDirectory(path.c_str())) return FALSE;
        return TRUE;
    }

    static void _utest() {
        assert(LTrim("") == "");
        assert(LTrim("aa") == "aa");
        assert(LTrim("  \t \r \n  aa") == "aa");
        assert(RTrim("") == "");
        assert(RTrim("aa") == "aa");
        assert(RTrim("aa\\\\  \t \r \n  ") == "aa\\\\");
        assert(RTrim("aa\\\\  \t \r \n  ", TRUE) == "aa");
        assert(Basename("") == "");
        assert(Basename("aa") == "aa");
        assert(Basename("c:\\folder1\\folder2\\file.c") == "file.c");
        assert(Basename("c:\\folder1\\folder2\\") == "folder2");
        assert(Basename("c:\\folder1\\folder2\\\\\\") == "folder2");
        assert(Basename("c:\\") == "\\");
        assert(Dirname("") == "");
        assert(Dirname("aa") == ".");
        assert(Dirname("c:\\folder1\\folder2\\file.c") == "c:\\folder1\\folder2");
        assert(Dirname("c:\\folder1\\folder2\\") == "c:\\folder1");
        assert(Dirname("c:\\folder1\\") == "c:");
        assert(PathCombine("c:\\folder\\", "file1") == "c:\\folder\\file1");
        assert(PathCombine("  c:\\folder\\\\ ", "  file1  ") == "  c:\\folder\\file1  ");
        assert(PathCombine("  c:\\folder", "file1  ") == "  c:\\folder\\file1  ");
        assert(PathUnix2Win("c:/folder/file1") == "c:\\folder\\file1");
        assert(PathWin2Unix("c:\\folder\\file1") == "c:/folder/file1");
        assert(PathAbsolute("c:\\folder\\\\sub\\..\\file1") == "c:\\folder\\file1");
        assert(PathAbsolute(".") != "");
        DirectoryCreate(Dirname("hello\\coucou\\file1.txt"));
        assert(DirectoryExists("hello\\coucou\\"));
        assert(DirectoryExists(".\\hello\\coucou\\"));
        DirectoryCreate("hello2");
        assert(DirectoryExists("hello2"));
        DirectoryCreate("hello\\coucou\\folder");
        assert(DirectoryExists("hello\\coucou\\\\folder\\\\\\  "));
        DirectoryDelete("hello\\coucou\\folder\\\\\\  ", 10, 30);
        assert(DirectoryExists("hello\\coucou\\\\  "));
        DirectoryDelete("hello\\   ", 10, 30);
        assert(!DirectoryExists("hello\\"));
        DirectoryDelete("hello2\\    ", 10, 30);
        assert(!DirectoryExists("hello2\\"));
    }
}

#if 0

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

	void DirectoryCreate(const std::string& dirName_in)
	{	
		std::string startWith = "";
		std::string str = dirName_in;

		while(!str.empty()) {
			auto found = str.find_first_of('\\');
			startWith += str.substr(0, found) + '\\';
			CreateDirectory(startWith.c_str(), NULL);
			if (found != std::string::npos) {
				str = str.substr(found + 1);
			} else {
				str = "";
			}
		}
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
#endif