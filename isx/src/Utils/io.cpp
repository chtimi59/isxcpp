#include "io.h"
//std
#include <string>
// system headers
#include <windows.h>

namespace io
{
    bool DirectoryExists(const std::string& dirName_in) {
        DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES)
            return false;  //something is wrong with your path!

        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
            return true;   // this is a directory!

        return false;    // this is not a directory!
    }
}
