#pragma once
//std
#include <string>
// system headers
#include <windows.h>

namespace io
{
	bool DirectoryDelete(const std::string& directoryPath, UINT maxRetries = 10, UINT millisecondsDelay = 30);
	bool DirectoryExists(const std::string& dirName_in);
}
