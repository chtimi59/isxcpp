#pragma once
#include <assert.h>
#include <io.h>

#include <vector>
#include <nlohmann/json.hpp>

/*  In InnoSetup "@Callback" are known as Method Pointer
    referenced in docs as "procedure of object"
    They bascially contains 2 pointers, one for the Method itself, and the other for the context datas
    http://docwiki.embarcadero.com/RADStudio/Tokyo/en/Procedural_Types_(Delphi)#Method_Pointers
*/
typedef struct {
    void* code;
    void* data;
} TMethodPointer;

namespace heap
{
    const LPVOID push(const LPVOID buff, const size_t size);
    const char* push(std::string);
    const LPVOID push(const TMethodPointer method, int paramcount);
    const int push(nlohmann::basic_json<> json);
    nlohmann::basic_json<>* json(const int idx);
}

namespace io
{
    void MsgBox(const std::string& txt, const std::string& caption = "");
}

namespace misc
{
    std::vector<std::string> split(const std::string &s, char delim);

    /**
        returns pointer to JSON item or NULL if not found.
        path will be modified to point a valid path
        ex: "a.b.c.d" will becomes "a.b" if c do not exist
    */
    nlohmann::basic_json<>* JsonTravel(nlohmann::basic_json<>* j, std::string &path);
}