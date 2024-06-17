#include <rad/Core/MemoryDebug.h>

namespace rad
{

void EnableMemoryLeakDetection()
{
#if defined(RAD_OS_WINDOWS) && defined(RAD_COMPILER_MSVC)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void DumpMemoryStatistics()
{
#if defined(RAD_OS_WINDOWS) && defined(RAD_COMPILER_MSVC)
    _CrtMemState s = {};
    _CrtMemCheckpoint(&s);
    _CrtMemDumpStatistics(&s);
#endif
}

static _CrtMemState g_memState1;
static _CrtMemState g_memState2;

void MemoryCheckBegin()
{
#if defined(RAD_OS_WINDOWS) && defined(RAD_COMPILER_MSVC)
    _CrtMemCheckpoint(&g_memState1);
#endif
}

void MemoryCheckEnd()
{
#if defined(RAD_OS_WINDOWS) && defined(RAD_COMPILER_MSVC)
    _CrtMemCheckpoint(&g_memState2);
    _CrtMemState memDiff = {};
    if (_CrtMemDifference(&memDiff, &g_memState1, &g_memState2))
    {
        _CrtMemDumpStatistics(&memDiff);
        _CrtMemDumpAllObjectsSince(&g_memState1);
    }
#endif
}

} // namespace rad
