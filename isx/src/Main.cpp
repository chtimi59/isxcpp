#define INSTANCIATE_COMMON
#include "common.h"

// project headers
#include "Dialogs/Resources.h"
#include "Dialogs/Dialog1.h"
#include "Dialogs/UIEvent.h"
#include "Job/JobsScheduler.h"
#include "Task/DownloadTask.h"
#include "Task/ExecuteTask.h"
#include "Task/UnZipTask.h"
#include "Task/FakeTask.h"
#include "Task/DeleteTask.h"

// std
#include <memory>
#include <vector>
#include <string>

// system headers
#include <windows.h>

// Products List
static std::shared_ptr<JobsScheduler> pProducts = std::make_shared<JobsScheduler>("Products list");

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            HINST = hinstDLL;
            ISINSTALL = true;
            ISQUIET = false;
            NEW_LINE = "\n";
            char szTmp[MAX_PATH];
            GetModuleFileName(hinstDLL, szTmp, MAX_PATH);
            ROOTPATH = io::Dirname(szTmp);
            GetTempPath(MAX_PATH, szTmp);
            TMPPATH = io::PathCombine(szTmp, "isx");
            CreateDirectory(TMPPATH.c_str(), NULL);
            UIEvent::Constructor();
            break;

        case DLL_PROCESS_DETACH:
            UIEvent::Destructor();
            break;
    }    
    return true;
}

/**
* Initialize
*/
extern "C" void __stdcall Initialize(
    bool isInstall,
    bool isQuiet,
    const char* lang,
    const char* tmpPath
) {
    // InnoSetup consider '' as NULL
    if (!lang) lang = "";
    if (!tmpPath) tmpPath = "";

    ISINSTALL = isInstall;
    ISQUIET = isQuiet;
    res::setLang(lang);
    if (io::DirectoryExists(tmpPath)) TMPPATH = tmpPath;
    io::DbgOutput("temporary folder set to %s", TMPPATH.c_str());
}
    

/**
* Clear Product Entries
*/
extern "C" void __stdcall ClearProducts()
{
    pProducts->clear();
}

/**
* Create a Product Entry
*/
extern "C" int __stdcall CreateProduct(
    const char* name
) {
    // InnoSetup consider '' as NULL
    if (!name) name = "notitle";

    int idx = pProducts->size();
    pProducts->add(std::make_shared<Product>(name));
    return idx;
}

/**
* Add a Download Task to a product
*/
extern "C" void __stdcall AddDownloadTask(
    int productIndex,
    const char* url,
    const char* dest
) {
    // InnoSetup consider '' as NULL
    if (!url) url = "";
    if (!dest) dest = "";

    auto pJob = pProducts->get(productIndex);
    if (!pJob) return;
    auto pProd = std::dynamic_pointer_cast<JobsScheduler>(pJob);
    if (!pProd) return;
    auto task = std::make_shared<DownloadTask>(url, dest);
    pProd->add(std::dynamic_pointer_cast<Job>(task));
}

/**
* Add a Execute Task to a product
*/
extern "C" void __stdcall AddExecuteTask(
        int productIndex,
        const char* workingDirectory,
        const char* command,
        const char* arguments,
        bool cancelable
) {
    // InnoSetup consider '' as NULL
    if (!workingDirectory) workingDirectory = "";
    if (!command) command = "";
    if (!arguments) arguments = "";

    auto pJob = pProducts->get(productIndex);
    if (!pJob) return;
    auto pProd = std::dynamic_pointer_cast<JobsScheduler>(pJob);
    if (!pProd) return;
    auto task = std::make_shared<ExecuteTask>(workingDirectory, command, arguments, cancelable);
    pProd->add(std::dynamic_pointer_cast<Job>(task));
}

/**
* Add an UnZip Task to a product
*/
extern "C" void __stdcall AddUnZipTask(
    int productIndex,
    const char* path,
    const char* dst,
    bool clear
) {
    // InnoSetup consider '' as NULL
    if (!path) path = "";
    if (!dst) dst = "";

    auto pJob = pProducts->get(productIndex);
    if (!pJob) return;
    auto pProd = std::dynamic_pointer_cast<JobsScheduler>(pJob);
    if (!pProd) return;
    auto task = std::make_shared<UnZipTask>(path, dst, clear);
    pProd->add(std::dynamic_pointer_cast<Job>(task));
}

/**
* Add an Delete Task to a product
*/
extern "C" void __stdcall AddDeleteTask(
    int productIndex,
    const char* path,
    bool exitIfFail
) {
    // InnoSetup consider '' as NULL
    if (!path) path = "";

    auto pJob = pProducts->get(productIndex);
    if (!pJob) return;
    auto pProd = std::dynamic_pointer_cast<JobsScheduler>(pJob);
    if (!pProd) return;
    auto task = std::make_shared<DeleteTask>(path, exitIfFail);
    pProd->add(std::dynamic_pointer_cast<Job>(task));    
}

/**
* Add a Fake Task (it's only a timed progress bar) to a product
* NOTE: Used for test
*/
extern "C" void __stdcall AddFakeTask(
    int productIndex,
    const char* name
) {
    // InnoSetup consider '' as NULL
    if (!name) name = "notitle";

    auto pJob = pProducts->get(productIndex);
    if (!pJob) return;
    auto pProd = std::dynamic_pointer_cast<JobsScheduler>(pJob);
    if (!pProd) return;
    auto task = std::make_shared<FakeTask>(name);
    pProd->add(std::dynamic_pointer_cast<Job>(task));    
}

/**
* Return the InnoSetup Memo string which is a digest of all operation which will be performed
*/
extern "C" const char * __stdcall GetReadyMemo(
    const char* space,
    const char* newLine
) {
    // InnoSetup consider '' as NULL
    if (!space) space = "";
    if (!newLine) newLine = "";

    NEW_LINE = newLine;
    std::string ret = "";
    if (pProducts->size() > 0) ret = res::getString(IDS_MEMOTITLE) + NEW_LINE;
    for (auto it = pProducts->begin(); it != pProducts->end(); ++it)
    {
        ret += space + it->get()->getName() + NEW_LINE;
    }
    return heap::push(ret);
}

/**
* Do sequential all tasks associated to all products
*/
extern "C" const char * __stdcall Run(
    int hWnd,
    bool matchPrepareToInstallPage
) {
    if (pProducts->size() == 0) return SUCCESS;
    auto dialog1 = Dialog1((HWND)hWnd, matchPrepareToInstallPage, pProducts);
    auto result = dialog1.show();
    if (!ISINSTALL && result != SUCCESS) io::MsgBox(result);
    return heap::push(result);
}

/**
* Make current thread wait for a delay in millisecond
*/
extern "C" void __stdcall Wait(
    int ms
) {
    Sleep(ms);
}