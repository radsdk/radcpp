#pragma once

#include <rad/Core/Platform.h>
#include <ctime>
#include <chrono>

namespace rad
{

struct tm* LocalTime(const time_t* timer, struct tm* buffer);

} // namespace rad
