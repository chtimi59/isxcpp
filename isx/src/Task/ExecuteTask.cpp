#include "common.h"
#include "ExecuteTask.h"

ExecuteTask::ExecuteTask(const char* workingDirectory, const char* command, const char* arguments, bool cancelable) :
    Task("ExecuteTask"),
    workingDirectory(workingDirectory),
    command(command),
    arguments(arguments),
    cancelable(cancelable)
{
    killEvent = CreateEvent(NULL, FALSE, FALSE, "ExecuteTaskKill");
    if (killEvent == NULL) throw std::invalid_argument("couldn't create killEvent");
    if (!InitializeCriticalSectionAndSpinCount(&killReasonLock, 0))
        throw std::invalid_argument("couldn't create killReasonLock");
};

ExecuteTask::~ExecuteTask()
{
    if (killEvent) CloseHandle(killEvent);
    DeleteCriticalSection(&killReasonLock);
};


void ExecuteTask::kill(const std::string& reason) {
    EnterCriticalSection(&killReasonLock);
    killReason = reason;
    LeaveCriticalSection(&killReasonLock);
    if (killEvent) SetEvent(killEvent);
}

const std::string ExecuteTask::main()
{
    std::string cmd = command + " " + arguments;
    DWORD exitCode = EXIT_SUCCESS;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL,     // No module name (use command line)
        (LPSTR)cmd.c_str(),
        NULL,           // (LPSECURITY_ATTRIBUTES) Process handle not inheritable
        NULL,           // (LPSECURITY_ATTRIBUTES) Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        (LPSTR)workingDirectory.c_str(),
        &si,            // Pointer to STARTUPINFO structure
        &pi)            // Pointer to PROCESS_INFORMATION structure
        )
    {
        io::DbgPopLastError();
        return res::getString(IDS_TASKEXEERROR, command.c_str());
    }

    /* say something to say that we're alive ! */
    setProgress(50);
    sendUpdate();

    // Wait until child process exits.
    bool bWasKill = false;
    const HANDLE evtHandles[] = { pi.hProcess, killEvent };
    const DWORD count = sizeof(evtHandles) / sizeof(HANDLE);
    bool bLoop = TRUE;
    while (bLoop)
    {
        DWORD dwStatus = MsgWaitForMultipleObjects(count, evtHandles, FALSE, INFINITE, 0);
        switch (dwStatus)
        {
            /* process end */
            case WAIT_OBJECT_0:
                bLoop = FALSE;
                break;

            /* kill process */
            case WAIT_OBJECT_0 + 1:
            {
                if (cancelable) {
                    bWasKill = true;
                    TerminateProcess(pi.hProcess, 1);
                }
                break;
            }
        }
    } /* infinite loop */

    // Get the exit code.
    GetExitCodeProcess(pi.hProcess, &exitCode);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (bWasKill)
    {
        std::string reason;
        EnterCriticalSection(&killReasonLock);
        reason = killReason;
        LeaveCriticalSection(&killReasonLock);
        Job::kill(reason);
    }

    if (exitCode != EXIT_SUCCESS)
        return res::getString(IDS_TASKEXEERROR, command.c_str());

    return SUCCESS;
}

