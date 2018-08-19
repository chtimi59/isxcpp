// project headers
#include "Resources/res.h"
#include "Dialogs/Resources.h"
#include "Dialogs/Dialog1.h"
#include "Dialogs/Events.h"
#include "Job/JobsScheduler.h"
#include "Task/DownloadTask.h"
#include "Task/ExecuteTask.h"
#include "Task/UnZipTask.h"
#include "Task/FakeTask.h"
#include "Task/DeleteTask.h"
#include "Utils/helpers.h"
#include "Utils/io.h"
// std
#include <memory>
#include <vector>
#include <string>
// system headers
#include <windows.h>

// Products List
static std::shared_ptr<JobsScheduler> pProducts = std::make_shared<JobsScheduler>("Products list");

// Is ISX is using during installation(setup) or uninstallation of InnoSetup
static bool ISINSTALL;

// Known as {tmp} this the temporary folder used by InnoSetup
static std::string TMPPATH;

// The folder where isx dlls and ressources currently are
static std::string ISXPATH;

// 2 letters lang format, like 'en', 'fr'
static std::string LANG = "en";

// How InnoSetup defined new line.
static std::string NEW_LINE = "\n";

HINSTANCE hInst = NULL;

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            hInst = hinstDLL;
            Events::Constructor();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            Events::Constructor();
            hInst = NULL;
            break;

    }    
    return true;
}

/**
* Initialize ISX
*/
extern "C" void Initialize(
    bool isInstall,
    const char* tmpPath,
    const char* isxPath,
    const char* lang
) {
    ISINSTALL = isInstall;
    TMPPATH = tmpPath;
    if (!io::DirectoryExists(TMPPATH)) {
        throw std::invalid_argument("'" + TMPPATH + "' Do not exists");
        return;
    }
    ISXPATH = isxPath;
    if (!io::DirectoryExists(ISXPATH)) {
        throw std::invalid_argument("'" + ISXPATH + "' Do not exists");
        return;
    }
    LANG = lang;
}

/**
* Clear Product Entries
*/
extern "C" void ClearProducts()
{
    pProducts->clear();
}

/**
* Create a Product Entry
*/
extern "C" int CreateProduct(
	const char* name
) {
    int idx = pProducts->size();
    pProducts->add(std::make_shared<Product>(name));
    return idx;
}

/**
* Add a Download Task to a product
*/
extern "C" void AddDownloadTask(
	int productIndex,
	const char* url,
	const char* dest
) {
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
extern "C" void AddExecuteTask(
		int productIndex,
		const char* workingDirectory,
		const char* command,
		const char* arguments
) {
	auto pJob = pProducts->get(productIndex);
	if (!pJob) return;
	auto pProd = std::dynamic_pointer_cast<JobsScheduler>(pJob);
	if (!pProd) return;
	auto task = std::make_shared<ExecuteTask>(workingDirectory, command, arguments);
	pProd->add(std::dynamic_pointer_cast<Job>(task));
}

/**
* Add an UnZip Task to a product
*/
extern "C" void AddUnZipTask(
	int productIndex,
	const char* path,
	const char* dst,
	bool clear
) {
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
extern "C" void AddDeleteTask(
	int productIndex,
	const char* path,
	bool exitIfFail
) {
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
extern "C" void AddFakeTask(
	int productIndex,
	const char* name
) {
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
extern "C" const char * GetReadyMemo(
	const char* space,
	const char* newLine
) {
    NEW_LINE = newLine;
    std::string ret = "";
    try
    {
        if (pProducts->size() > 0) ret = ressources::getString(IDS_MEMOTITLE) + NEW_LINE;
        for (auto it = pProducts->begin(); it != pProducts->end(); ++it)
        {
            ret += space + it->get()->getName() + NEW_LINE;
        }
    }
    catch (...)
    {
        /* silence error */
    }

    return helpers::HeapPush(ret);
}

/**
* Do sequential all tasks associated to all products
*/
extern "C" const char * Run(
	int hWnd,
	bool matchPrepareToInstallPage
) {
    if (pProducts->size() == 0) return Job::SUCCESS.c_str();
    auto dialog1 = Dialog1((HWND)hWnd, matchPrepareToInstallPage, pProducts);
    auto result = dialog1.show();
    return helpers::HeapPush(result);
}

/**
* Make current thread wait for a delay in millisecond
*/
extern "C" void Wait(
	int ms
) {
    Sleep(ms);
}