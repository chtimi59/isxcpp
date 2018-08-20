#pragma once

namespace heap
{
    const LPVOID push(const LPVOID buff, const size_t size);
    const char* push(std::string);
    void release(const LPVOID buff);
    void release();
}

namespace io
{
    void DbgOutput(const char* szFormat, ...);
    void MsgBox(const std::string& txt, const std::string& caption = "");
    bool DirectoryDelete(const std::string& directoryPath, UINT maxRetries = 10, UINT millisecondsDelay = 30);
    bool DirectoryExists(const std::string& dirName_in);
}

class PString
{
    public:

		PString() {}
		PString(std::string &val) { this->set(val); }
		PString(const char* val) { this->set(val); }
        
		void set(std::string &val);
        void set(const char* val);

        PString& operator=(const PString& b);
        bool operator!=(const PString& other) const;
        bool operator==(const PString& other) const;
		LPARAM getLParam();

    private:
        char buff[MAX_PATH] = { 0 };
        size_t size = 0;
};
