#pragma once

#include "rad/Core/Platform.h"
#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"

namespace rad
{

// Should be called once at the start of main, not thread safe.
bool SetupDefaultLogger(const std::string& filename, bool truncate);
std::vector<spdlog::sink_ptr>& GetDefaultLogSinks();
spdlog::sink_ptr GetDefaultLogFileSink();

std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name);

} // namespace rad
