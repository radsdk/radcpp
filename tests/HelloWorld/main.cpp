#include "rad/Core/Platform.h"
#include <iostream>
#include <format>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    std::cout << std::format("Hello, {}!", "World") << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
