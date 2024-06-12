#include "rad/Core/Platform.h"
#include "rad/Core/String.h"
#include "rad/IO/Logging.h"
#include "rad/System/CpuInfo.h"
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    rad::SetupDefaultLogger("HelloWorld.log", true);

    RAD_LOG(Default, info, "Hello, {}!", "World");
#if defined(CPU_FEATURES_ARCH_X86)
    RAD_LOG(Default, info, "CPU: {} ({})",
        rad::StrTrim(rad::g_X86Info.brand_string),
        rad::g_X86Info.vendor);
#endif
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
