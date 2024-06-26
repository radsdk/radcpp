#include <rad/IO/Logging.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/cfg/env.h>

namespace rad
{

static std::shared_ptr<spdlog::logger> g_logger;
static spdlog::sink_ptr g_logFileSink;
#if defined(_DEBUG) && defined(RAD_COMPILER_MSVC)
static spdlog::sink_ptr g_msvcSink;
#endif

bool InitLogging(const std::string& filename, bool truncate)
{
    if (!g_logger)
    {
        g_logger = spdlog::stdout_color_mt("Default");
        spdlog::set_default_logger(g_logger);
    }

    if (!g_logFileSink)
    {
        g_logFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, truncate);
        g_logger->sinks().push_back(g_logFileSink);
    }

#if defined(_DEBUG) && defined(RAD_COMPILER_MSVC)
    if (!g_msvcSink)
    {
        g_msvcSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        g_logger->sinks().push_back(g_msvcSink);
    }
#endif

    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    spdlog::set_pattern("%^[%T.%e] %n (%l)%$: %v");
    spdlog::flush_on(spdlog::level::warn);
    spdlog::cfg::load_env_levels();
    return true;
}

spdlog::logger* GetDefaultLogger()
{
    return g_logger.get();
}

std::vector<spdlog::sink_ptr>& GetDefaultLogSinks()
{
    return g_logger->sinks();
}

spdlog::sink_ptr GetDefaultLogFileSink()
{
    return g_logFileSink;
}

std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name)
{
    auto& sinks = g_logger->sinks();
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(name,
        sinks.begin(), sinks.end());
    spdlog::initialize_logger(logger);
    return std::move(logger);
}

spdlog::logger* GetGuiLogger()
{
    static std::shared_ptr<spdlog::logger> logger = CreateLogger("Gui");
    return logger.get();
}

spdlog::logger* GetMediaLogger()
{
    static std::shared_ptr<spdlog::logger> logger = CreateLogger("Media");
    return logger.get();
}

} // namespace rad
