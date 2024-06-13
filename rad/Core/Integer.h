#pragma once

#include <rad/Core/Platform.h>
#include <cassert>
#include <cstdint>
#include <bit>
// has_single_bit: checks if a number is an integral power of 2.
// bit_ceil: finds the smallest integral power of two not less than the given value.
// bit_floor: finds the largest integral power of two not greater than the given value.
// bit_width: finds the smallest number of bits needed to represent the given value.
// rotl: computes the result of bitwise left-rotation.
// rotr: computes the result of bitwise right-rotation.
// countl_zero: counts the number of consecutive ​0​ bits, starting from the most significant bit.
// countl_one: counts the number of consecutive 1 bits, starting from the most significant bit.
// countr_zero: counts the number of consecutive ​0​ bits, starting from the least significant bit.
// countr_one: counts the number of consecutive 1 bits, starting from the least significant bit.
// popcount: counts the number of 1 bits in an unsigned integer.
#include <concepts>
// integral/signed_integral/unsigned_integral
// is_integral/is_signed/is_unsigned

namespace rad
{

using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;

using Uint8 = uint8_t;
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint64 = uint64_t;

template<std::integral T>
constexpr bool HasBits(T mask, T bits) noexcept
{
    return ((mask & bits) == bits);
}

template<std::integral T>
constexpr bool HasNoBits(T mask, T bits) noexcept
{
    return ((mask & bits) == 0);
}

template<std::integral T>
constexpr bool HasAnyBits(T mask, T bits) noexcept
{
    return ((mask & bits) != 0);
}

template<std::unsigned_integral T>
constexpr T RoundUpToMultiple(T value, T alignment)
{
    assert(alignment >= 1);
    assert((value + (alignment - 1)) >= value); // check overflow
    return (((value + (alignment - 1)) / alignment) * alignment);
}

template <std::unsigned_integral T>
constexpr T RoundDownToMultiple(T value, T alignment)
{
    assert(alignment >= 1);
    return ((value / alignment) * alignment);
}

template<std::unsigned_integral T>
constexpr bool IsPow2(T x)
{
#if defined(__cpp_lib_int_pow2)
    return std::has_single_bit(x);
#else
    return (x > 0) && ((x & (x - 1)) == 0);
#endif
}

template <std::unsigned_integral T>
T Pow2AlignUp(T value, T alignment)
{
    assert(IsPow2(alignment));
    assert((value + (alignment - 1)) >= value);
    return ((value + (alignment - 1)) & ~(alignment - 1));
}

template <std::unsigned_integral T>
T Pow2AlignDown(T value, T alignment)
{
    assert(IsPow2(alignment));
    return (value & ~(alignment - 1));
}

// Search the mask data from most significant bit (MSB) to least significant bit (LSB) for a set bit (1).
// The mask input must be nonzero or the index returned is undefined.
uint32_t BitScanReverse32(uint32_t mask);
uint32_t BitScanReverse64(uint64_t mask);

// Count the number of bits set in an unsigned integer (popcount).
uint32_t CountBits(uint32_t x);
uint64_t CountBits(uint64_t x);

uint32_t RoundUpToNextPow2(uint32_t x);
uint64_t RoundUpToNextPow2(uint64_t x);

uint32_t RoundUpToPow2(uint32_t x);
uint64_t RoundUpToPow2(uint64_t x);

template<std::unsigned_integral T>
constexpr T RoundDownToPow2(T x)
{
    return std::bit_floor(x);
}

inline uint32_t PackU8x4(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
    return
        (x & 0xFF) << (0 * 8) |
        (y & 0xFF) << (1 * 8) |
        (z & 0xFF) << (2 * 8) |
        (w & 0xFF) << (3 * 8);
}

inline uint32_t PackS8x4(int32_t x, int32_t y, int32_t z, int32_t w)
{
    return
        static_cast<uint32_t>(static_cast<uint8_t>(static_cast<int8_t>(x))) << (0 * 8) |
        static_cast<uint32_t>(static_cast<uint8_t>(static_cast<int8_t>(y))) << (1 * 8) |
        static_cast<uint32_t>(static_cast<uint8_t>(static_cast<int8_t>(z))) << (2 * 8) |
        static_cast<uint32_t>(static_cast<uint8_t>(static_cast<int8_t>(w))) << (3 * 8);
}

} // namespace rad
