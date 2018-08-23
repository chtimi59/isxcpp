#include "common.h"
#include "ExecuteTask.h"

void ExecuteTask::kill(const std::string& reason) {
    sendKill(reason);
}

const std::string ExecuteTask::main()
{
    setTitle(res::getString(IDS_TASKEXE));
    sendUpdate();

    DWORD exitCode = EXIT_SUCCESS;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char currentDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDirectory);

    // Unfortunatly CreateProcess() don't use it's own lpCurrentDirectory to resolve lpApplicationName
    // so let's change currentDirectory before calling CreateProcess(0
    if (io::DirectoryExists(workingDirectory)) {
        SetCurrentDirectory(workingDirectory.c_str());
    }
    
    if (!CreateProcess(
            (LPSTR)command.c_str(),
            (LPSTR)arguments.c_str(),
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
        io::DbgOutput("CreateProcess failed, cmd:'%s', dir'%s'", command.c_str(), workingDirectory.c_str());
        SetCurrentDirectory(currentDirectory);
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
    
    /* restore currentDirectory */
    SetCurrentDirectory(currentDirectory);

    // Get the exit code.
    GetExitCodeProcess(pi.hProcess, &exitCode);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (bWasKill) Job::kill(getKillReason());

    if (exitCode != EXIT_SUCCESS) {
        io::DbgOutput("process exit code %u", exitCode);
        return res::getString(IDS_TASKEXEERROR, command.c_str());
    }
    setProgress(100);
    sendUpdate();
    return SUCCESS;
}

