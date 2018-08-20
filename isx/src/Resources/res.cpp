#include "common.h"
#include <stdarg.h>

namespace res
{
    const std::string getString(const int id, ...) {
        char szFormat[256];
		// TODO: use right ressources with "LANG"
        int ret = LoadString(HINST, id, szFormat, sizeof(szFormat) * sizeof(char));
		char szBuff[1024];
		va_list arg;
		va_start(arg, id);
		_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
		va_end(arg);
        return szBuff;
    }
}