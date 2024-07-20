#pragma once

#include <rad/Core/Platform.h>
#include <spdlog/spdlog.h>

namespace rad
{

// Not thread safe.
bool InitLogging(spdlog::sink_ptr fileSink);
bool InitLogging(const std::string& filename, bool truncate);
std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name);

} // namespace rad

#if !defined(RAD_DISABLE_LOGGING)
#define RAD_LOG(Logger, Level, ...) SPDLOG_LOGGER_CALL(Logger, spdlog::level::Level, __VA_ARGS__)
#else
#define RAD_LOG(Logger, Level, ...)
#endif
