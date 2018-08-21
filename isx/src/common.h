#pragma once

#include <string>
#include "Windows.h"

#include "Resources/res.h"
#include "Utils/utils.h"

#ifdef INSTANCIATE_COMMON
#define EXTERN
#else
#define EXTERN extern
#endif

#define DBG_SLOWDOWN

// InnoSetup define success as empty string
#define SUCCESS ""

// DLL instance
EXTERN HINSTANCE HINST;
// Is this DLL intialized for an installation(setup) or uninstallation
EXTERN bool ISINSTALL;
// 2 letters lang format, like 'en', 'fr'
EXTERN std::string LANG;
// Allow windows MessageBox if true
EXTERN bool ISQUIET;
// How InnoSetup defines new lines
EXTERN std::string NEW_LINE;
// The folder where dlls and ressources are
EXTERN std::string ROOTPATH;
// Known as {tmp} constant in InnoSetup
// http://www.jrsoftware.org/ishelp/index.php?topic=consts
EXTERN std::string TMPPATH;

// For string manipulation
EXTERN TCHAR szTmp[MAX_PATH];
