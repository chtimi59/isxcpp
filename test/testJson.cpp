#include "utils.h"

#include <string>
#include <iostream>
#include <iomanip>

#include <isx.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <cctype>
#include <algorithm>
#include <map>

void testjson()
{
    int root;
    assert(JsonParse("{", &root) == 0);
    assert(JsonParse("{}", &root) == 1);

    {
        int val = 5, ret = 0;
        assert(JsonInt(root, "number", false, &ret) == 0);
        assert(JsonInt(root, "number", true, &val) == 1);
        assert(JsonInt(root, "number", false, &ret) == 1);
        assert(val == ret);
    }

    {
        float val = 1.234567f, ret = 0.f;
        assert(JsonFloat(root, "float", false, &ret) == 0);
        assert(JsonFloat(root, "float", true, &val) == 1);
        assert(JsonFloat(root, "float", false, &ret) == 1);
        assert(val == ret);
    }

    {
        const char* val = "hello 1";
        const char * ret;
        assert(JsonString(root, "string", false, &ret) == 0);
        assert(JsonString(root, "string", true, &val) == 1);
        assert(JsonString(root, "string", false, &ret) == 1);
        assert(std::string(ret).compare(val) == 0);
    }

    {
        int val, ret;
        assert(JsonParse("{}", &val) == 1);
        assert(JsonObj(root, "myobj", false, &ret) == 0);
        assert(JsonObj(root, "myobj", true, &val) == 1);
        assert(JsonObj(root, "myobj", false, &ret) == 1);

        float val2 = 3.141516f, ret2;
        assert(JsonFloat(root, "myobj.pi", true, &val2) == 1);
        assert(JsonFloat(root, "myobj.pi", false, &ret2) == 1);
        assert(val2 == ret2);
    }

    {
        int val, ret;
        assert(JsonParse("[]", &val) == 1);
        assert(JsonObj(root, "array", false, &ret) == 0);
        assert(JsonObj(root, "array", true, &val) == 1);
        assert(JsonObj(root, "array", false, &ret) == 1);
        for (int i = 0; i < 5; i++) {
            assert(JsonInt(root, "array.-1", true, &i) == 1);
        }

        assert(JsonObj(root, "array", false, &ret) == 1);
        int count = JsonSize(ret);
        assert(count == 5);
        
        for (int i = 0; i < JsonSize(ret); i++) {
            int val2;
            assert(JsonIntFromIdx(ret, i, false, &val2) == 1);
            assert(val2 == i);
        }
    }

    {
        int j;
        JsonParse("{\"a\":{\"b\":5}}", &j);
        int val;
        JsonParse("{\"foo\":6}", &val);
        assert(JsonObj(j, "a.b.c.d", true, &val) == 0);
        const char* result;
        JsonStringify(j, &result);
        std::string ref = "{\"a\":{\"b\":5}}";
        assert(ref.compare(result) == 0);
    }

    const char* result;
    JsonStringify(root, &result);
    std::string ref = "{\"array\":[0,1,2,3,4],\"float\":1.2345670461654663,\"myobj\":{\"pi\":3.1415159702301025},\"number\":5,\"string\":\"hello 1\"}";
    assert(ref.compare(result) == 0);
}