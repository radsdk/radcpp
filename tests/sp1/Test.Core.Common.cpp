#include "gtest/gtest.h"
#include "rad/Core/Common.h"

using namespace rad;

TEST(Core, Common)
{
    EXPECT_EQ(CountBits(0x00000000), 0);
    EXPECT_EQ(CountBits(0x55555555), 16);
    EXPECT_EQ(CountBits(0xAAAAAAAA), 16);
    EXPECT_EQ(CountBits(0xFFFFFFFF), 32);
}
