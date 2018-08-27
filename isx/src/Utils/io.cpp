#include "common.h"

// std
#include <windows.h>

namespace io
{
    void MsgBox(const std::string& txt, const std::string& caption) {
        if (!ISQUIET) {
            const std::string title = caption.empty() ? res::getString(ISINSTALL ? IDS_INSTALL_DIALOGTITLE : IDS_UNINSTALL_DIALOGTITLE) : caption;
            MessageBox(NULL, txt.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
        }
    }
}
