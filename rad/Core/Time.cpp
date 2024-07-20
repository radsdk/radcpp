#include <rad/Core/Time.h>
#include <cassert>

namespace rad
{

tm* LocalTime(const time_t* timer, tm* buffer)
{
    assert((timer != nullptr) && (buffer != nullptr));
#if defined(RAD_OS_WINDOWS)
    errno_t err = localtime_s(buffer, timer);
    if (err == 0)
    {
        return buffer;
    }
    else
    {
        return nullptr;
    }
#else
    return localtime_r(timer, buffer);
#endif
}

std::string GetTimeStringUTC()
{
    std::time_t t = std::time(nullptr);
    if (t == (std::time_t)(-1))
    {
        return std::string();
    }
    std::string buffer(sizeof("YYYY-MM-DDThh:mm:ssZ"), 0);
#if defined(RAD_COMPILER_MSVC)
    std::tm dateTime = {};
    gmtime_s(&dateTime, &t);
    size_t bytesWritten = std::strftime(buffer.data(), buffer.size(),
        "%FT%TZ", &dateTime);
#else
    size_t bytesWritten = std::strftime(buffer.data(), buffer.size(),
        "%FT%TZ", std::gmtime(&t));
#endif
    if (bytesWritten == buffer.size() - 1)
    {
        buffer.pop_back(); // remove '\0' - NUL terminator.
        return buffer;
    }
    else
    {
        buffer.clear();
        return buffer;
    }
}

std::string GetTimeStringISO8601()
{
    std::time_t t = std::time(nullptr);
    if (t == (std::time_t)(-1))
    {
        return std::string();
    }
    std::string buffer(sizeof("YYYY-MM-DDThh:mm:ss+0000"), 0);
#if defined(RAD_COMPILER_MSVC)
    std::tm dateTime = {};
    localtime_s(&dateTime, &t);
    size_t bytesWritten = std::strftime(buffer.data(), buffer.size(),
        "%FT%T%z", &dateTime);
#else
    size_t bytesWritten = std::strftime(buffer.data(), buffer.size(),
        "%FT%T%z", std::localtime(&t));
#endif
    if (bytesWritten == buffer.size() - 1)
    {
        buffer.pop_back(); // remove '\0' - NUL terminator.
        return buffer;
    }
    else
    {
        buffer.clear();
        return buffer;
    }
}

std::string GetTimeStringRFC2822()
{
    std::time_t t = std::time(nullptr);
    if (t == (std::time_t)(-1))
    {
        return std::string();
    }
    std::string buffer(sizeof("Fri, 28 Jun 2024 18:16:09 +0000"), 0);
#if defined(RAD_COMPILER_MSVC)
    std::tm dateTime = {};
    localtime_s(&dateTime, &t);
    size_t bytesWritten = std::strftime(buffer.data(), buffer.size(),
        "%a, %d %b %Y %T %z", &dateTime);
#else
    size_t bytesWritten = std::strftime(buffer.data(), buffer.size(),
        "%a, %d %b %Y %T %z", std::localtime(&t));
#endif
    if (bytesWritten == buffer.size() - 1)
    {
        buffer.pop_back(); // remove '\0' - NUL terminator.
        return buffer;
    }
    else
    {
        buffer.clear();
        return buffer;
    }
}

} // namespace rad
