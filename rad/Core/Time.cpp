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

} // namespace rad
