#include <gtest/gtest.h>
#include <rad/Core/Float.h>
#include <rad/IO/Logging.h>

TEST(Core, Float)
{
    EXPECT_EQ(rad::QuantizeUnorm8(0.0f), 0);
    EXPECT_EQ(rad::QuantizeUnorm16(0.0f), 0);
    EXPECT_EQ(rad::QuantizeUnorm32(0.0f), 0);

    EXPECT_EQ(rad::QuantizeUnorm8(1.0f), UINT8_MAX);
    EXPECT_EQ(rad::QuantizeUnorm16(1.0f), UINT16_MAX);
    EXPECT_EQ(rad::QuantizeUnorm32(1.0f), UINT32_MAX);

    const size_t count = 10000;
    float e8 = 0.0f;
    float e16 = 0.0f;
    float e32 = 0.0f;
    for (size_t i = 0; i < count; ++i)
    {
        float normalized = float(i) * (1.0f / float(count));
        float f8 = normalized;
        float f16 = normalized;
        float f32 = normalized;
        for (size_t repeat = 0; repeat < 100; ++repeat)
        {
            float epsilon = 0.0f;
            f8 = rad::DequantizeUnorm8(rad::QuantizeUnorm8(f8));
            epsilon = std::abs(f8 - normalized);
            if (e8 < epsilon)
            {
                EXPECT_EQ(repeat, 0);
                e8 = epsilon;
            }
            f16 = rad::DequantizeUnorm16(rad::QuantizeUnorm16(f16));
            epsilon = std::abs(f16 - normalized);
            if (e16 < epsilon)
            {
                EXPECT_EQ(repeat, 0);
                e16 = epsilon;
            }
            f32 = rad::DequantizeUnorm32(rad::QuantizeUnorm32(f32));
            epsilon = std::abs(f32 - normalized);
            if (e32 < epsilon)
            {
                EXPECT_EQ(repeat, 0);
                e32 = epsilon;
            }
        }
    }

    EXPECT_LE(e8, 0.002f);
    EXPECT_LE(e16, 0.000008f);
    EXPECT_LE(e32, 0.0000000002f);

    SPDLOG_INFO("QuantizeUnorm8 max epsilon: {}", e8);
    SPDLOG_INFO("QuantizeUnorm16 max epsilon: {}", e16);
    SPDLOG_INFO("QuantizeUnorm32 max epsilon: {}", e32);
}
