#pragma once

#include <rad/Core/Integer.h>
#include <cfloat>

namespace rad
{

inline float fp32_from_bits(uint32_t input)
{
    union
    {
        uint32_t u32;
        float f32;
    };
    u32 = input;
    return f32;
}

inline uint32_t fp32_to_bits(float input)
{
    union
    {
        float f32;
        uint32_t u32;
    };
    f32 = input;
    return u32;
}

float Normalize(float value, float min, float max);
uint8_t QuantizeUnorm8(float normalized);
uint16_t QuantizeUnorm16(float normalized);
uint32_t QuantizeUnorm32(double normalized);
float DequantizeUnorm8(uint8_t quantized);
float DequantizeUnorm16(uint16_t quantized);
float DequantizeUnorm32(uint32_t quantized);

} // namespace rad
