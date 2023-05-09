#pragma once

#include "rad/Core/Common.h"
#include "rad/IO/File.h"
#include <chrono>
#include <mutex>

NAMESPACE_RAD_BEGIN

enum class LogLevel
{
    Undefined,
    Verbose,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
    Count
};

const char* GetLogLevelString(LogLevel level);

class LogDevice
{
public:
    using Clock = std::chrono::system_clock;

    LogDevice();
    ~LogDevice();

    void Write(const char* data, size_t sizeInBytes);
    void Flush();

    void EnableConsoleOutput(bool enableConsoleOutput = true)
    {
        m_enableConsoleOutput = enableConsoleOutput;
    }

private:
    std::mutex m_outputMutex;
    IO::File m_logFile;

    bool m_enableConsoleOutput = true;

}; // class LogDevice

extern LogDevice g_logDevice;

class LogCategory
{
public:
    LogCategory(std::string_view name, LogLevel outputLevel) :
        m_name(name),
        m_outputLevel(outputLevel)
    {
    }
    ~LogCategory()
    {
    }

    void SetOutputLevel(LogLevel outputLevel)
    {
        m_outputLevel = outputLevel;
    }

    void SetFlushLevel(LogLevel flushLevel)
    {
        m_flushLevel = flushLevel;
    }

    void Print(LogLevel level, const char* format, ...);

protected:
    std::string m_name;
    LogLevel m_outputLevel;
    LogLevel m_flushLevel = LogLevel::Warn;

}; // class LogCategory

NAMESPACE_RAD_END

#ifndef RAD_NO_LOGGING

#define RAD_LOG_CATEGORY(Name) g_logCategory##Name
#define RAD_LOG_CATEGORY_DECLARE(Name, OutputLevel) \
class LogCategory##Name : public rad::LogCategory \
{ \
public: \
    LogCategory##Name() : LogCategory(#Name, rad::LogLevel::OutputLevel) {} \
}; \
extern LogCategory##Name RAD_LOG_CATEGORY(Name);
#define RAD_LOG_CATEGORY_DEFINE(Name) LogCategory##Name RAD_LOG_CATEGORY(Name);
#define RAD_LOG(Name, Level, Format, ...) RAD_LOG_CATEGORY(Name).Print(rad::LogLevel::Level, Format, ##__VA_ARGS__)
#define RAD_SET_LOG_OUTPUT_LEVEL(Name, Level) RAD_LOG_CATEGORY(Name).SetOutputLevel(LogLevel::Level)
#define RAD_SET_LOG_FLUSH_LEVEL(Name, Level) RAD_LOG_CATEGORY(Name).SetFlushLevel(LogLevel::Level)

#else

#define RAD_LOG_CATEGORY(Name)
#define RAD_LOG_CATEGORY_DECLARE(Name, OutputLevel)
#define RAD_LOG_CATEGORY_DEFINE(Name)
#define RAD_LOG(CategoryName, Level, Format, ...)
#define RAD_SET_LOG_OUTPUT_LEVEL(Name, Level)
#define RAD_SET_LOG_FLUSH_LEVEL(Name, Level)

#endif

RAD_LOG_CATEGORY_DECLARE(Global, Verbose);
#define LogGlobal(Level, Format, ...) RAD_LOG(Global, Level, Format, ##__VA_ARGS__)
