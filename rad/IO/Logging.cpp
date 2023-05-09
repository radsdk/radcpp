#include "rad/IO/Logging.h"
#include "rad/IO/File.h"

#include <cstdarg>

RAD_LOG_CATEGORY_DEFINE(Global);

NAMESPACE_RAD_BEGIN

LogDevice g_logDevice;

const char* g_logLevelStrings[int(LogLevel::Count)] =
{
    "Undefined",
    "Verbose",
    "Debug",
    "Info",
    "Warn",
    "Error",
    "Critical",
};

const char* GetLogLevelString(LogLevel level)
{
    return g_logLevelStrings[int(level)];
}

LogDevice::LogDevice()
{
}

LogDevice::~LogDevice()
{
}

void LogDevice::Write(const char* data, size_t sizeInBytes)
{
    std::lock_guard lockGuard(m_outputMutex);

    if (!m_logFile.IsOpen())
    {
        std::string processName = (const char*)IO::GetCurrentProcessPath()
            .filename().u8string().c_str();
        m_logFile.Open(processName + ".log", IO::FileAccessWrite);
    }

    if (m_logFile.IsOpen())
    {
        m_logFile.Write(data, sizeInBytes);
    }

    if (m_enableConsoleOutput)
    {
        fprintf(stderr, "%s", data);
    }
}

void LogDevice::Flush()
{
    m_logFile.Flush();
}

void LogCategory::Print(LogLevel level, const char* format, ...)
{
    LogDevice::Clock::time_point timestamp = std::chrono::system_clock::now();

    thread_local std::string message;
    va_list args;
    va_start(args, format);
    StrPrintInPlaceArgList(message, format, args);
    va_end(args);

    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        timestamp.time_since_epoch()).count() % 1000;
    std::time_t timepoint = std::chrono::system_clock::to_time_t(timestamp);
    std::tm datetime = {};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&datetime, &timepoint);
#else
    localtime_r(&timepoint, &datetime);
#endif

    thread_local std::string buffer;
    int charsPrinted = StrPrintInPlace(buffer, "[%02d:%02d:%02d.%03lld] %s: %s: %s\n",
        datetime.tm_hour, datetime.tm_min, datetime.tm_sec, milliseconds,
        m_name.data(), g_logLevelStrings[int(level)], message.data());

    if ((charsPrinted > 0) && (level >= m_outputLevel))
    {
        g_logDevice.Write(buffer.data(), charsPrinted);
    }

    if (level >= m_flushLevel)
    {
        g_logDevice.Flush();
    }
}

NAMESPACE_RAD_END
