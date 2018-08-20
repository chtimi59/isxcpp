#include "api.h"

#include <string>
#include <iostream>  
#include <iomanip> 

#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

#if 0
void testStdString() {
	char szBuff[256] = { 0 };

	strcpy(szBuff, "Hello World");
	std::string s1 = szBuff;

	strcpy(szBuff, "Youpie");
	std::string s2 = szBuff;

	std::string s3 = szBuff;

	printf("%s\n", s1.c_str());
	printf("%s\n", s2.c_str());

	s2 = std::string("Fuck!");

	printf("%s\n", s3.c_str());

	s1 = std::string("Fuck!");

	if (s1 == s2) printf("OK!\n");
	s1 = std::string("Fuck!!");
	if (s1 != s2) printf("OK!\n");
}

DWORD WINAPI MyThread2(LPVOID lpParam)
{
	int i = 0;
	while (TRUE) {
		Sleep(250);
		printf("MyThread2 00%u\n", i++);
	}
}

DWORD WINAPI MyThread1(LPVOID lpParam)
{
	const HANDLE hThread = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);
	int i = 0;
	while (TRUE) {
		Sleep(250);
		printf("MyThread1 %u\n", i++);
	}
}

void testthreadkill() {
	const HANDLE hThread = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	Sleep(5000);
	printf("****** kill MyThread1 **********\n");
	TerminateThread(hThread, 1);

	printf("****** process will close in 2s **********\n");
	Sleep(2000);
}
#endif

int main()
{
#if 0

#else
	if (!loadAPI()) return EXIT_FAILURE;
    Initialize(FALSE, FALSE, "fr", NULL);
	bool bRedo = true;
	while(bRedo)
	{
		int p1, p2;
		const char* result = NULL;

		ClearProducts();
		p1 = CreateProduct("My Product1");
		p2 = CreateProduct("My Product2");
		ClearProducts();
		p1 = CreateProduct("My Product1");
		p2 = CreateProduct("My Product2");
		assert(p1 == 0 && p2 == 1);
		printf(GetReadyMemo("...", "\n"));

		AddFakeTask(666, "Task fail");
		AddDeleteTask(p1, "C:\\dev\\isxcpp\\sdf\\\\\\\\      ", false);
		AddFakeTask(p1, "Task 1.2");
		AddExecuteTask(p2, "c:\\dev", "C:\\Program Files\\Microsoft VS Code\\Code.exe", "");
		AddFakeTask(p2, "Task 2.2");

		result = Run(0, false);
		printf("%s\n", result);

		bRedo = strcmp(result, "Operation canceled !") == 0;
		if (bRedo) {
			printf("Redo!");
			Wait(2000);
		}
	};

    printf("Success!");
#endif
	return EXIT_SUCCESS;
}

