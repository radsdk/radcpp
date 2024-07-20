#include <gtest/gtest.h>
#include <rad/IO/Logging.h>

int main(int argc, char* argv[])
{
    rad::InitLogging("test.log", true);
    for (int i = 0; i < argc; ++i)
    {
        SPDLOG_INFO("argv[{}]: {}", i, argv[i]);
    }
    SPDLOG_INFO("Hello, World!");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
