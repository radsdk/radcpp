#pragma once

#include <rad/Core/Platform.h>
#include <ctime>
#include <chrono>

namespace rad
{

struct tm* LocalTime(const time_t* timer, struct tm* buffer);
// Returns string in format "YYYY-MM-DDThh:mm:ssZ" or empty if failed.
std::string GetTimeStringUTC();
// Returns string in format "YYYY-MM-DDThh:mm:ss+0000" or empty if failed.
std::string GetTimeStringISO8601();
// Returns string in format "Fri, 28 Jun 2024 18:00:00 +0000" or empty if failed.
std::string GetTimeStringRFC2822();

} // namespace rad
