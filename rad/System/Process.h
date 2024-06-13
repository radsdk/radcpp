#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/String.h>
#include <boost/process.hpp>

namespace rad
{

std::vector<std::string> ExecuteAndReadLines(const std::string& command);

} // namespace rad
