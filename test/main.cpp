#include <gtest/gtest.h>
#include <rad/System/Program.h>
#include <rad/IO/Logging.h>

int main(int argc, char* argv[])
{
    rad::Program program(argc, argv);
    for (int i = 0; i < argc; ++i)
    {
        SPDLOG_INFO("argv[{}]: {}", i, argv[i]);
    }
    SPDLOG_INFO("Hello, World!");
#if defined(_DEBUG)
    program.PrintStackTrace();
#endif

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
