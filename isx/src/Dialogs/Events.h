#pragma once
// std
#include <string>
#include <memory>
// system headers
#include "Windows.h"

struct Events
{
    class PString
    {
        public:
            LPARAM getLParam();
            void set(std::string &val);
            void set(const char* val);
            PString& operator=(const PString& b);
            bool operator!=(const PString& other) const;
            bool operator==(const PString& other) const;
            
        private:
            char buff[256] = {0};
            size_t size = 0;
    };

    struct UIParams {
        PString label1;
        PString label2;
        PString label3;
        int progress1 = 0;
        int progress2 = 0;
        std::string result;
        UIParams& operator=(const UIParams& b);
    };

    static void Constructor();
    static void Destructor();

    static UIParams GetCurrentUI();
    static void SendUIEvent(UIParams &param);
    static void SendCancelEvent();
    
    static HANDLE UserCancelEvent();
    static HANDLE UIEvent();
};
