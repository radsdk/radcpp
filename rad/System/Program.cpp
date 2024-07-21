#include <rad/System/Program.h>
#include <rad/System/CpuInfo.h>
#include <rad/IO/Logging.h>
#include <backward.hpp>

namespace rad
{

static Program* g_program = nullptr;

Program* GetProgram()
{
    return g_program;
}

Program::Program()
{
    assert(g_program == nullptr);
    g_program = this;
}

Program::~Program()
{
    g_program = nullptr;
}

bool Program::Init(int argc, char** argv)
{
#if defined(_DEBUG) && defined(RAD_OS_WINDOWS) && defined(RAD_COMPILER_MSVC)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    InstallDefaultSignalHandlers();
    InitLogging(std::string(argv[0]) + ".log", true);
    LogSystemInfo();
    return true;
}

// Implement with backward-cpp: https://github.com/bombela/backward-cpp
// C++23 <stacktrace>: https://en.cppreference.com/w/cpp/header/stacktrace
void Program::PrintStackTrace(int depth)
{
    using namespace backward;
    // On Windows, must delcare Printer before load_here, or the first print won't work, is it a bug?
    Printer p;
    StackTrace st;
    p.color_mode = ColorMode::never;
    p.trace_context_size = 9;
    st.skip_n_firsts(2); // skip current and load_here.
    st.load_here(depth);
    p.print(st);
}

void Program::Exit(int code)
{
    std::exit(code);
}

void Program::QuickExit(int code)
{
    std::quick_exit(code);
}

void Program::Terminate()
{
    std::terminate();
}

void Program::Abort()
{
    std::abort();
}

bool Program::InstallDefaultSignalHandlers()
{
    backward::SignalHandling signalHandling;
    return signalHandling.loaded();
}

bool Program::LogSystemInfo()
{
#if defined(CPU_FEATURES_ARCH_X86)
    SPDLOG_INFO("CPU: {} ({})",
        rad::StrTrim(rad::g_X86Info.brand_string),
        rad::g_X86Info.vendor);
#endif
    return true;
}

} // namespace rad
