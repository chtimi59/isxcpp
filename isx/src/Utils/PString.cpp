#include "common.h"

LPARAM PString::getLParam() {
    return (LPARAM)buff;
}

void PString::set(std::string &val) {
    size = val.empty() ? 0 : val.size();
    if (size + 1 >= sizeof(buff)) throw std::invalid_argument("overload");
    memcpy(buff, val.c_str(), size);
    buff[size++] = '\0';
}

void PString::set(const char* val) {
    size = (val == NULL) ? 0 : strlen(val);
    if (size + 1 >= sizeof(buff)) throw std::invalid_argument("overload");
    strcpy_s(buff, sizeof(buff), val);
    buff[size++] = '\0';
}

PString& PString::operator=(const PString& other) {
    size = other.size;
    memcpy(&buff, &(other.buff), size);
    return *this;
}

bool PString::operator==(const PString& other) const {
    if (size != other.size) return FALSE;
    if (memcmp(buff, other.buff, size)) return FALSE;
    return TRUE;
}

bool PString::operator!=(const PString& other) const {
    if (size != other.size) return TRUE;
    if (memcmp(buff, other.buff, size)) return TRUE;
    return FALSE;
}
