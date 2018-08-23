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
    char currentDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDirectory);

    /* [lpApplicationName/lpCommandLine] *
    
    lpApplicationName is the application path 
    lpCommandLine can be see as the arguments (argc/argv) of the application
    example:

        lpApplicationName = "C:\Windows\System32\cmd.exe"
        lpCommandLine = "/c batchfile.bat"

    The lpApplicationName parameter can be NULL.
    In that case, the application must be the first white space–delimited token
    in the lpCommandLine string.
    example:

        lpApplicationName = NULL
        lpCommandLine = "notepad.exe mytextfile.txt"

    If you are using a long file name that contains a space, use quoted strings
    to indicate where the file name ends and the arguments begin

    Note:
    One major difference, I've notice is, despite lpCommandLine, lpApplicationName
    dont' resolve application path. Hence,

    THIS WON't WORK:

        lpApplicationName = "notepad.exe"
        lpCommandLine = NULL

    but this does:

        lpApplicationName = NULL
        lpCommandLine = "notepad.exe"
    */

    std::string cmd = command + " " + arguments; // white space–delimited token
    
    char* lpApplicationName = NULL;
    char* lpCommandLine = (LPSTR)cmd.c_str();

    /* [lpCurrentDirectory] *

    lpCurrentDirectory, defines the currentDirectory used under the process,
    example:

        lpCurrentDirectory = c:\MyDocuments

    That's say, unfortunatly lpCurrentDirectory is not used to resolve lpApplicationName,
    nor lpCommandLine, so to fix that we can change currentDirectory before calling CreateProcess()

    which make lpCurrentDirectory useless an be set to NULL

    */
    char* lpCurrentDirectory = NULL;

    if (io::DirectoryExists(workingDirectory)) {
        SetCurrentDirectory(workingDirectory.c_str());
    }

    /* [lpStartupInfo] *
    */
    STARTUPINFO lpStartupInfo;
    ZeroMemory(&lpStartupInfo, sizeof(lpStartupInfo));
    lpStartupInfo.cb = sizeof(lpStartupInfo);

    /* [lpProcessInformation] *
    */
    PROCESS_INFORMATION lpProcessInformation;
    ZeroMemory(&lpProcessInformation, sizeof(lpProcessInformation));

    if (!CreateProcess(
            lpApplicationName,
            lpCommandLine,
            NULL,               // (LPSECURITY_ATTRIBUTES) Process handle not inheritable
            NULL,               // (LPSECURITY_ATTRIBUTES) Thread handle not inheritable
            FALSE,              // Set handle inheritance to FALSE
            0,                  // No creation flags
            NULL,               // Use parent's environment block
            lpCurrentDirectory,
            &lpStartupInfo,
            &lpProcessInformation
        ))
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
    const HANDLE evtHandles[] = { lpProcessInformation.hProcess, killEvent };
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
                    TerminateProcess(lpProcessInformation.hProcess, 1);
                }
                break;
            }
        }
    } /* infinite loop */
    
    /* restore currentDirectory */
    SetCurrentDirectory(currentDirectory);

    // Get the exit code.
    GetExitCodeProcess(lpProcessInformation.hProcess, &exitCode);

    // Close process and thread handles. 
    CloseHandle(lpProcessInformation.hProcess);
    CloseHandle(lpProcessInformation.hThread);

    if (bWasKill) Job::kill(getKillReason());

    if (exitCode != EXIT_SUCCESS) {
        io::DbgOutput("process exit code %u", exitCode);
        return res::getString(IDS_TASKEXEERROR, command.c_str());
    }
    setProgress(100);
    sendUpdate();
    return SUCCESS;
}

