#include <gtest/gtest.h>
#include <rad/Core/Integer.h>

TEST(Core, Integer)
{
    EXPECT_TRUE(rad::HasBits(0xAAAAAAAAu, 0xA0A0A0A0u));
    EXPECT_TRUE(rad::HasBits(0xAAAAAAAAu, 0u));
    EXPECT_FALSE(rad::HasBits(0xAAAAAAAAu, 0xA1A0A0A0u));
    EXPECT_FALSE(rad::HasBits(0u, 0x10u));

    EXPECT_TRUE(rad::HasNoBits(0xAAAAAAAAu, 0x55555555u));
    EXPECT_TRUE(rad::HasNoBits(0xAAAAAAAAu, 0u));
    EXPECT_FALSE(rad::HasNoBits(0xAAAAAAAAu, 0x52555555u));
    EXPECT_FALSE(rad::HasNoBits(0xAAAAAAAAu, 0x52555855u));

    EXPECT_TRUE(rad::HasAnyBits(0xAAAAAAAAu, 0x02000000u));
    EXPECT_TRUE(rad::HasAnyBits(0xAAAAAAAAu, 0xFFFFFFFFu));
    EXPECT_FALSE(rad::HasAnyBits(0xAAAAAAAAu, 0x55555555u));
    EXPECT_FALSE(rad::HasAnyBits(0xAAAAAAAAu, 0u));

    EXPECT_EQ(rad::RoundUpToMultiple(0u, 12u), 0u);
    EXPECT_EQ(rad::RoundUpToMultiple(90u, 12u), 96u);
    EXPECT_EQ(rad::RoundDownToMultiple(0u, 12u), 0u);
    EXPECT_EQ(rad::RoundDownToMultiple(90u, 12u), 84u);

    EXPECT_TRUE(rad::IsPow2(0x10000000u));
    EXPECT_TRUE(rad::IsPow2(0x1000000000000000u));
    EXPECT_FALSE(rad::IsPow2(0x10001000u));
    EXPECT_FALSE(rad::IsPow2(0x1000000010000000u));

    EXPECT_EQ(rad::Pow2AlignUp(0u, 16u), 0u);
    EXPECT_EQ(rad::Pow2AlignUp(90u, 16u), 96u);
    EXPECT_EQ(rad::Pow2AlignDown(0u, 16u), 0u);
    EXPECT_EQ(rad::Pow2AlignDown(90u, 16u), 80u);

    EXPECT_EQ(rad::CountBits(0xAAAAAAAAu), 16);
    EXPECT_EQ(rad::CountBits(0xFFFFFFFFu), 32);
    EXPECT_EQ(rad::CountBits(0xAAAAAAAAAAAAAAAAu), 32);
    EXPECT_EQ(rad::CountBits(0xFFFFFFFFFFFFFFFFu), 64);

    EXPECT_EQ(rad::BitScanReverse32(0x4040u), 14);
    EXPECT_EQ(rad::BitScanReverse32(0x8080u), 15);
    EXPECT_EQ(rad::BitScanReverse64(0x400000004000u), 46);
    EXPECT_EQ(rad::BitScanReverse64(0x800000008000u), 47);

    EXPECT_EQ(rad::RoundUpToNextPow2(0u), 1u);
    EXPECT_EQ(rad::RoundUpToNextPow2(90u), 128u);
    EXPECT_EQ(rad::RoundUpToNextPow2(128u), 256u);
    EXPECT_EQ(rad::RoundUpToPow2(0u), 1u);
    EXPECT_EQ(rad::RoundUpToPow2(90u), 128u);
    EXPECT_EQ(rad::RoundUpToPow2(128u), 128u);

    EXPECT_EQ(rad::PackU8x4(1, 2, 3, 4), 0x04030201u);
    EXPECT_EQ(rad::PackS8x4(1, 2, 3, 4), 0x04030201u);
    EXPECT_EQ(rad::PackS8x4(-1, -2, -3, -4), 0b11111100111111011111111011111111u);
}
