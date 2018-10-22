#pragma once

namespace Http
{
    const std::string Get(
        const std::string& url,
        long* pResponseCode = NULL);

    const std::string Post(
        const std::string& url,
        const std::string& ContentType,
        const std::string& body,
        long* pResponseCode = NULL);
}
