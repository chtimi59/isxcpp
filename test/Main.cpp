#include "api.h"
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <Shlwapi.h>

int main()
{
	if (!loadAPI()) return EXIT_FAILURE;

    Initialize(true, szTmpPath, szRootPath, "fr");

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
		AddFakeTask(p1, "Task 1.1");
		AddFakeTask(p1, "Task 1.2");
		AddFakeTask(p2, "Task 2.1");
		AddFakeTask(p2, "Task 2.2");

		result = Run(0, false);
		printf("%s\n", result);

		bRedo = strcmp(result, "Cancel") == 0;
		if (bRedo) {
			printf("Redo!");
			Wait(2000);
		}
	};

    printf("Success!");
    return EXIT_SUCCESS;
}
