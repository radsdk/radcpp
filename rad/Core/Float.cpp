#include "Float.h"

namespace rad
{

float Normalize(float value, float min, float max)
{
    assert(min < max);
    if (value < min)
    {
        return 0.0f;
    }
    if (value > max)
    {
        return 1.0f;
    }
    return (value - min) / (max - min);
}

uint8_t QuantizeUnorm8(float normalized)
{
    assert(normalized >= 0.0f && normalized <= 1.0f);
    float scaled = normalized * float(UINT8_MAX);
    uint8_t rounded = (uint8_t)(scaled + 0.5f);
    return rounded;
}

uint16_t QuantizeUnorm16(float normalized)
{
    assert(normalized >= 0.0f && normalized <= 1.0f);
    float scaled = normalized * float(UINT16_MAX);
    uint16_t rounded = (uint16_t)(scaled + 0.5f);
    return rounded;
}

uint32_t QuantizeUnorm32(double normalized)
{
    assert(normalized >= 0.0f && normalized <= 1.0f);
    double scaled = normalized * double(UINT32_MAX);
    uint32_t rounded = (uint32_t)(scaled + 0.5f);
    return rounded;
}

float DequantizeUnorm8(uint8_t quantized)
{
    float interval = 1.0f / float(UINT8_MAX);
    return float(quantized) * interval;
}

float DequantizeUnorm16(uint16_t quantized)
{
    float interval = 1.0f / float(UINT16_MAX);
    return float(quantized) * interval;
}

float DequantizeUnorm32(uint32_t quantized)
{
    double interval = 1.0f / double(UINT32_MAX);
    return float(double(quantized) * interval);
}

} // namespace rad
