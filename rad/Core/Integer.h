#pragma once

#include "Config.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <bit>

NAMESPACE_RAD_BEGIN

/// Extract any bitfield [firstBit, lastBit] from an integer.
template <typename T>
constexpr T BitExtract(T value, uint32_t firstBit, uint32_t lastBit)
{
    return (value >> firstBit) & ((1 << (lastBit - firstBit + 1)) - 1);
}

/// Search the mask data from most significant bit (MSB) to least significant bit (LSB) for a set bit (1).
/// The mask must be nonzero or the index returned is undefined.
uint32_t BitScanReverse32(uint32_t mask);

/// Search the mask data from most significant bit (MSB) to least significant bit (LSB) for a set bit (1).
/// The mask must be nonzero or the index returned is undefined.
uint32_t BitScanReverse64(uint64_t mask);

/// Count the number of bits set in an unsigned integer.
uint32_t CountBits(uint32_t x);

constexpr bool IsPow2(uint64_t x)
{
    return (x == 0) ? false : ((x & (x - 1)) == 0);
}

template <typename T>
T Pow2AlignUp(T value, uint64_t alignment)
{
    assert(IsPow2(alignment));
    return ((value + static_cast<T>(alignment) - 1) & ~(static_cast<T>(alignment) - 1));
}

template <typename T>
T Pow2AlignDown(T value, uint64_t alignment)
{
    assert(IsPow2(alignment));
    return (value & ~(alignment - 1));
}

uint32_t RoundUpToNextPow2(uint32_t x);
uint64_t RoundUpToNextPow2(uint64_t x);

uint32_t RoundUpToPow2(uint32_t x);
uint64_t RoundUpToPow2(uint64_t x);

template<typename T>
constexpr T RoundUpToMultiple(T value, T alignment)
{
    return (((value + (alignment - 1)) / alignment) * alignment);
}

template <typename T>
constexpr T RoundDownToMultiple(T value, T alignment)
{
    return ((value / alignment) * alignment);
}

NAMESPACE_RAD_END
