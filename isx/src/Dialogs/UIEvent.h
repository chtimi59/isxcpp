#pragma once

struct UIEvent
{
    struct Payload {
        std::string label1;
        std::string label2;
        std::string label3;
        int progress1 = 0;
        int progress2 = 0;
        std::string result;
        Payload& operator=(const Payload& b);
    };

    static void Constructor();
    static void Destructor();

    static Payload GetCurrent();

    static void Send(Payload &param);

    static HANDLE Event();
};
