#include <string>
#include <sstream>
#include <vector>
#include <iterator>

// https://github.com/nlohmann/json
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace misc
{
    template<typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            *(result++) = item;
        }
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    /*
        returns pointer to JSON item or NULL if not found.
        path will be modified to point a valid path
        ex: "a.b.c.d" will becomes "a.b" if c do not exist
    */
    nlohmann::basic_json<>* JsonTravel(nlohmann::basic_json<>* j, std::string &path) {
        if (!j) return NULL;
        bool isFirst = true;
        auto v = misc::split(path, '.');
        path = "";
        auto p = j;
        for (auto it = v.begin(); it != v.end(); ++it) {
            if (p->is_null()) return p;
            if (p->is_array()) {
                int idx = std::stoi(*it);
                if (idx >= (signed)p->size()) return NULL;
                if (idx < 0) return NULL;
                p = &(p->at(idx));
            } else {
                auto key = *it;
                if (p->find(key) == p->end()) return NULL;
                p = &(p->at(key));
            }

            if (!isFirst) path += ".";
            path += *it;
            isFirst = false;
        }
        return p;
    }
}
