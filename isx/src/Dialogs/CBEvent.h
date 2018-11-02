#pragma once

struct CBEvent
{
    struct Payload {
        int productIdx = 0;
        int taskIdx = 0;
        bool isTerminated = false;
        bool isError = false;
        bool operator==(const Payload& p);
        bool operator!=(const Payload& p);
    };

    static void Constructor();
    static void Destructor();

    static Payload GetCurrent();

    static void Send(Payload &param);

    static HANDLE Event();
};
