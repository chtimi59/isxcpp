#include "common.h"
#include "CBEvent.h"

static HANDLE g_Event;
static CRITICAL_SECTION g_Lock;
static CBEvent::Payload g_Payload;

void CBEvent::Constructor() {
    if (!InitializeCriticalSectionAndSpinCount(&g_Lock, 0))
        io::ThrowError("couldn't create g_Lock");
    g_Event = CreateEvent(NULL, FALSE, FALSE, "CBEvent");
    if (g_Event == NULL)
        io::ThrowError("couldn't create g_Event");
}

void CBEvent::Destructor() {
    if (g_Event) CloseHandle(g_Event);
    DeleteCriticalSection(&g_Lock);
}

HANDLE CBEvent::Event() {
    return g_Event;
}

CBEvent::Payload CBEvent::GetCurrent() {
    EnterCriticalSection(&g_Lock);
    Payload out = g_Payload;
    LeaveCriticalSection(&g_Lock);
    return out;
}

void CBEvent::Send(Payload &param) {
    EnterCriticalSection(&g_Lock);
    g_Payload = param;
    LeaveCriticalSection(&g_Lock);
    if (g_Event) SetEvent(g_Event);
}

bool CBEvent::Payload::operator==(const Payload& p)
{
    bool ret = true;
    ret = ret && (this->productIdx == p.productIdx);
    ret = ret && (this->taskIdx == p.taskIdx);
    ret = ret && (this->isError == p.isError);
    ret = ret && (this->isTerminated == p.isTerminated);
    return ret;
}
bool CBEvent::Payload::operator!=(const Payload& p)
{
    return !((*this) == p);
}