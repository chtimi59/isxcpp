/**
* Initialize
*/
extern "C" void __stdcall Initialize(
    bool isInstall,
    bool isQuiet,
    const char* lang,
    const char* tmpPath
);

/**
* Clear Product Entries
*/
extern "C" void __stdcall ClearProducts();

/**
* Create a Product Entry
*/
extern "C" int __stdcall CreateProduct(
    const char* name
);

/**
* Add a Download Task to a product
*/
extern "C" void __stdcall AddDownloadTask(
    int productIndex,
    const char* url,
    const char* dest
);

/**
* Add a Execute Task to a product
*/
extern "C" void __stdcall AddExecuteTask(
    int productIndex,
    const char* workingDirectory,
    const char* command,
    const char* arguments,
    bool cancelable
);

/**
* Add an UnZip Task to a product
*/
extern "C" void __stdcall AddUnZipTask(
    int productIndex,
    const char* path,
    const char* dst,
    bool clear
);

/**
* Add an Delete Task to a product
*/
extern "C" void __stdcall AddDeleteTask(
    int productIndex,
    const char* path,
    bool exitIfFail
);

/**
* Add a Fake Task (it's only a timed progress bar) to a product
* NOTE: Used for test
*/
extern "C" void __stdcall AddFakeTask(
    int productIndex,
    const char* name
);

/**
* Return the InnoSetup Memo string which is a digest of all operation which will be performed
*/
extern "C" const char * __stdcall GetReadyMemo(
    const char* space,
    const char* newLine
);

/**
* Do sequential all tasks associated to all products
*/
extern "C" const char * __stdcall Run(
    int hWnd,
    bool matchPrepareToInstallPage
);

/**
* Make current thread wait for a delay in millisecond
*/
extern "C" void __stdcall Wait(
    int ms
);