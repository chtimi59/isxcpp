#define INSTANCIATE_COMMON
#include "common.h"

// project headers
#include "Dialogs/Resources.h"
#include "Dialogs/Dialog1.h"
#include "Dialogs/UIEvent.h"
#include "Dialogs/CBEvent.h"
#include "Job/JobsScheduler.h"
#include "Task/DownloadTask.h"
#include "Task/ExecuteTask.h"
#include "Task/UnZipTask.h"
#include "Task/FakeTask.h"
#include "Task/DeleteTask.h"
#include "Http/Http.h"

// std
#include <memory>
#include <vector>
#include <string>

// system headers
#include <windows.h>

// https://github.com/nlohmann/json
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <semver.h>

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
            CBEvent::Constructor();
            break;

        case DLL_PROCESS_DETACH:
            UIEvent::Destructor();
            CBEvent::Constructor();
            break;
    }
    return true;
}

/**
* Trick to allows forbidden casting in Pascal
*/
extern "C" void* __stdcall PassThrought(void* ptr)
{
    return ptr;
}

/**
* Trick to allows to use Pascal Callbacks
*/
extern "C" int __stdcall WrapCallback(TMethodPointer method, int paramCount)
{
    return (int)(heap::push(method, paramCount));
}
extern "C" int __stdcall TestCallback(void* callback)
{
    return ((int(__stdcall *)(int, int, int, int, int, int))(callback))(11, 22, 33, 44, 55, 66);
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
    bool matchPrepareToInstallPage,
    TaskDoneCallBack cb
) {
    if (pProducts->size() == 0) return SUCCESS;
    auto dialog1 = Dialog1((HWND)hWnd, matchPrepareToInstallPage, cb, pProducts);
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

/**
* Returns BODY from GET
*/
extern "C" const char * __stdcall HttpGet(
    const char* url,
    int* pCode
) {
    if (!url) url = "";
    long ResponseCode = 0;
    auto ret = Http::Get(url, &ResponseCode);
    if (pCode) *pCode = ResponseCode;
    return heap::push(ret);
}

/**
* Returns BODY from POST
*/
extern "C" const char * __stdcall HttpPost(
    const char* url,
    const char* body,
    const char* contentType,
    int* pCode
) {
    if (!url) url = "";
    if (!body) body = "";
    if (!contentType) contentType = "";
    long ResponseCode = 0;
    auto ret = Http::Post(url, body, contentType, &ResponseCode);
    if (pCode) *pCode = ResponseCode;
    return heap::push(ret);
}


/**
* Create a JSON Handler from string
*/
extern "C" int __stdcall JsonParse(const char* data, int* hdl)
{
    if (!hdl) return 0;
    if (!data) return 0;
    try {
        *hdl = heap::push(json::parse(data));
        return 1;
    } catch(...) {
        return 0;
    }
}

/**
* Stringify a JSON handler
*/
extern "C" int __stdcall JsonStringify(const int hdl, const char** val)
{
    if (!val) return 0;
    try { 
        auto str = heap::json(hdl)->dump();
        *val = heap::push(str);
        return 1;
    } catch (...) {
        return 0;
    }
}

template<typename Fmt>
int JsonFmt(
    const int handle,
    const char* path,
    int isSet,
    Fmt pValue
) {
    if (!path) path = "";
    if (!pValue) return 0;
    std::string out = path ? std::string(path) : "";
    try {
        auto root = heap::json(handle);
        auto p = misc::JsonTravel(root, out);
        if (isSet) {
            if (p) {
                p->operator=(*pValue);
                return 1;
            } else {
                /* create key */
                auto prev = misc::JsonTravel(root, out);
                auto v = misc::split(path, '.');
                if (prev->is_array()) {
                    (*prev).push_back(*pValue);
                    return 1;
                }
                if (prev->is_object()) {
                    auto key = v.back();
                    (*prev)[key] = *pValue;
                    return 1;
                }
                return 0; // other type are invalid
            }
        } else {
            if (p) {
                *pValue = *p;
                return 1;
            } else {
                return 0;
            }
        }
    } catch (...) {
        return 0;
    }
}

/**
* Set or Get int from JSON handler
*/
extern "C" int __stdcall JsonInt(const int hdl, const char* path, int isSet, int* val)
{
    return JsonFmt(hdl, path, isSet, val);
}
/**
* Set or Get int from JSON handler
*/
extern "C" int __stdcall JsonIntFromIdx(const int hdl, int index, int isSet, int* val)
{
    char path[MAX_PATH] = { 0 };
    _itoa_s(index, path, MAX_PATH, 10);
    return JsonInt(hdl, path, isSet, val);
}

/**
* Set or Get float from JSON handler
*/
extern "C" int __stdcall JsonFloat(const int hdl, const char* path, int isSet, float* val)
{
    return JsonFmt(hdl, path, isSet, val);
}
/**
* Set or Get float from JSON handler
*/
extern "C" int __stdcall JsonFloatFromIdx(const int hdl, int index, int isSet, float* val)
{
    char path[MAX_PATH] = { 0 };
    _itoa_s(index, path, MAX_PATH, 10);
    return JsonFloat(hdl, path, isSet, val);
}

/**
* Set or Get string from JSON handler
*/
extern "C" int __stdcall JsonString(const int hdl, const char* path, int isSet, const char** val)
{
    std::string obj;
    if (!path) path = "";
    if (isSet && val) obj = std::string(*val);
    auto r = JsonFmt(hdl, path, isSet, &obj);
    if (!isSet && val) *val = heap::push(obj);
    return r;
}
/**
* Set or Get string from JSON handler
*/
extern "C" int __stdcall JsonStringFromIdx(const int hdl, const int index, int isSet, const char** val)
{
    char path[MAX_PATH] = {0};
    _itoa_s(index, path, MAX_PATH, 10);
    return JsonString(hdl, path, isSet, val);
}

/**
* Set or Get Object from JSON handler
* Note: Array are treated as Object where keys are indexes
*/
extern "C" int __stdcall JsonObj(const int hdl, const char* path, int isSet, int* val)
{
    if (!val) return 0;
    if (isSet) {
        auto p = heap::json(*val);
        auto r = JsonFmt(hdl, path, isSet, p);
        return r;
    } else {
        nlohmann::basic_json<> obj;
        auto r = JsonFmt(hdl, path, isSet, &obj);
        *val = heap::push(obj);
        return r;
    }
}
/**
* Set or Get Object from JSON handler
* Note: Array are treated as Object where keys are indexes
*/
extern "C" int __stdcall JsonObjFromIdx(const int hdl, int index, int isSet, int* val)
{
    char path[MAX_PATH] = { 0 };
    _itoa_s(index, path, MAX_PATH, 10);
    return JsonObj(hdl, path, isSet, val);
}
/**
* Get Json size
*/
extern "C" int __stdcall JsonSize(const int hdl)
{
    std::string out = "";
    auto p = misc::JsonTravel(heap::json(hdl), out);
    if (p) return p->size();
    return 0;
}


/**
* Compare 2 version
* return -3  if v1 is an invalid version
* return -2  if v2 is an invalid version
* return -1  if v1 is lower than v2
* return  0  if v1 is equal to v2
* return  >0 if v1 is higher then v2
*/
extern "C" int __stdcall VerCompare(const char* v1, const char* v2)
{
    if (!v1) v1 = "";
    if (!v2) v2 = "";
    semver_t sv1 = {};
    semver_t sv2 = {};
    if (semver_parse(v1, &sv1)) return -3;
    if (semver_parse(v2, &sv2)) return -2;
    int resolution = semver_compare(sv1, sv2);
    semver_free(&sv1);
    semver_free(&sv2);
    return resolution;
}
/**
* Semver Checks
* return 1 if version statisfy semver, 0 otherwise
*/
extern "C" int __stdcall VerSatisfy(const char* semver, const char* version)
{
    if (!semver) semver = "";
    if (!version) version = "";
    semver_t ssemver = {};
    semver_t sversion = {};
    char op[3];
    if (semver_parse_op(semver, &ssemver, &op)) return 0;
    if (semver_parse(version, &sversion)) return 0;
    int resolution = semver_satisfies(sversion, ssemver, op);
    semver_free(&sversion);
    semver_free(&ssemver);
    return resolution;
}
/**
* Format Version String
*/
extern "C" const char * __stdcall VerFormat(const char* ver, int pad)
{
    if (!ver) ver = "";
    if (pad <= 0) return "";
    if (pad > 16) return "";
    char fmt[MAX_PATH];
    
    const size_t len = (pad * 3/*maj,minor,patch*/ + 2 /*dots*/ + 1 /*nullstring*/) * sizeof(char);
    semver_t s = {};
    if (semver_parse(ver, &s)) return "";
    sprintf_s(fmt, MAX_PATH, "%%0%ud.%%0%ud.%%0%ud", pad, pad, pad);
    
    char* buf = (char*)malloc(len+1); /* +1 to allow snprintf truncation detection */
    snprintf((char*)buf, len+1, fmt, s.major, s.minor, s.patch);
    size_t c = (strlen(buf) + 1) * sizeof(char);
    if (c != len) buf[0] = '\0';
    auto ret = heap::push(buf, len+1);
    free(buf);
    return (const char *)ret;
}