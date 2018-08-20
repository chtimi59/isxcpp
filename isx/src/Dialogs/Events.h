#pragma once

struct Events
{
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
