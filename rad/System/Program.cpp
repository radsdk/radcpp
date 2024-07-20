#include <rad/System/Program.h>
#include <rad/System/CpuInfo.h>
#include <rad/IO/Logging.h>
#include <backward.hpp>

namespace rad
{

static Program* g_program = nullptr;

Program::Program(int argc, char** argv)
{
    assert(g_program == nullptr);
    g_program = this;
    InstallDefaultSignalHandlers();
    InitLogging(std::string(argv[0]) + ".log", true);
    LogSystemInfo();
}

Program::~Program()
{
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

Program* GetProgram()
{
    return g_program;
}

} // namespace rad
