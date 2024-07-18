#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/Float.h>

namespace rad
{

uint8_t FP8E4M3_FromFP32(float f);
float FP8E4M3_ToFP32(uint8_t input);

uint8_t FP8E5M2_FromFP32(float f);
float FP8E5M2_ToFP32(uint8_t input);

} // namespace rad
