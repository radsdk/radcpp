#pragma once

#include <rad/Core/Platform.h>
#include <rad/IO/FileSystem.h>
#include <boost/process/env.hpp>

namespace rad
{

extern boost::process::native_environment g_environ;

uint32_t getpid();
FilePath GetCurrentProcessPath();
std::string GetCurrentProcessName();

std::vector<FilePath> listdrives();

bool SetConsoleOutputEncodingUTF8();

} // namespace rad
