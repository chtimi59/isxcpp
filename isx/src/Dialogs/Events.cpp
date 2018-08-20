#include "common.h"
#include "Events.h"

static HANDLE g_hUserCancelEvent;
static HANDLE g_hUserhUIEvent;
static CRITICAL_SECTION g_UILock;
static Events::UIParams g_UIParams;

void Events::Constructor() {
    if (!InitializeCriticalSectionAndSpinCount(&g_UILock, 0))
        throw std::invalid_argument("couldn't create g_UILock");
    g_hUserhUIEvent = CreateEvent(NULL, FALSE, FALSE, "UpdateUI");
    if (g_hUserhUIEvent == NULL)
        throw std::invalid_argument("couldn't create g_hUserhUIEvent");
    g_hUserCancelEvent = CreateEvent(NULL, FALSE, FALSE, "cancel");
        if (g_hUserCancelEvent == NULL) throw std::invalid_argument("couldn't create g_hUserCancelEvent");
}

HANDLE Events::UserCancelEvent() {
    return g_hUserCancelEvent;
}

HANDLE Events::UIEvent() {
    return g_hUserhUIEvent;
}

void Events::Destructor() {
    if (g_hUserCancelEvent) CloseHandle(g_hUserCancelEvent);
    if (g_hUserhUIEvent) CloseHandle(g_hUserhUIEvent);
    DeleteCriticalSection(&g_UILock);
}

Events::UIParams Events::GetCurrentUI() {
    EnterCriticalSection(&g_UILock);
    UIParams out = g_UIParams;
    LeaveCriticalSection(&g_UILock);
    return out;
}

void Events::SendUIEvent(UIParams &param) {
    EnterCriticalSection(&g_UILock);
    g_UIParams = param;
    LeaveCriticalSection(&g_UILock);
    if (g_hUserhUIEvent) SetEvent(g_hUserhUIEvent);
}

void Events::SendCancelEvent() {
    if (g_hUserCancelEvent) SetEvent(g_hUserCancelEvent);
}

Events::UIParams& Events::UIParams::operator=(const UIParams& other) {
    label1 = other.label1;
    label2 = other.label2;
    label3 = other.label3;
    progress1 = other.progress1;
    progress2 = other.progress2;
    result = other.result;
    return *this;
}
