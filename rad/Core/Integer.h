#pragma once

#include <cstdint>
#include <bit>
#include <concepts>

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

} // namespace rad
