#include <rad/Core/Platform.h>
#include <rad/Core/String.h>
#include <rad/IO/Logging.h>
#include <rad/System/CpuInfo.h>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    rad::InitLogging("HelloWorld.log", true);

    auto logger = rad::GetDefaultLogger();

    RAD_LOG(logger, info, "Hello, {}!", "World");
#if defined(CPU_FEATURES_ARCH_X86)
    RAD_LOG(logger, info, "CPU: {} ({})",
        rad::StrTrim(rad::g_X86Info.brand_string),
        rad::g_X86Info.vendor);
#endif
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
