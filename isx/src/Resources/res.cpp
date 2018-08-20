#include "common.h"
#include <stdarg.h>

namespace res
{
    const std::string getString(const int id, ...)
    {
        // TODO: use right ressources with "LANG"
        // note: LANG may be 'default'

        char szFormat[256];
        int ret = LoadString(HINST, id, szFormat, sizeof(szFormat) * sizeof(char));
        char szBuff[1024];
        va_list arg;
        va_start(arg, id);
        _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
        va_end(arg);
        return szBuff;
    }
}