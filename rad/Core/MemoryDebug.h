#pragma once

#include <rad/Core/Platform.h>

#if defined(_DEBUG)
    #if defined(RAD_OS_WINDOWS) && defined(RAD_COMPILER_MSVC)
        // https://learn.microsoft.com/en-us/cpp/c-runtime-library/find-memory-leaks-using-the-crt-library?view=msvc-170
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
        #define RAD_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
    #else
        #define RAD_NEW new
    #endif
#else
    #define RAD_NEW new
#endif

namespace rad
{

void EnableMemoryLeakDetection();
void DumpMemoryStatistics();
void MemoryCheckBegin();
void MemoryCheckEnd();

} // namespace rad
