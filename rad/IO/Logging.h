#pragma once

#include "rad/Core/Platform.h"
#include "spdlog/spdlog.h"

namespace rad
{

bool SetupDefaultLogger(const std::string& filename, bool truncate);
std::vector<spdlog::sink_ptr>& GetDefaultLogSinks();
spdlog::sink_ptr GetDefaultLogFileSink();

} // namespace rad
