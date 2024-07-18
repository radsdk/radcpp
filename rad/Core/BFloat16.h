#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/Float.h>

namespace rad
{

// The bfloat16 format was developed by Google Brain, an artificial intelligence research group at Google.
// https://en.wikipedia.org/wiki/Bfloat16_floating-point_format
// sign: 1 bit; exponent: 8 bits; fraction: 7 bits;
// precision: between two and three decimal digits.
// range: up to about 3.4 × 10^38.
// max: 0x7f7f = 0 11111110 1111111 = (28 - 1) * 2^-7 * 2^127; about 3.38953139 * 10^38 < FLT_MAX
// min: 0x0080 = 0 00000001 0000000 = 2^-126; about 1.175494351 * 10^-38
// min (subnormal): 2^-126-7=2-133; about 9.2 * 10^-41
#define BF16_MIN 0x0080
#define BF16_MAX 0x7f7f
// 0000 = 0 00000000 0000000 = 0
// 8000 = 1 00000000 0000000 = −0
// 7f80 = 0 11111111 0000000 = infinity
// ff80 = 1 11111111 0000000 = −infinity
// 4049 = 0 10000000 1001001 = 3.140625; // pi
// 3eab = 0 01111101 0101011 = 0.333984375 // 1/3
// NaN: not all significand bits zero.
// ffc1 = x 11111111 1000001 => qNaN
// ff81 = x 11111111 0000001 => sNaN

// Round toward 0, NaN handled (0x7FC0).
uint16_t BF16_FromFP32RoundToZero(float x);
// Google TPU and NVIDIA, NaN handled (0x7FC0).
uint16_t BF16_FromFP32RoundToNearestEven(float x);
float BF16_ToFP32(uint16_t x);

} // namespace rad
