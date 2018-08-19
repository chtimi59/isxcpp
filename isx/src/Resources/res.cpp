#pragma once
//std
#include <string>
// system headers
#include <windows.h>
#include <stdarg.h>

extern HINSTANCE hInst;

namespace ressources
{
	/*
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);
	OutputDebugString(szBuff);
	*/
    const std::string getString(const int id, ...) {
        char szFormat[256];
        int ret = LoadString(hInst, id, szFormat, sizeof(szFormat) * sizeof(char));
		char szBuff[1024];
		va_list arg;
		va_start(arg, id);
		_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
		va_end(arg);
        return szBuff;
    }
}