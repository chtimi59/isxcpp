#include "common.h"
#include "UIEvent.h"

static HANDLE g_Event;
static CRITICAL_SECTION g_Lock;
static UIEvent::Payload g_Payload;

void UIEvent::Constructor() {
    if (!InitializeCriticalSectionAndSpinCount(&g_Lock, 0))
        io::ThrowError("couldn't create g_Lock");
    g_Event = CreateEvent(NULL, FALSE, FALSE, "UIEvent");
    if (g_Event == NULL)
        io::ThrowError("couldn't create g_Event");
}

void UIEvent::Destructor() {
    if (g_Event) CloseHandle(g_Event);
    DeleteCriticalSection(&g_Lock);
}

HANDLE UIEvent::Event() {
    return g_Event;
}

UIEvent::Payload UIEvent::GetCurrent() {
    EnterCriticalSection(&g_Lock);
    Payload out = g_Payload;
    LeaveCriticalSection(&g_Lock);
    return out;
}

void UIEvent::Send(Payload &param) {
    EnterCriticalSection(&g_Lock);
    g_Payload = param;
    LeaveCriticalSection(&g_Lock);
    if (g_Event) SetEvent(g_Event);
}

UIEvent::Payload& UIEvent::Payload::operator=(const Payload& other) {
    label1 = other.label1;
    label2 = other.label2;
    label3 = other.label3;
    progress1 = other.progress1;
    progress2 = other.progress2;
    result = other.result;
    return *this;
}
