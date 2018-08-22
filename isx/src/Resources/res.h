#pragma once

#define IDS_INSTALL_DIALOGTITLE         101
#define IDS_UNINSTALL_DIALOGTITLE       102
#define IDS_CANCELBTN                   103
#define IDS_CANCELMSG                   104
#define IDS_ERROR                       105
#define IDS_MEMOTITLE                   106
#define IDS_TASKDELETE                  107
#define IDS_TASKDELETEERROR             108
#define IDS_TASKDWNL                    109
#define IDS_TASKDWNLERROR               110
#define IDS_TASKEXE                     111
#define IDS_TASKEXEERROR                112
#define IDS_TASKUNZIP                   113
#define IDS_TASKUNZIPERROR              114

namespace res
{
    const void setLang(const std::string& lang);
    const std::string getString(const int ID, ...);
}