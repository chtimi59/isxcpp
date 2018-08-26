#include "common.h"
#include <stdarg.h>
#include <map>

class BankStrings
{
private:
    typedef std::map<size_t, std::string> tBank;
    typedef std::map<std::string, tBank> tBanksMap;

public:
    BankStrings()
    {
        #include "Lang\en.dat"
        #include "Lang\fr.dat"

        auto it = RES.find("default");
        if (it == RES.end()) io::ThrowError("default bank missing");
        mDefaultBank = it->second;
        mBank = mDefaultBank;
    }

    const void setBank(const std::string& key) {
        auto it = RES.find(key);
        if (it == RES.end())
        {
            io::DbgOutput("lang '%s' unfound use 'default' instead", key.c_str());
            mBank = mDefaultBank;
        }
        else
        {
            io::DbgOutput("lang '%s'", key.c_str());
            mBank = it->second;
        }
    }

    const std::string operator[](size_t index) const {
        auto it = mBank.find(index);
        if (it == mBank.end())
        {
            it = mDefaultBank.find(index);
            if (it == mDefaultBank.end())
                return "? missing RES[\"default\"][" + std::to_string(index) + "] ?";
        }
        return it->second;
    }

private:
    tBanksMap RES;
    tBank mDefaultBank;
    tBank mBank;
};


namespace res
{
    static BankStrings languageStrings;

    const void setLang(const std::string& lang) {
        languageStrings.setBank(lang);
    }

    const std::string getString(const int id, ...)
    {
        auto szFormat = languageStrings[id];
        char szBuff[1024];
        va_list arg;
        va_start(arg, id);
        _vsnprintf_s(szBuff, sizeof(szBuff), szFormat.c_str(), arg);
        va_end(arg);
        return szBuff;
    }
}