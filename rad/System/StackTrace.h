#pragma once

#include <rad/Core/Platform.h>

namespace rad
{

bool InstallDefaultSignalHandlers();
void PrintStackTrace(int depth = 32);

} // namespace rad
