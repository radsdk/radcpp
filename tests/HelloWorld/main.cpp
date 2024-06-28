#include <rad/Core/MemoryDebug.h>
#include <rad/Core/String.h>
#include <rad/Core/Time.h>
#include <rad/IO/Logging.h>
#include <rad/System/CpuInfo.h>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
#if defined(_DEBUG)
    rad::EnableMemoryLeakDetection();
#endif

    rad::InitLogging("HelloWorld.log", true);

    auto logger = rad::GetDefaultLogger();
    RAD_LOG(logger, info, "Hello, {}!", "World");

    RAD_LOG(logger, info, "Time UTC: {}", rad::GetTimeStringUTC());
    RAD_LOG(logger, info, "Time ISO-8601: {}", rad::GetTimeStringISO8601());
    RAD_LOG(logger, info, "Time RFC 2822: {}", rad::GetTimeStringRFC2822());

#if defined(CPU_FEATURES_ARCH_X86)
    RAD_LOG(logger, info, "CPU: {} ({})",
        rad::StrTrim(rad::g_X86Info.brand_string),
        rad::g_X86Info.vendor);
#endif

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
