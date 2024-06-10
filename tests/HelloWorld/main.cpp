#include "rad/Core/Platform.h"
#include "rad/IO/Logging.h"
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    rad::SetupDefaultLogger("HelloWorld.log", true);

    SPDLOG_INFO("Hello, {}!", "World");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
