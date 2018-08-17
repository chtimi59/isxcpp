#pragma once
//std
#include <string>
// system headers
#include <windows.h>

extern HINSTANCE hInst;

namespace ressources
{
    const std::string getString(const int id) {
        std::string out;
        char text[256];
        int ret = LoadString(hInst, id, text, sizeof(text) * sizeof(char));
        return std::string(text);
    }
}