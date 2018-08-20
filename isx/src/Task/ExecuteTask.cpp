#include "common.h"
#include "ExecuteTask.h"

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
		NULL/*(LPSTR)workingDirectory.c_str()*/,
		&si,            // Pointer to STARTUPINFO structure
		&pi)            // Pointer to PROCESS_INFORMATION structure
		)
	{
		io::DbgPopLastError();
		return res::getString(IDS_TASKEXEERROR, command.c_str());
	}

	setProgress(50);
	sendUpdate();

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Get the exit code.
	GetExitCodeProcess(pi.hProcess, &exitCode);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (exitCode != EXIT_SUCCESS)
		return res::getString(IDS_TASKEXEERROR, command.c_str());

	return SUCCESS;
}

